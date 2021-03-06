# ===========================================================================
#
#                            PUBLIC DOMAIN NOTICE
#               National Center for Biotechnology Information
#
#  This software/database is a "United States Government Work" under the
#  terms of the United States Copyright Act.  It was written as part of
#  the author's official duties as a United States Government employee and
#  thus cannot be copyrighted.  This software/database is freely available
#  to the public for use. The National Library of Medicine and the U.S.
#  Government have not placed any restriction on its use or reproduction.
#
#  Although all reasonable efforts have been taken to ensure the accuracy
#  and reliability of the software and data, the NLM and the U.S.
#  Government do not and cannot warrant the performance or results that
#  may be obtained by using this software or data. The NLM and the U.S.
#  Government disclaim all warranties, express or implied, including
#  warranties of performance, merchantability or fitness for any particular
#  purpose.
#
#  Please cite the author in any work or product based on this material.
#
# ===========================================================================

default: all

TOP ?= $(shell ../../build/abspath.sh ../..)
MODULE = libs/vfs

INT_LIBS = \
	libvfs

ALL_LIBS = \
	$(INT_LIBS)

TEST_TOOLS = \

include $(TOP)/build/Makefile.env

RWORKDIR=$(TOP)/$(MODULE)

#-------------------------------------------------------------------------------
# outer targets
#
all std: makedirs
	@ $(MAKE_CMD) $(TARGDIR)/std

$(INT_LIBS): makedirs
	@ $(MAKE_CMD) $(ILIBDIR)/$@

$(TEST_TOOLS): makedirs
	@ $(MAKE_CMD) $(TEST_BINDIR)/$@

.PHONY: all std $(ALL_LIBS) $(TEST_TOOLS)

#-------------------------------------------------------------------------------
# std
#
$(TARGDIR)/std: \
	$(addprefix $(ILIBDIR)/,$(INT_LIBS))

.PHONY: $(TARGDIR)/std

#-------------------------------------------------------------------------------
# clean
#
clean: stdclean
	@ rm -f $(addsuffix *,$(addprefix $(TEST_BINDIR)/,$(TEST_TOOLS)))

.PHONY: clean

#-------------------------------------------------------------------------------
# vfs
#
$(ILIBDIR)/libvfs: $(addprefix $(ILIBDIR)/libvfs.,$(ILIBEXT))

VFS_SRC_CMN = \
	syspath \
	manager  \
	resolver

VFS_SRC = \
	path \
	$(VFS_SRC_CMN)

VFS_OBJ = \
	$(addsuffix .$(LOBX),$(VFS_SRC))

VFS_LIB = \
	-lkns \
	-lkns \
	-lkrypto \
	-lkfg \
	-lkfs \
	-lklib

$(ILIBDIR)/libvfs.$(LIBX): $(VFS_OBJ)
	$(LD) --slib -o $@ $^ $(VFS_LIB)



#-------------------------------------------------------------------------------
# white-box test
#

TEST_SRC = \
	path-test

TEST_OBJ = \
	$(addsuffix .$(OBJX),$(TEST_SRC)) \
	$(addsuffix .$(LOBX),$(VFS_SRC_CMN))

TEST_LIB = \
	-skapp \
	-skns \
	-skrypto \
	-skfg \
	-skfs \
	-skproc \
	-sklib 

$(TEST_BINDIR)/path-test: $(TEST_OBJ)
	$(LD) --exe -o $@ $^ $(TEST_LIB)
