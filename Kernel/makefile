CC = /opt/32/bin/gcc
LD = /opt/32/bin/ld

kernel: kboot.c kboot_head.S
	${CC} kboot.c -c -fno-builtin
	${CC} kboot_head.S -c
	${LD} kboot.o kboot_head.o -s -o kernel -Ttext 0x100000 --entry=start

clean:
	rm *.o
	rm kernel
