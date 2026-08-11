PREFIX := arm-linux-gnueabihf-

ifeq ($(ARCH), arm)
	CC := $(PREFIX)gcc
else
	CC := gcc
endif

all:
	$(CC) -o hello main.c
clean:
	rm -rf hello
