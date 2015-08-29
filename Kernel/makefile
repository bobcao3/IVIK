CC = /opt/32/bin/gcc
LD = /opt/32/bin/ld
OBJS = KernelFunc.o k8024display.o kgdt_idt.o kpic.o ktime.o kboot_head.o kboot.o

kboot.o: kboot.c 
	${CC} -c -o $@ $? -fno-builtin -O3 -Ofast

kboot_head.o: kboot_head.S
	${CC} -c -o $@ $?

k8024display.o: k8024display.c
	${CC} -c -o $@ $? -fno-builtin -O3 -Ofast

kpic.o: kpic.c
	${CC} -c -o $@ $? -fno-builtin -O3 -Ofast

kgdt_idt.o: kgdt_idt.c
	${CC} -c -o $@ $? -fno-builtin -O3 -Ofast

ktime.o: ktime.c
	${CC} -c -o $@ $? -fno-builtin -O3 -Ofast

KernelFunc.o: KernelFunc.asm
	nasm -f elf -o $@ $?

kernel: ${OBJS}
	${LD} $? -s -o $@ -Ttext 0x100000 --entry=start

clean:
	rm -f *.o
	rm -f kernel
