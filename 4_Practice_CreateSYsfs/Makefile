obj-m += demo.o

KDIR := /home/tuananh/Documents/work/BeagleBone/driver/kernelbuildscripts/KERNEL
PWD  := $(shell pwd)

ARCH ?= arm

# Toolchain đúng version (GCC 14.3.0) mà bb-kernel đã tải về khi build kernel
TOOLCHAIN_BIN := /home/tuananh/Documents/work/BeagleBone/driver/kernelbuildscripts/dl/gcc-14.3.0-nolibc/arm-linux-gnueabi/bin

ifeq ($(ARCH), arm)
	CROSS_COMPILE := $(TOOLCHAIN_BIN)/arm-linux-gnueabi-
else
	CROSS_COMPILE :=
endif

all:
	make -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	make -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean

deploy:
	scp demo.ko debian@<BBB_IP>:/home/debian/
