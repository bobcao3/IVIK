
/* GDT IDT */
void load_gdtr(short i, int t);
void load_idtr(short i, int t);

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
