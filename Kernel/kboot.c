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

/* 与显示相关的设置。 */
#define COLUMNS                 80
#define LINES                   24
#define ATTRIBUTE               7
#define VIDEO                   0xB8000

static int xpos; /* X 坐标。 */
static int ypos; /* Y 坐标。 */
static volatile unsigned char *video; /* 指向显存。 */

static void cls (void);
static void itoa (char *buf, int base, int d);
static void putchar (int c);
void kprintf (const char *format, ...);

/* GDT IDT */
struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};

struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};

static void init_gdtidt (void);
static void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
static void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);

static void load_gdtr(short i, int t);
static void load_idtr(short i, int t);

void init_gdtidt(void)
{
	struct SEGMENT_DESCRIPTOR	*gdt = (struct SEGMENT_DESCRIPTOR*)	0x00270000;
	struct GATE_DESCRIPTOR		*idt = (struct GATE_DESCRIPTOR*)	0x0026f800;
	int i;

	/* GDT */
	for (i = 0; i < 8192; i++) {
		set_segmdesc(gdt + i, 0, 0, 0);	
	}
	set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, 0x4092);
	set_segmdesc(gdt + 2, 0x000b8000, 0x00280000, 0x409a);
	load_gdtr(0xffff, 0x00270000);
	
	/* IDT */
	for (i = 0; i < 256; i++) {
		set_gatedesc(idt + i, 0, 0, 0);	
	}
	load_idtr(0x7ff, 0x0026f800);

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

/* Kernel start */

void kstart (unsigned long magic, unsigned long addr)
{
    multiboot_info_t *mbi;

    /* 清屏。 */
    cls ();

    /* 将 MBI 指向 Multiboot 信息结构。 */
    mbi = (multiboot_info_t *) addr;

    /* mem_* 是否有效？ */
    if (CHECK_FLAG (mbi->flags, 0))
        kprintf ("mem_lower = %uKB, mem_upper = %uKB\n", (unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);
    
    kprintf ("Kernel loaded, preparing gdt and idt\n");
	init_gdtidt();
	kprintf ("GDT and IDT inited\n");
}

/* 清屏并初始化 VIDEO，XPOS 和 YPOS。 */
static void cls (void)
{
    int i;

    video = (unsigned char *) VIDEO;

    for (i = 0; i < COLUMNS * LINES * 2; i++)
        *(video + i) = 0;

    xpos = 0;
    ypos = 0;
}

/* 将整数 D 转换为字符串并保存在 BUF 中。如果 BASE 为 'd'，则 D 为十进制，如果 BASE 为 'x'，则 D 为十六进制。 */
static void itoa (char *buf, int base, int d)
{
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;

    /* 如果指定了 %d 并且 D 是负数，在开始添上负号。 */
    if (base == 'd' && d < 0)
    {
        *p++ = '-';
        buf++;
        ud = -d;
    }
    else if (base == 'x')
        divisor = 16;

    /* 用 DIVISOR 去除 UD 直到 UD == 0。 */
    do
    {
        int remainder = ud % divisor;

        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
    while (ud /= divisor);

    /* 在字符串尾添上终结符。 */
    *p = 0;

    /* 反转 BUF。 */
    p1 = buf;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

/* 在屏幕上输出字符 C 。 */
static void putchar (int c)
{
    if (c == '\n' || c == '\r')
    {
newline:
        xpos = 0;
        ypos++;
        if (ypos >= LINES)
            ypos = 0;
        return;
    }

    *(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
    *(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;

    xpos++;
    if (xpos >= COLUMNS)
        goto newline;
}

/* 格式化字符串并在屏幕上输出，就像 libc 函数 printf 一样。 */
void kprintf (const char *format, ...)
{
    char **arg = (char **) &format;
    int c;
    char buf[20];

    arg++;

    while ((c = *format++) != 0)
    {
        if (c != '%')
            putchar (c);
        else
        {
            char *p;

            c = *format++;
            switch (c)
            {
            case 'd':
            case 'u':
            case 'x':
                itoa (buf, c, *((int *) arg++));
                p = buf;
                goto string;
                break;

            case 's':
                p = *arg++;
                if (! p)
                    p = "(null)";

string:
                while (*p)
                    putchar (*p++);
                break;

            default:
                putchar (*((int *) arg++));
                break;
            }
        }
    }
}
