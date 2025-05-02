PRJ_NATIVE_MK_FILE	:= $(realpath $(lastword $(MAKEFILE_LIST)))
PRJ_NATIVE_MK_DIR	:= $(shell dirname $(PRJ_NATIVE_MK_FILE))

I_HDD00			:= $(HOME)/iHDD00
MK_RACCOON_DIR		:= $(I_HDD00)/08.PROJECT/mk-raccoon

EXTRAINCDIRS		+= $(PRJ_NATIVE_MK_DIR)/inc
VPATH			+= $(PRJ_NATIVE_MK_DIR)/src_c
VPATH			+= $(PRJ_NATIVE_MK_DIR)/src_cpp

CFLAGS			+= -Wall -Werror -Wextra -Wno-unused-parameter

OPT			:= g

include $(MK_RACCOON_DIR)/gcc_native.mk
# include $(MK_RACCOON_DIR)/llvm_native.mk
