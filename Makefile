#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

#  git submodule update --init --recursive

include dpf/Makefile.base.mk

all: dgl plugins gen

init_git_submodules: 
	git submodule update --init --recursive

fmt:
	clang-format  -i plugins/yasz/lib/*
# --------------------------------------------------------------

dgl:
	$(MAKE) -C dpf/dgl

plugins: dgl
	$(MAKE) all -C plugins/yasz

ifneq ($(CROSS_COMPILING),true)
gen: plugins dpf/utils/lv2_ttl_generator
	@$(CURDIR)/dpf/utils/generate-ttl.sh
ifeq ($(MACOS),true)
	@$(CURDIR)/dpf/utils/generate-vst-bundles.sh
endif

dpf/utils/lv2_ttl_generator:
	$(MAKE) -C dpf/utils/lv2-ttl-generator
else
gen:
endif

# --------------------------------------------------------------

clean:
	$(MAKE) clean -C dpf/dgl
	$(MAKE) clean -C dpf/utils/lv2-ttl-generator
	$(MAKE) clean -C plugins/yasz
	rm -rf bin build

install:
	install -d ${HOME}/.lv2/yasz.lv2/
	install -m 644 -D bin/yasz.lv2/* ${HOME}/.lv2/yasz.lv2/

style:
	astyle --options="astyle.cfg" "plugins/yasz/lib/*.c,*.h"
# --------------------------------------------------------------

.PHONY: plugins
