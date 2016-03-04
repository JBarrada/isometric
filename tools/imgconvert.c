#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct tag_image_header {
	uint16_t f_width;
	uint16_t f_height;
	
	uint8_t palette_colors[255*3];
	
	uint8_t osimg_data[1000*1000];
} image_header;

int load_bmp(image_header *hdr, char *path);
int in_palette(uint8_t *color, uint8_t* palette);

int main(int argc, char *argv[]) {
	if (argc == 2) {
		image_header hdr;
		memset(&hdr, 0, sizeof(hdr));
		load_bmp(&hdr, argv[1]);
		
		char osimg_path[512];
		memset(osimg_path, 0, 512);
		memcpy(osimg_path, argv[1], strlen(argv[1])-4);
		memcpy(osimg_path+strlen(osimg_path), ".osimg", 6);
		FILE* output = fopen(osimg_path, "wb");
		
		fwrite(&hdr.f_width, sizeof(uint16_t), 1, output);
		fwrite(&hdr.f_height, 2, 1, output);

		fseek(output, 4, 0);
		fwrite(&hdr.palette_colors, 3, 255, output);
		
		fseek(output, 832+4, 0);
		//uint8_t* img_data = (uint8_t*)hdr.img_data_pointer;
		fwrite(&hdr.osimg_data, 1, hdr.f_width*hdr.f_height, output);
		
		fclose(output);
	}
}

int load_bmp(image_header *hdr, char *path) {
	FILE* file = fopen(path, "rb");
	
	int data_offset, image_width, image_height;
	
	fseek(file, 10, 0);
	fread(&data_offset, 4, 1, file);
	
	fseek(file, 18, 0);
	fread(&image_width, 4, 1, file);
	fread(&image_height, 4, 1, file);
	
	int row_padding = (((image_width*3) % 4) == 0)? 0 : 4 - ((image_width*3) % 4);
	
	char data[image_height*((image_width*3) + row_padding)];
	fseek(file, data_offset, 0);
	fread(&data, sizeof(data), 1, file);
	
	fclose(file);
	
	int palette_pos = 0;
	for (int row = 0; row < image_height; row++) {
		int offset = ((image_width*3) + row_padding)*row;
		for (int col = 0; col < image_width; col++) {
			uint8_t color[3];
			color[0] = (uint8_t)data[offset+2]/4;
			color[1] = (uint8_t)data[offset+1]/4;
			color[2] = (uint8_t)data[offset+0]/4;
			if (in_palette(color, hdr->palette_colors) == -1) {
				memcpy(&hdr->palette_colors[palette_pos*3], color, 3);
				printf("%d|%d|%d\n", color[0], color[1], color[2]);
				palette_pos++;
			}
			offset += 3;
		}
	}
	
	if (palette_pos > 50) {
		printf("--------palette excessive, check image--------\n");
		getchar();
	}
	
	//uint8_t sprite_data[image_height*image_width];
	int sprite_data_pos = 0;
	for (int row = 0; row < image_height; row++) {
		int offset = ((image_width*3) + row_padding)*row;
		for (int col = 0; col < image_width; col++) {
			uint8_t color[3];
			color[0] = (uint8_t)data[offset+2]/4;
			color[1] = (uint8_t)data[offset+1]/4;
			color[2] = (uint8_t)data[offset+0]/4;
			
			int palette_color = in_palette(color, hdr->palette_colors);
			
			//printf("%d", palette_color);
			hdr->osimg_data[sprite_data_pos++] = (uint8_t)palette_color;
			offset += 3;
		}
		//printf(" (%d)\n", row);
	}
	
	hdr->f_width = image_width;
	hdr->f_height = image_height;
	//hdr->img_data_pointer = (uint32_t)&sprite_data;
	//hdr->img_data_size = image_height*image_width;
	
	return 1;
}

int in_palette(uint8_t *color, uint8_t* palette) {
	for (int i = 0; i < 255; i++) {
		if (palette[i*3] == color[0] && palette[i*3+1] == color[1] && palette[i*3+2] == color[2]) {
			return i;
		}
	}
	return -1;
}