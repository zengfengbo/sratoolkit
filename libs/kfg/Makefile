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
MODULE = libs/kfg

INT_LIBS = \
	libkfg

ALL_LIBS = \
	$(INT_LIBS)

include $(TOP)/build/Makefile.env

#-------------------------------------------------------------------------------
# outer targets
#
all std: makedirs
	@ $(MAKE_CMD) $(TARGDIR)/std

$(INT_LIBS): makedirs
	@ $(MAKE_CMD) $(ILIBDIR)/$@

.PHONY: all std $(ALL_LIBS)

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

.PHONY: clean

#-------------------------------------------------------------------------------
# kfg
#
$(ILIBDIR)/libkfg: $(TARGDIR)/default.kfg $(addprefix $(ILIBDIR)/libkfg.,$(ILIBEXT))

$(TARGDIR)/default.kfg: $(NCBIDIR)/default.kfg $(TEST_BINDIR)/ncbi/default.kfg

$(NCBIDIR)/default.kfg: $(SRCDIR)/default.kfg
	rm -f $(NCBIDIR)/config.kfg
	cp $^ $@

ifneq ($(NCBIDIR),$(TEST_BINDIR)/ncbi)
$(TEST_BINDIR)/ncbi/default.kfg: $(SRCDIR)/default.kfg
	mkdir -p $(TEST_BINDIR)/ncbi
	cp $^ $@
endif

KFG_SRC = \
	config-grammar \
	config-lex \
	config \
	kart \
	repository \
	report-kfg \
    keystore 

KFG_OBJ = \
	$(addsuffix .$(LOBX),$(KFG_SRC))

KFG_LIB = \
	-dkfs \
	-dklib

$(ILIBDIR)/libkfg.$(LIBX): $(INTERM_SRC) $(KFG_OBJ)
	$(LD) --slib -o $@ $^ $(KFG_LIB)

