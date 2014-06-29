
typedef struct {
	int width;
	int height;
	int bitsPerPixel;
} display;

typedef struct {
	int x;
	int y;
	int w;
	int h;
} rect; 

typedef struct {
	int x;
	int y;
} point;

typedef struct {
	char a;
	char r;
	char g;
	char b;
} color;

typedef struct {
	char* name;
	int columns;
	int cell_width;
	int cell_height;
	int offset_left;
	int offset_height;
	int image_width;
} fontdata;

typedef struct {
	int width;
	int height;
	int *pixels;
} Surface;

int gfx_init(void);
int gfx_clear(void);
int gfx_draw(rect* rectangle, int color);
int gfx_blit(int x, int y, Surface *surface);
int gfx_quit(void);
void gfx_display(display *disp);

// text
int gfx_text_draw(int x, int y, char* text);
int gfx_text_init(fontdata *f);