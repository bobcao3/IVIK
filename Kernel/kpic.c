/**
 * kpic.c
 */
#include "KernelFunc.h"

/* PIC */
#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

void init_pic(void)
{
	io_out8(PIC0_IMR, 0xff);
	io_out8(PIC1_IMR, 0xff);

	io_out8(PIC0_ICW1, 0x11	); // edge trigger mode
	io_out8(PIC0_ICW2, 0xf0	); // IRQ0-7 becomes int f0h - f7h
	io_out8(PIC0_ICW3, 1<<2	); // connect PIC1 by IRQ2
	io_out8(PIC0_ICW4, 0x01	); // no buffer mode
	
	io_out8(PIC1_ICW1, 0x11	); // edge trigger mode
	io_out8(PIC1_ICW2, 0xf8	); // IRQ8-15 becomes int f8h - ffh
	io_out8(PIC1_ICW3, 2	); // connect PIC1 by IRQ2
	io_out8(PIC1_ICW4, 0x01	); // no buffer mode

	io_out8(PIC0_IMR, 0xfb	);
	io_out8(PIC1_IMR, 0xff	);

	kprintf("PIC inited\n");

	return;
}
