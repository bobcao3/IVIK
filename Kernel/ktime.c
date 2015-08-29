/**
 * ktime.c
 */
#include "KernelFunc.h"

/* Time */
void get_time()
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
