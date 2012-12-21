/*===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
*/

#include <kfs/extern.h>
#include <kfs/lockfile.h>
#include <kfs/impl.h>
#include <kfs/file.h>
#include <kfs/directory.h>
#include <klib/text.h>
#include <klib/refcount.h>
#include <klib/rc.h>
#include <sysalloc.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>


/*--------------------------------------------------------------------------
 * KLockFile
 *  a mutual exclusion lock on a file
 *  the lock is acquired upon creation
 *  and released upon destruction
 *
 * NB - only guaranteed to work when used from a single host
 */
struct KLockFile
{
    KDirectory *dir;
    KRefcount refcount;
    char path [ 1 ];
};

static
rc_t KLockFileWhack ( KLockFile *self )
{
    /* remove the lock file from file system */
    rc_t rc = KDirectoryRemove ( self -> dir, true, self -> path );
    if ( rc != 0 )
        return rc;

    KDirectoryRelease ( self -> dir );
    free ( self );
    return 0;
}


/* AddRef
 */
LIB_EXPORT rc_t CC KLockFileAddRef ( const KLockFile *self )
{
    if ( self != NULL )
    {
        switch ( KRefcountAdd ( & self -> refcount, "KLockFile" ) )
        {
        case krefLimit:
            return RC ( rcFS, rcLock, rcAttaching, rcRange, rcExcessive );
        }
    }
    return 0;
}

/* Release
 */
LIB_EXPORT rc_t CC KLockFileRelease ( const KLockFile *self )
{
    if ( self != NULL )
    {
        switch ( KRefcountDrop ( & self -> refcount, "KLockFile" ) )
        {
        case krefWhack:
            return KLockFileWhack ( ( KLockFile* ) self );
        case krefLimit:
            return RC ( rcFS, rcLock, rcReleasing, rcRange, rcExcessive );
        }
    }
    return 0;
}

/* Make
 */
static
rc_t KLockFileMake ( KLockFile **lock, KDirectory *dir, const char *path )
{
    rc_t rc;
    size_t path_size = string_size ( path );
    KLockFile *f = malloc ( sizeof * f + path_size );
    if ( f == NULL )
        rc = RC ( rcFS, rcLock, rcConstructing, rcMemory, rcExhausted );
    else
    {
        rc = KDirectoryAddRef ( f -> dir = dir );
        if ( rc == 0 )
        {
            strcpy ( f -> path, path );
            KRefcountInit ( & f -> refcount, 1, "KLockFile", "make", path );
            * lock = f;
            return 0;
        }

        free ( f );
    }

    return rc;
}


/*--------------------------------------------------------------------------
 * KDirectory
 *  interface extensions
 */


/* CreateLockFile
 *  attempts to create a KLockFile
 *
 *  "lock" [ OUT ] - return parameter for newly created lock file
 *
 *  "path" [ IN ] - NUL terminated string in directory-native
 *  character set denoting lock file
 */
LIB_EXPORT rc_t CC KDirectoryVCreateLockFile ( KDirectory *self,
    KLockFile **lock, const char *path, va_list args )
{
    rc_t rc;

    if ( lock == NULL )
        rc = RC ( rcFS, rcFile, rcLocking, rcParam, rcNull );
    else
    {
        if ( self == NULL )
            rc = RC ( rcFS, rcFile, rcLocking, rcSelf, rcNull );
        else if ( path == NULL )
            rc = RC ( rcFS, rcFile, rcLocking, rcPath, rcNull );
        else if ( path [ 0 ] == 0 )
            rc = RC ( rcFS, rcFile, rcLocking, rcPath, rcEmpty );
        else
        {
            char full [ 4096 ];
            rc = KDirectoryVResolvePath ( self, false, full, sizeof full, path, args );
            if ( rc == 0 )
            {
                KFile *lock_file;
                rc = KDirectoryCreateFile ( self, & lock_file, false, 0200, kcmCreate | kcmParents, full );
                if ( rc == 0 )
                {
                    rc_t rc2;

                    /* no longer need the file - not going to write to it anyway */
                    KFileRelease ( lock_file );

                    /* we have the lock */
                    rc = KLockFileMake ( lock, self, full );
                    if ( rc == 0 )
                        return 0;

                    /* must unlink lockfile */
                    rc2 = KDirectoryRemove ( self, true, full );
                    if ( rc2 != 0 )
                        /* issue a report */;
                }
                else if ( GetRCState ( rc ) == rcExists )
                {
                    /* map the rc to kproc type values */
                    rc = RC ( rcFS, rcFile, rcLocking, rcLocking, rcBusy );
                }
                else
                {
                    rc = ResetRCContext ( rc, rcFS, rcFile, rcLocking );
                }
            }
        }

        * lock = NULL;
    }

    return rc;
}

LIB_EXPORT rc_t CC KDirectoryCreateLockFile ( KDirectory *self,
    KLockFile **lock, const char *path, ... )
{
    rc_t rc;

    va_list args;
    va_start ( args, path );

    rc = KDirectoryVCreateLockFile ( self, lock, path, args );

    va_end ( args );

    return rc;
}


/* CreateExclusiveAccessFile
 *  opens a file with exclusive write access
 *
 *  "f" [ OUT ] - return parameter for newly opened file
 *
 *  "update" [ IN ] - if true, open in read/write mode
 *  otherwise, open in write-only mode
 *
 *  "access" [ IN ] - standard Unix access mode, e.g. 0664
 *
 *  "mode" [ IN ] - a creation mode ( see explanation above ).
 *
 *  "path" [ IN ] - NUL terminated string in directory-native
 *  character set denoting target file
 */

LIB_EXPORT rc_t CC KDirectoryCreateExclusiveAccessFile ( KDirectory *self, KFile **f,
    bool update, uint32_t access, KCreateMode mode, const char *path, ... )
{
    rc_t rc;

    va_list args;
    va_start ( args, path );

    rc = KDirectoryVCreateExclusiveAccessFile ( self, f, update, access, mode, path, args );

    va_end ( args );

    return rc;
}
