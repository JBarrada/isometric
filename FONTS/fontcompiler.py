import os
import struct

font_bitmap = 'bitmaps\\axis_s.bmp'
output_file = 'compiled\\axis_s.osfont'
spacing = 1
space_width = 4

bmp = open(font_bitmap, 'rb')

bmp.seek(10, 0)
data_offset = struct.unpack('<I', bmp.read(4))[0]

bmp.seek(18, 0)
bmp_width = struct.unpack('<I', bmp.read(4))[0]
bmp_height = struct.unpack('<I', bmp.read(4))[0]

row_padding = 0 if ((bmp_width*3) % 4) == 0 else 4-((bmp_width*3) % 4)

bmp.seek(data_offset, 0)
raw = bmp.read(bmp_height*((bmp_width*3)+row_padding))

bmp.close()

indexed = [0]*(bmp_width*bmp_height)

for row in range(bmp_height):
    for col in range(bmp_width):
        offset = (((bmp_width*3)+row_padding)*row)+(col*3)
        indexed[row*bmp_width+col] = 1 if ord(raw[offset]) == 0 else 0

char_mask = [0]*bmp_width

for col in range(bmp_width):
    for row in range(bmp_height):
        offset = row*bmp_width+col
        if indexed[offset] == 1:
            char_mask[col] = 1

char_widths = [0]*32+[space_width]+[0]*(256-33)
current_char = 33

temp_positions = [0]*256
current_width = 0
for col in range(bmp_width):
    if char_mask[col] != 0:
        current_width += 1
    elif current_width != 0:
        char_widths[current_char] = current_width
        temp_positions[current_char] = col-current_width
        current_width = 0
        current_char += 1

compressed = [0]*(bmp_height*sum(char_widths))
compressed_col = 0
for c in range(256):
    if char_widths[c] != 0:
        for col in range(char_widths[c]):
            for row in range(bmp_height):
                indexed_offset = (row*bmp_width)+(temp_positions[c]+col)
                compressed_offset = (row*sum(char_widths))+(compressed_col+col)
                compressed[compressed_offset] = indexed[indexed_offset]
        compressed_col += char_widths[c]

osfont = open(output_file, 'wb')
osfont.write(chr(bmp_height))
osfont.write(chr(spacing))
for c in range(256):
    osfont.write(chr(char_widths[c]))

for b in compressed:
    osfont.write(chr(b))

osfont.close()

for row in range(bmp_height):
    debugstring = ''
    for col in range(sum(char_widths)):
        offset = row*sum(char_widths)+col
        debugstring += '1' if compressed[offset] == 1 else ' '
    print debugstring
