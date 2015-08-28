Kernel/kernel: Kernel/
	make -C $? kernel

run: Kernel/kernel
	qemu-system-i386 -kernel Kernel/kernel &

clean: Kernel/
	make -C $? clean
