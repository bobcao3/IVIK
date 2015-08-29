/**
 * KernelFunc.h
 */

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
void init_pic(void);

/* kgdt_idt.c */
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
static st_time time;

/* memory.c */
#define MEMMAN_FREES	4000	 // 4000 records of memory

struct FREEINFO {
	unsigned int addr, size;
};

struct MEMMAN {
	int frees, maxfrees, lostsize, losts;
	struct FREEINFO free[MEMMAN_FREES];
};

void memman_init (struct MEMMAN *man);
unsigned int memman_total (struct MEMMAN *man);
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size);
int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size);
