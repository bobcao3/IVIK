/**
 * kernel.c
 */

/* a.out 符号表。 */
typedef struct aout_symbol_table
{
    unsigned long tabsize;
    unsigned long strsize;
    unsigned long addr;
    unsigned long reserved;
} aout_symbol_table_t;

/* ELF 的 section header table。 */
typedef struct elf_section_header_table
{
    unsigned long num;
    unsigned long size;
    unsigned long addr;
    unsigned long shndx;
} elf_section_header_table_t;

/* Multiboot 信息。 */
typedef struct multiboot_info
{
    unsigned long flags;
    unsigned long mem_lower;
    unsigned long mem_upper;
    unsigned long boot_device;
    unsigned long cmdline;
    unsigned long mods_count;
    unsigned long mods_addr;
    union
    {
        aout_symbol_table_t aout_sym;
        elf_section_header_table_t elf_sec;
    } u;
    unsigned long mmap_length;
    unsigned long mmap_addr;
} multiboot_info_t;

/* 检测 FLAGS 中的位 BIT 是否被置位。 */
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

void get_time (void);

#include "KernelFunc.h"

/* Kernel start */

void kstart (unsigned long magic, unsigned long addr)
{
    multiboot_info_t *mbi;

    /* 清屏。 */
    cls();

    /* 将 MBI 指向 Multiboot 信息结构。 */
    mbi = (multiboot_info_t *) addr;

    /* mem_* 是否有效？ */
    if (CHECK_FLAG (mbi->flags, 0))
        kprintf ("mem_lower = %uKB, mem_upper = %uKB, installed = %uMB\n\n", (unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper, (unsigned) (mbi->mem_upper+1024) >> 10);
    
	kprintf ("Kernel cmd line = %s\n", (char*)(unsigned) mbi->cmdline);
	kprintf ("Boot device = %s\n", (char*)(unsigned) mbi->boot_device);

	init_gdtidt();
	kprintf ("GDT and IDT inited\n");

	init_pic();
	io_sti();
	init_pit();

	/* Memory */
	unsigned int memtotal = ((unsigned) mbi->mem_upper + 1024) << 10;
	
	memman_init();
	memman_free(0x00110000, memtotal - 0x00110000);
	kprintf("Memory free: %dKB\n", memman_total() >> 10);

	while(1) {
		io_hlt();
		kprintf("Doom\n");
	}
	//return;
}

void ktimes(void)
{
	kprintf("A");
	return;
}

