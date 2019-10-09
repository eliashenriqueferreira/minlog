###########################################################################################################################################################################################################################################################
# Copyright 2019 Biteiro LTDA. All rights reserved.
#
#  Date         Author          	Subject
#  07.10.2019   Elias Henrique      Initial version
###########################################################################################################################################################################################################################################################

###########################################################################################################################################################################################################################################################
# this files may be used to override configuration from cmdline
# use *.local.mk for local configurations that won't be on the vcs
-include conf/*.mk

# declare inputs and targets
_srcs          := main.cpp minlog.c
_targets       := minlogApp
_resources     :=

# declare libs to link against
LIBS           ?=
ARLIBS         ?=

# one may declare also extra include dirs (maybe from config.mk)
EXTRA_INCDIRS  ?=
EXTRA_LIBDIRS  ?=
EXTRA_ARDIRS   ?=

# include common rules, variables, etc.
include app.mk

# include some other object files
#_objs    += $(addprefix $(_wrk_dir)/,util.o log.o)

#$(_wrk_dir)/log.o: $(_src_dir)/../../../<path to other>/log.cpp
#	$(_cross_prefix)$(CXX) -c -o $@ -MMD $(addprefix -I,$(_inc_dirs)) $(_cflags) $<
#
#$(_wrk_dir)/util.o: $(_src_dir)/../../../<path to other>/util.c
#	$(_cross_prefix)$(CXX) -c -o $@ -MMD $(addprefix -I,$(_inc_dirs)) $(_cflags) $<

# specific rule to build final executable
$(_targets): % : $(_objs)
	$(_cross_prefix)$(CXX) -o $@ $(_objs) $(addprefix -l,$(_libs)) $(addprefix -L,$(_lib_dirs)) $(_arlibs) $(_ldflags)


.PHONY: run
run:
##ifeq ($(findstring xterm-256color,$(TERM)),)
##	@echo "Este programa nao roda fora da maquina virtual atraves do make!"
##else
	@echo "Rodando o programa: $(_targets)"
	@$(_targets)
##endif
