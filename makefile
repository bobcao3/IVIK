Kernel/kernel: Kernel/
	make -C $<

run: Kernel/kernel
	qemu-system-i386 -kernel Kernel/kernel &
