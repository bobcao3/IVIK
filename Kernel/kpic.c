/**
 * kpic.c
 */
#include "KernelFunc.h"

/* PIC */
void init_pic(void)
{
	io_out8(PIC0_IMR, 0xff);
	io_out8(PIC1_IMR, 0xff);

	io_out8(PIC0_ICW1, 0x11	); // edge trigger mode
	io_out8(PIC0_ICW2, 0x20	); // IRQ0-7 becomes int f0h - f7h
	io_out8(PIC0_ICW3, 1<<2	); // connect PIC1 by IRQ2
	io_out8(PIC0_ICW4, 0x01	); // no buffer mode
	
	io_out8(PIC1_ICW1, 0x11	); // edge trigger mode
	io_out8(PIC1_ICW2, 0x28	); // IRQ8-15 becomes int f8h - feh
	io_out8(PIC1_ICW3, 2	); // connect PIC1 by IRQ2
	io_out8(PIC1_ICW4, 0x01	); // no buffer mode

	io_out8(PIC0_IMR, 0xfb	);
	io_out8(PIC1_IMR, 0xff	);

	kprintf("PIC inited\n");

	return;
}

void inthandler27(int *esp)
{
	io_out8(PIC0_OCW2, 0x67);
	return;
}

