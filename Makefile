main := main.c

CC := /opt/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.2/bin/loongarch64-linux-gnu-

PWD := $(shell pwd)
TDIR := ~/tftpboot

default:
	gcc -o m main.c

cross:
	$(CC)gcc -o m aux_test.c
	cp m ~/tftpboot

test:
	$(CC)gcc -o t test.c
	cp t ~/tftpboot

clean:
	rm -rf *.o m
