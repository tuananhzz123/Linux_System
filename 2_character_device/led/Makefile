PREFIX := arm-linux-gnueabihf-

# Mặc định ARCH là arm nếu chưa truyền từ bên ngoài
ARCH ?= arm

ifeq ($(ARCH), arm)
    CC := $(PREFIX)gcc
    KBUILD_ARGS := ARCH=arm CROSS_COMPILE=$(PREFIX)
else
    CC := gcc
    KBUILD_ARGS :=
endif

# Thư mục chứa Kernel Source/Headers cho BBB
KDIR ?= /home/tuananh/Documents/work/BeagleBone/driver/kernelbuildscripts/KERNEL

# File driver hello.c sẽ tạo ra hello.o -> hello.ko
obj-m += hello.o

all: module app

module:
	make -C $(KDIR) M=$(PWD) $(KBUILD_ARGS) modules

app: app.c
	$(CC) -o app app.c

clean:
	make -C $(KDIR) M=$(PWD) $(KBUILD_ARGS) clean
	rm -rf app hello

.PHONY: all module app clean