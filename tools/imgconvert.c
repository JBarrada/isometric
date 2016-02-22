#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


typedef struct tag_sprite_header {
	uint16_t f_width;
	uint16_t f_height;
	
	uint16_t f_top;
	uint16_t f_left;
	
	uint16_t iso_bottom;
	uint16_t iso_left;
	
	uint8_t tiles_x;
	uint8_t tiles_y;
	uint8_t tiles_z;
	
	uint16_t anim_f_count;
	uint8_t anim_fps;
	
	char bitmap_path[256];
	char sprite_name[256];
	
	uint8_t debug_r;
	uint8_t debug_g;
	uint8_t debug_b;
	
	uint8_t palette_colors[255*3];
	
	uint32_t sprite_data_pointer;
	int sprite_data_size;
	
} sprite_header;

int load_sprite_header(sprite_header *hdr, char *path); 
int get_value(const char *str, char c, char *value);
int load_bmp(sprite_header *hdr, char *path);
int in_palette(uint8_t *color, uint8_t* palette);

int main(int argc, char *argv[]) {
	if (argc == 2) {
		sprite_header hdr;
		memset(&hdr, 0, sizeof(hdr));
		load_sprite_header(&hdr, argv[1]);

		load_bmp(&hdr, hdr.bitmap_path);
		
		char sprite_path[512];
		sprintf(sprite_path, "%s.sprite", hdr.sprite_name);
		FILE* output = fopen(sprite_path, "wb");
		
		fwrite(&hdr.f_width, sizeof(uint16_t), 1, output);
		fwrite(&hdr.f_height, 2, 1, output);
		
		fseek(output, 5, 0);
		fwrite(&hdr.f_top, 2, 1, output);
		fwrite(&hdr.f_left, 2, 1, output);
		
		fseek(output, 10, 0);
		fwrite(&hdr.iso_bottom, 2, 1, output);
		fwrite(&hdr.iso_left, 2, 1, output);
		
		fseek(output, 15, 0);
		fwrite(&hdr.tiles_x, 1, 1, output);
		fwrite(&hdr.tiles_y, 1, 1, output);
		fwrite(&hdr.tiles_z, 1, 1, output);
		
		fseek(output, 19, 0);
		fwrite(&hdr.anim_f_count, 2, 1, output);
		fwrite(&hdr.anim_fps, 1, 1, output);
		
		fseek(output, 22, 0);
		fwrite(&hdr.debug_r, 1, 1, output);
		fwrite(&hdr.debug_g, 1, 1, output);
		fwrite(&hdr.debug_b, 1, 1, output);
		
		fseek(output, 32, 0);
		fwrite(&hdr.palette_colors, 3, 255, output);
		
		fseek(output, 832, 0);
		fwrite(&hdr.sprite_name, 32, 1, output);
		
		fseek(output, 832+32, 0);
		uint8_t* sprite_data = (uint8_t*)hdr.sprite_data_pointer;
		fwrite(sprite_data, 1, hdr.sprite_data_size, output);
		
		fclose(output);
		
		/*
		FILE* outputr = fopen(sprite_path, "r");

		int fpos = 0;
		while (!feof(outputr)) {
			char c = fgetc(outputr);
			printf("%2x ", c & 0xff);
			fpos++;
			if (fpos %16 == 0) {
				printf("\n");
			}
		}
		
		fclose(outputr);
		*/
		
	}
}

int load_bmp(sprite_header *hdr, char *path) {
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
			color[0] = data[offset+2];
			color[1] = data[offset+1];
			color[2] = data[offset+0];
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
	
	uint8_t sprite_data[image_height*image_width];
	int sprite_data_pos = 0;
	for (int row = 0; row < image_height; row++) {
		int offset = ((image_width*3) + row_padding)*row;
		for (int col = 0; col < image_width; col++) {
			uint8_t color[3];
			color[0] = data[offset+2];
			color[1] = data[offset+1];
			color[2] = data[offset+0];
			
			int palette_color = in_palette(color, hdr->palette_colors);
			
			//printf("%d", palette_color);
			sprite_data[sprite_data_pos++] = (uint8_t) palette_color;
			offset += 3;
		}
		//printf(" (%d)\n", row);
	}
	
	hdr->sprite_data_pointer = (uint32_t)&sprite_data;
	hdr->sprite_data_size = image_height*image_width;
	
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

int load_sprite_header(sprite_header *hdr, char *path) {
	FILE* file = fopen(path, "r");
	
	char line[256];
	char value[256];
	
	char values[16][256];
	memset(&values, 0, sizeof(values));
	int values_pos = 0;
	while (fgets(line, sizeof(line), file) != NULL) {
		get_value(line, ':', value);
		memcpy(values[values_pos++], value, strlen(value));
	}
	
	fclose(file);
	
	hdr->f_width = atoi(values[0]);
	hdr->f_height = atoi(values[1]);

	hdr->f_top = atoi(values[2]);
	hdr->f_left = atoi(values[3]);

	hdr->iso_bottom = atoi(values[4]);
	hdr->iso_left = atoi(values[5]);
	
	hdr->tiles_x = atoi(values[6]);
	hdr->tiles_y = atoi(values[7]);
	hdr->tiles_z = atoi(values[8]);

	hdr->anim_f_count = atoi(values[9]);
	hdr->anim_fps = atoi(values[10]);
	
	memcpy(&hdr->bitmap_path[0], values[11], strlen(values[11]));
	memcpy(&hdr->sprite_name[0], values[12], strlen(values[12]));
	
	hdr->debug_r = atoi(values[13]);
	hdr->debug_g = atoi(values[14]);
	hdr->debug_b = atoi(values[15]);
	
	
	return 1;
}

int get_value(const char *str, char c, char *value) {
	int value_pos = 0;
	bool pass_delim = false;
	for (int i = 0; i < strlen(str); i++) {
		if (pass_delim && str[i] != ' ' && str[i] != '\n') {
			value[value_pos++] = str[i];
		}
		
		if (str[i] == c) {
			pass_delim = true;
		}
	}
	value[value_pos++] = 0;
	return 1;
}