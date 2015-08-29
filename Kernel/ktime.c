/**
 * ktime.c
 */
#include "KernelFunc.h"

/* Time */
void get_time(void)
{
	io_out8(0x70, 0x09);
	time.yy=io_in8(0x71);
	io_out8(0x70, 0x08);
	time.mm=io_in8(0x71);
	io_out8(0x70, 0x07);
	time.dd=io_in8(0x71);
	io_out8(0x70, 0x04);
	time.h=io_in8(0x71);
	io_out8(0x70, 0x02);
	time.m=io_in8(0x71);
	io_out8(0x70, 0x00);
	time.s=io_in8(0x71);

	kprintf("Now is : (UTC)%x/%x/%x %x:%x:%x\n", time.yy, time.mm, time.dd, time.h, time.m, time.s);
	
	return;
}

void inthandlerf0(int *esp)
{
	io_out8(PIC0_OCW2, 0x60);
	kprintf("INT F0h!\n");
	timerctl.tick++;
	return;
}

#define PIT_CTRL	0x0043
#define PIT_CNT0	0x0040

void init_pit(void)
{
	io_out8(PIT_CTRL, 0x34);
	io_out8(PIT_CNT0, 0x9c);
	io_out8(PIT_CNT0, 0x2e);
	timerctl.tick = 0;
	io_out8(PIC0_IMR, 0xfe); // Enable Timer
	io_out8(PIC1_IMR, 0xff);
	
	kprintf("PIT Inited\n");
	get_time();
	return;
}
