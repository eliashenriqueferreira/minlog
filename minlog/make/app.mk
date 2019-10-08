###########################################################################################################################################################################################################################################################
# Copyright 2019 Biteiro LTDA. All rights reserved.
#
#  Date         Author          	Subject
#  07.10.2019   Elias Henrique      Initial version
###########################################################################################################################################################################################################################################################

###########################################################################################################################################################################################################################################################
# app.mk - common content for aplication Makefiles
###########################################################################################################################################################################################################################################################


# these lines are suggested by GNU make documentation
SHELL = /bin/sh
.SUFFIXES:


# default target
.PHONY: default
default: all


# externally defined vars
TARGET         ?= x86_64-ue
X_TOOLS_DIR    ?= /opt/x-tools
BUILD_DEBUG    ?=
CFLAGS         ?=
CXXFLAGS       ?=
LDFLAGS        ?=
CROSS          ?= no
AR             ?= ar
CC             ?= gcc-7
CXX            ?= g++-7
INSTALL        ?= install
INSTALL_PROGRAM ?= $(INSTALL)
INSTALL_DATA   ?= $(INSTALL) -m 644
LD             ?= ld
MAKE           ?= make
DESTDIR        ?= $(abspath ../../release)
EXPORT         ?= export


# internal vars
_use_cross     := $(if $(findstring no,$(CROSS)),,1)
_cflags        := -fPIC $(CFLAGS)
_cxxflags      := -fPIC $(CXXFLAGS)
_ldflags       := $(LDFLAGS)
_libs          := $(LIBS) dl
_arlibs        := $(ARLIBS)
_cross_prefix  := $(if $(_use_cross),$(X_TOOLS_DIR)/$(TARGET)-linux-gnu/bin/$(TARGET)-linux-gnu-)
_sysroot       := $(if $(_use_cross),$(X_TOOLS_DIR)/$(TARGET)-linux-gnu/$(TARGET)-linux-gnu/sysroot)
_src_dir       := ../src
_inc_dirs      := ../inc
_lib_dirs      := ../lib
_res_dirs      := ../res
_wrk_dir       := ../wrk/release
_inc_dirs      := $(_inc_dirs) $(EXTRA_INCDIRS)
_lib_dirs      := $(_lib_dirs) $(EXTRA_LIBDIRS)
#_flavor        := $(if $(findstring 1,$(BUILD_DEBUG)),debug,release)
_flavor        := $(FLAVOR)

# changes for DEBUG (and any other flavor)
ifeq ($(_flavor),debug)
_cflags        := -g3 -O0 -DBUILD_DEBUG $(_cflags)
_cxxflags      := -g3 -O0 -DBUILD_DEBUG $(_cxxflags)
_wrk_dir       := ../wrk/debug
endif

_srcs          := $(addprefix $(_src_dir)/,$(_srcs))
_objs          := $(subst .cpp,.o,$(_srcs))
_objs          := $(subst .c,.o,$(_objs))
_objs          := $(subst $(_src_dir),$(_wrk_dir),$(_objs))
_deps          := $(subst .o,.d,$(_objs))
_arlibs        := $(addprefix lib,$(addsuffix .a,$(_arlibs)))
_arlibs        := $(realpath $(foreach d,$(EXTRA_ARDIRS),$(addprefix $(d)/,$(_arlibs))))
_targets       := $(addprefix $(_wrk_dir)/,$(_targets))
_resources     := $(foreach d,$(_res_dirs),$(addprefix $(d)/,$(_resources)))
_incstoexport  := $(realpath $(foreach d,$(_inc_dirs),$(addprefix $(d)/,$(_incstoexport))))


# internal macros



# external targets
.PHONY: all build
all: build
build: check_dirs $(_targets)

# implicit rule to compile C source files
$(_wrk_dir)/%.o: $(_src_dir)/%.c
	$(_cross_prefix)$(CC) -c -o $@ -MMD $(addprefix -I,$(_inc_dirs)) $(_cflags) $<

# implicit rule to compile C++ source files
$(_wrk_dir)/%.o: $(_src_dir)/%.cpp
	$(_cross_prefix)$(CXX) -c -o $@ -MMD $(addprefix -I,$(_inc_dirs)) $(_cxxflags) $<


.PHONY: clean
clean:
	rm -f $(_objs) $(_targets) $(_deps)
#	rm -f $(realpath $(_objs)) $(_targets)

.PHONY: rebuild
rebuild:
	@$(MAKE) clean
	@$(MAKE) all


.PHONY: install
install: build
	$(INSTALL) -m 755 -d $(DESTDIR)
	$(if $(_targets),$(INSTALL) -m 755 -t $(DESTDIR) $(_targets))
	$(if $(_resources),$(INSTALL_DATA) -t $(DESTDIR) $(_resources))
	$(if $(_incstoexport),$(INSTALL_DATA) -t $(DESTDIR) $(_incstoexport))


.PHONY: debug
debug: 
	@echo debug > flavor.txt
	@$(MAKE) vars
	
.PHONY: undebug
undebug: 
	@echo release > flavor.txt
	@$(MAKE) vars
	
  
.PHONY: help
help:
	@echo "Targets:"
	@echo ". clean ........... clean all temporary and final artifacts"
	@echo ". all ............. build all artifacts"
	@echo ". rebuild ......... clean then all"
	@echo ". install ......... install deliverables"
	@echo ". run ............. run binary"
	@echo ". help ............ show help"
	@echo ". debug ........... changes flavor to debug"
	@echo ". undebug ......... changes flavor to release"
	@echo ". vars ............ show vars"

.PHONY: vars
vars:
	@echo "External vars:"
	@echo ". TARGET .......... [$(TARGET)]"
	@echo ". X_TOOLS_DIR ..... [$(X_TOOLS_DIR)]"
	@echo ". BUILD_DEBUG ..... [$(BUILD_DEBUG)]"
	@echo ". CFLAGS .......... [$(CFLAGS)]"
	@echo ". CXXFLAGS ........ [$(CXXFLAGS)]"
	@echo ". LDFLAGS ......... [$(LDFLAGS)]"
	@echo ". LIBS ............ [$(LIBS)]"
	@echo ". ARLIBS .......... [$(ARLIBS)]"
	@echo ". EXTRA_INCDIRS ... [$(EXTRA_INCDIRS)]"
	@echo ". EXTRA_LIBDIRS ... [$(EXTRA_LIBDIRS)]"
	@echo ". EXTRA_ARDIRS .... [$(EXTRA_ARDIRS)]"
	@echo ". CROSS ........... [$(CROSS)]"
	@echo ". AR .............. [$(AR)]"
	@echo ". CC .............. [$(CC)]"
	@echo ". CXX ............. [$(CXX)]"
	@echo ". INSTALL ......... [$(INSTALL)]"
	@echo ". LD .............. [$(LD)]"
	@echo ". MAKE ............ [$(MAKE)]"
	@echo ". DESTDIR ......... [$(DESTDIR)]"
	@echo "Internal vars:"
	@echo ". _use_cross ...... [$(_use_cross)]"
	@echo ". _cflags ......... [$(_cflags)]"
	@echo ". _cxxflags ....... [$(_cxxflags)]"
	@echo ". _ldflags ........ [$(_ldflags)]"
	@echo ". _libs ........... [$(_libs)]"
	@echo ". _arlibs ......... [$(_arlibs)]"
	@echo ". _cross_prefix ... [$(_cross_prefix)]"
	@echo ". _sysroot ........ [$(_sysroot)]"
	@echo ". _src_dir ........ [$(_src_dir)]"
	@echo ". _inc_dirs ....... [$(_inc_dirs)]"
	@echo ". _lib_dirs ....... [$(_lib_dirs)]"
	@echo ". _res_dirs ....... [$(_res_dirs)]"
	@echo ". _flavor ......... [$(_flavor)]"
	@echo ". _wrk_dir ........ [$(_wrk_dir)]"
	@echo ". _srcs ........... [$(_srcs)]"
	@echo ". _targets ........ [$(_targets)]"
	@echo ". _resources ...... [$(_resources)]"
	@echo ". _incstoexport ... [$(_incstoexport)]"
	@echo ". _objs ........... [$(_objs)]"
	@echo ". _deps ........... [$(_deps)]"


# internal auxiliary targets
.PHONY: check_dirs
check_dirs:
	@test -d "$(_wrk_dir)" || mkdir -p "$(_wrk_dir)"
	@$(foreach d,$(dir $(_objs)),test -d "$(d)" || mkdir -p "$(d)";) 


# last includes
-include $(patsubst %.o,%.d,$(_objs))
