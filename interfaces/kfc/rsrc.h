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

#ifndef _h_kfc_rsrc_
#define _h_kfc_rsrc_

#ifndef _h_kfc_defs_
#include <kfc/defs.h>
#endif

/*--------------------------------------------------------------------------
 * forwards
 */
struct KMemMgr;
struct KConfig;
struct KDBManager;
struct VDBManager;


/*--------------------------------------------------------------------------
 * KRsrc
 *  a very watered-down version of vdb-3 resource capabilities
 */
typedef struct KRsrc KRsrc;
struct KRsrc
{
    struct KMemMgr * mem;
    struct KConfig * cfg;
    struct KDBManager * kdb;
    struct VDBManager * vdb;
};


/* Init
 *  initialize a local block from another
 */
void KRsrcInit ( KRsrc *rsrc, ctx_t ctx );


/* Whack
 *  release references
 */
void KRsrcWhack ( KRsrc *self, ctx_t ctx );


/*--------------------------------------------------------------------------
 * KRsrcBits
 *  bitfield definitions for optional resources
 *  for use in hybrid approach of VDB-2
 */
enum KRsrcBits
{
    rbKDBManager = ( 1 << 0 ),
    rbVDBManager = ( 1 << 1 )
};

#endif /* _h_kfc_rsrc_ */