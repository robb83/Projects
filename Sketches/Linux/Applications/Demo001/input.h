#define BITS_PER_LONG        (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1) 
#define OFF(x) ((x)%BITS_PER_LONG) 
#define BIT(x) (1UL<<OFF(x)) 
#define LONG(x) ((x)/BITS_PER_LONG) 
#define test_bit(bit, array) ((array[LONG(bit)] >> OFF(bit)) & 1)

#define EVENT_KEY 1
#define EVENT_ABS 2
#define EVENT_REL 4

#ifndef ABS_MT_POSITION_X
#define ABS_MT_POSITION_X 0x35
#endif

#ifndef ABS_MT_POSITION_Y
#define ABS_MT_POSITION_Y 0x36
#endif

typedef struct {
	int index;
	int type;
	int key;
	int key_value;
	int x;
	int y;
	int z;
	int abs_x;
	int abs_y;
	int abs_z;
	int abs_x_min;
	int abs_x_max;
	int abs_y_min;
	int abs_y_max;
	int abs_z_min;
	int abs_z_max;
} event;

int input_init(void (*input_handler)(event*));
int input_quit(void);
int input_poll(void);