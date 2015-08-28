CC = /opt/32/bin/gcc
LD = /opt/32/bin/ld
OBJS = kboot.o kboot_head.o KernelFunc.o

kboot.o: kboot.c 
	${CC} -c -o $@ $? -fno-builtin -O3 -Ofast

kboot_head.o: kboot_head.S
	${CC} -c -o $@ $?

KernelFunc.o: KernelFunc.asm
	nasm -f elf -o $@ $?

kernel: ${OBJS}
	${LD} $? -s -o $@ -Ttext 0x100000 --entry=start

clean:
	rm -f *.o
	rm -f kernel
