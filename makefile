Kernel/kernel: Kernel/
	make -C $? kernel

run: Kernel/kernel
	qemu-system-i386 -kernel Kernel/kernel -rtc base=utc &

clean: Kernel/
	make -C $? clean
