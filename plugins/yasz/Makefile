#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = yasz

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	YaszPlugin.cpp \
	lib/onepole.c  \
	lib/osc.c lib/utils.c  \
	lib/yasz.c lib/adsr.c  \
	lib/midi.c lib/voice.c \
	lib/osct.c lib/wave.c \
	lib/ks.c


FILES_UI  = \
	YaszUI.cpp

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
