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


default: std

TOP ?= $(shell ../../build/abspath.sh ../..)
MODULE = libs/align

INT_LIBS = \
	libncbi-bam \
	libalign-writer \
	libalign-access \
	libalign-reader

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
# libncbi-bam
#
$(ILIBDIR)/libncbi-bam: $(addprefix $(ILIBDIR)/libncbi-bam.,$(ILIBEXT))

BAM_SRC = \
	bam

BAM_OBJ = \
	$(addsuffix .$(LOBX),$(BAM_SRC))

BAM_LIB = 

$(ILIBDIR)/libncbi-bam.$(LIBX): $(BAM_OBJ)
	$(LD) --slib -o $@ $^ $(BAM_LIB)


#-------------------------------------------------------------------------------
# align-access
#
$(ILIBDIR)/libalign-access: $(addprefix $(ILIBDIR)/libalign-access.,$(ILIBEXT))

ALIGN_ACCESS_SRC = \
	align-access

ALIGN_ACCESS_OBJ = \
	$(addsuffix .$(LOBX),$(ALIGN_ACCESS_SRC))

ALIGN_ACCESS_LIB = 

$(ILIBDIR)/libalign-access.$(LIBX): $(ALIGN_ACCESS_OBJ)
	$(LD) --slib -o $@ $^ $(ALIGN_ACCESS_LIB)

#-------------------------------------------------------------------------------
# align-reader
#
$(ILIBDIR)/libalign-reader: $(addprefix $(ILIBDIR)/libalign-reader.,$(ILIBEXT))

ALIGN_READER_SRC = \
	align_mgr \
	al_iterator \
	ref_iterator \
	pl_iterator \
	dna-reverse-cmpl \
	reference-cmn \
	reader-cmn \
	reader-refseq \
	reference \
	refseq-mgr \
	quality-quantizer

ALIGN_READER_OBJ = \
	$(addsuffix .$(LOBX),$(ALIGN_READER_SRC))

ALIGN_READER_LIB = 

$(ILIBDIR)/libalign-reader.$(LIBX): $(ALIGN_READER_OBJ)
	$(LD) --slib -o $@ $^ $(ALIGN_READER_LIB)

#-------------------------------------------------------------------------------
# align-writer
#
$(ILIBDIR)/libalign-writer: $(addprefix $(ILIBDIR)/libalign-writer.,$(LIBX))

ALIGN_WRITER_SRC = \
    dna-reverse-cmpl \
	reader-cmn \
	reference-cmn \
	reader-refseq \
	refseq-mgr \
	writer-cmn \
	writer-refseq \
	writer-alignment \
	writer-sequence \
	writer-ref \
	writer-reference

ALIGN_WRITER_OBJ = \
	$(addsuffix .$(LOBX),$(ALIGN_WRITER_SRC))

ALIGN_WRITER_LIB = 

$(ILIBDIR)/libalign-writer.$(LIBX): $(ALIGN_WRITER_OBJ)
	$(LD) --slib -o $@ $^ $(ALIGN_WRITER_LIB)
