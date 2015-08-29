/**
 * kgdt_idt.c
 */
#include "KernelFunc.h"

/* GDT IDT */
void init_gdtidt(void)
{
	struct SEGMENT_DESCRIPTOR	*gdt = (struct SEGMENT_DESCRIPTOR*)	0x00010000;
	struct GATE_DESCRIPTOR		*idt = (struct GATE_DESCRIPTOR*)	0x00030000;
	int i;

	/* GDT */
	for (i = 0; i < 8192; i++) {
		set_segmdesc(gdt + i, 0, 0, 0);	
	}
	
	set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, AR_DATA32_RW); // Data
	set_segmdesc(gdt + 2, 0x0007ffff, 0x00100000, AR_CODE32_ER); // Kernel
	load_gdtr(0xffff, 0x00010000);
	
	/* IDT */
	for (i = 0; i < 256; i++) {
		set_gatedesc(idt + i, 0, 0, 0);
	}

	// Broken here
	set_gatedesc(idt + 0x20, (int) asm_inthandler20, 2<<3, AR_INTGATE32); // Timer
	//set_gatedesc(idt + 0x0c, (int) asm_inthandler0c, 2 * 8, AR_INTGATE32); // Timer
	//set_gatedesc(idt + 0x0d, (int) asm_inthandler0d, 2 * 8, AR_INTGATE32); // Timer
	
	load_idtr(0x7ff, 0x00030000);

	kprintf ("GDT and IDT inited\n");

	return;
}

void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar)
{
	if (limit > 0xfffff) {
		ar |= 0x8000;
		limit /= 0x1000;
	}
	sd->limit_low	 = limit & 0xffff;
	sd->base_low	 = base & 0xffff;
	sd->base_mid	 = (base >> 16) & 0xff;
	sd->access_right = ar & 0xff;
	sd->limit_high	 = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
	sd->base_high	 = (base >> 24) & 0xff;
	return;
}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar)
{
	gd->offset_low	 = offset & 0xffff;
	gd->selector	 = selector;
	gd->dw_count	 = (ar >> 8) & 0xff;
	gd->access_right = ar & 0xff;
	gd->offset_high	 = (offset >> 16) & 0xffff;
	return;
}

