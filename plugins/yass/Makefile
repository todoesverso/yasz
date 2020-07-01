#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = yass

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	YassPlugin.cpp \
	lib/osc.c lib/utils.c

FILES_UI  = \
	YassUI.cpp

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk
BASE_FLAGS += -fdiagnostics-color
# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(HAVE_JACK),true)
ifeq ($(HAVE_OPENGL),true)
TARGETS += jack
endif
endif

TARGETS += vst

ifeq ($(HAVE_OPENGL),true)
TARGETS += lv2_sep
else
TARGETS += lv2_dsp
endif

all: $(TARGETS)

# --------------------------------------------------------------
