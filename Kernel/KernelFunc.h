/**
 * KernelFunc.h
 */

/* Kernel Func.asm */
void pic_intf0(void);

/* GDT IDT */
void load_gdtr(short i, int t);
void load_idtr(short i, int t);

/* KernelFunc.asm */
/* Basic IO */
void io_hlt(void);
	// Port in
char io_in8(int port);
short io_in16(int port);
int io_in32(int port);
	// Port out
void io_out8(int port, int data);
void io_out16(int port, int data);
void io_out32(int port, int data);

/* k8024display.c */
void cls (void);
static void itoa (char *buf, int base, int d);
static void putchar (int c);
void kprintf (const char *format, ...);
/* 与显示相关的设置。 */
#define COLUMNS                 80
#define LINES                   24
#define ATTRIBUTE               7
#define VIDEO                   0xB8000
static int xpos; /* X 坐标。 */
static int ypos; /* Y 坐标。 */
static volatile unsigned char *video; /* 指向显存。 */

/* kpic.c */
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

void init_pic(void);

/* kgdt_idt.c */
#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_LDT			0x0082
#define AR_TSS32		0x0089
#define AR_INTGATE32	0x008e

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

void init_gdtidt (void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);

/* ktime.c */
typedef struct st_time{
	char yy;
	char mm;
	char dd;
	char h;
	char m;
	char s;
} st_time;
struct TIMERCTL {
	unsigned int tick;
};
struct TIMERCTL timerctl;
static st_time time;

void get_time(void);
void init_pit(void);

/* memory.c */
#define MEMMAN_FREES	4000	 // 4000 records of memory

#define MEMMAN_ADDR	0x003c0000
static struct MEMMAN *memman = (struct MEMMAN*) MEMMAN_ADDR;

struct FREEINFO {
	unsigned int addr, size;
};

struct MEMMAN {
	int frees, maxfrees, lostsize, losts;
	struct FREEINFO free[MEMMAN_FREES];
};

void memman_init (void);
unsigned int memman_total (void);
unsigned int memman_alloc(unsigned int size);
int memman_free(unsigned int addr, unsigned int size);
