#include <vga.h>
#include <gfx.h>
#include <io.h>
#include <stdint.h>

int MODE640X480 = 0;

uint8_t row_refresh[HEIGHT_W/8];
uint8_t screen_buffer[WIDTH_W*HEIGHT_W];

uint8_t capture_buf[WIDTH_W*HEIGHT_W];

void swap_buffers() {
	int i, b, row;
	for (i=0; i<(HEIGHT_W/8); i++) {
		for (b=0; b<8; b++) {
			if ((row_refresh[i] >> b) & 1) {
				row_refresh[i] &= ~(1 << b);
				row = (i*8)+b;
				write_pixel_row(row, &screen_buffer[WIDTH_W*row]);
			}
		}
	}
}

void ashes_palette() {
	unsigned int pal[16] = {0x3f4f68, 0xd4c7ab, 0xd6ae76, 0xbe7675, 0x7a5f71, 0x000000, 0x000000, 0x000000, 
							0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0xffffff};
	setup_palette(pal);
}
void ashes_palette_dim() {
	unsigned int pal[16] = {0x2E3A4C, 0xBAAE96, 0xBA9868, 0xA36565, 0x5E4957, 0x000000, 0x000000, 0x000000, 
							0x3f4f68, 0xd4c7ab, 0xd6ae76, 0xbe7675, 0x7a5f71, 0x000000, 0x000000, 0xffffff};
	setup_palette(pal);
}

void gfx_init() {
	MODE640X480 = 1;
	
	memset(row_refresh, 0xff, HEIGHT_W/8);
	memset(screen_buffer, 0, WIDTH_W*HEIGHT_W);
	
	ashes_palette();
	
	swap_buffers();
}


void set_pixel(int x, int y, unsigned char c) {
	if ((x<WIDTH_W) && (y<HEIGHT_W) && (x>=0) && (y>=0)) {
		if (screen_buffer[y*WIDTH_W+x] != c) {
			screen_buffer[y*WIDTH_W+x] = c;
			row_refresh[(y/8)] |= (1 << (y%8));
		}
	}	
}

void capture() {
	memcpy(capture_buf, screen_buffer, WIDTH_W*HEIGHT_W);
}
void restore() {
	memcpy(screen_buffer, capture_buf, WIDTH_W*HEIGHT_W);
}

void line(int x1,int y1,int x2,int y2, unsigned char c) {
	int dx, dy, dxabs, dyabs, x, y, px, py;
	dx = (x2-x1);
	dy = (y2-y1);
	dxabs=(dx<0)?dx*(-1):dx;
	dyabs=(dy<0)?dy*(-1):dy;
	
	if ((dyabs<1000) && (dxabs<1000)) {
		x=dyabs>>1;
		y=dxabs>>1;
		px=x1;
		py=y1;
		
		int i;
		if (dxabs>=dyabs) {
			for(i=0;i<dxabs;i++) {
				y+=dyabs;
				if (y>=dxabs) {
					y-=dxabs;
					py+=(dy>=0)?1:-1;
				}
				px+=(dx>=0)?1:-1;
				set_pixel(px,py,c);
			}
		}
		else {
			for(i=0;i<dyabs;i++) {
				x+=dxabs;
				if (x>=dyabs) {
					x-=dyabs;
					px+=(dx>=0)?1:-1;
				}
				py+=(dy>=0)?1:-1;
				set_pixel(px,py,c);
			}
		}
	}
}

void circle_filled(int cx, int cy, int r, unsigned char c) {
	int i, tx, ty;
	int r2 = r * r;
	int area = r2 << 2;
	int rr = r << 1;

	for (i=0; i<area; i++)
	{
		tx = (i % rr) - r;
		ty = (i / rr) - r;

		if (tx * tx + ty * ty <= r2)
			set_pixel(cx+tx, cy+ty, c);
	}
}

void poly_filled(POINT points[], int num_points, unsigned char c) {
	int poly_top, poly_bot, poly_left, poly_right, swap, p_x, p_y, node_x[num_points], i, j, nodes;
	for (i=0; i<num_points; i++) {
		if (!i) {
			poly_top = poly_bot = points[i].y;
			poly_left = poly_right = points[i].x;
		}
		else {
			if (points[i].y > poly_top)
				poly_top = points[i].y;
			if (points[i].y < poly_bot)
				poly_bot = points[i].y;
			if (points[i].x > poly_right)
				poly_right = points[i].x;
			if (points[i].x < poly_left)
				poly_left = points[i].x;
		}
	}
	
	// loop through rows of the image
	for (p_y=poly_bot; p_y<poly_top; p_y++) {
		// build a list of nodes
		nodes = 0;
		j = num_points - 1;
		for (i=0; i<num_points; i++) {
			if (((points[i].y<(double)p_y) && (points[j].y>=(double)p_y)) || ((points[j].y<(double)p_y) && (points[i].y>=(double)p_y))) {
				node_x[nodes++] = (int) (points[i].x + (p_y-points[i].y) / (points[j].y-points[i].y) * (points[j].x-points[i].x)); 
			}
			j=i; 
		}
		
		// sort the nodes, via a simple “bubble” sort
		i=0;
		while (i<nodes-1) {
			if (node_x[i] > node_x[i+1]) {
				swap = node_x[i]; 
				node_x[i] = node_x[i+1]; 
				node_x[i+1] = swap; 
				if (i) 
					i--; 
			}
			else {
				i++; 
			}
		}
		
		//  Fill the pixels between node pairs.
		for (i=0; i<nodes; i+=2) {
			if (node_x[i] >= poly_right) 
				break;
			
			if (node_x[i+1] > poly_left) {
				if (node_x[i] < poly_left) 
					node_x[i] = poly_left;
				
				if (node_x[i+1] > poly_right) 
					node_x[i+1] = poly_right;
				
				for (p_x=node_x[i]; p_x<node_x[i+1]; p_x++) 
					set_pixel(p_x, p_y, c);
			}
		}
	}
}

void rectangle_filled(int x, int y, int width, int height, unsigned char c) {
	POINT rect[4];
	rect[0] = (POINT) {x, y};
	rect[1] = (POINT) {x+width, y};
	rect[2] = (POINT) {x+width, y+height};
	rect[3] = (POINT) {x, y+height};
	poly_filled(rect, 4, c);
}

void draw_glyph(int x, int y, int width, int height, char *data, int mask_color, int invert) {
	int ix, iy;
	char color, test_px;
	for (iy=0; iy<height; iy++) {
		for (ix=0; ix<width; ix++) {
			test_px = (invert)? data[(iy*width)+ix] ^ 1: data[(iy*width)+ix];
			color = ((mask_color != -1) && test_px)? mask_color : test_px;
			if (test_px) {
				set_pixel(x+ix, y+iy, color);
			}
		}
	}
}