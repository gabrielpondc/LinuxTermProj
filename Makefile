# SPDX-License-Identifier: GPL-2.0-only
#
# Makefile for the linux journaling routines.
#

obj-m += rbt.o


KDIR:=/lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm *.o 
	rm *.ko 
	rm *.mod.c 
	rm .*.cmd 
	rm *.order 
	rm *.symvers 
	rm *.mod
	rm .*.tmp
	rm *.txt
