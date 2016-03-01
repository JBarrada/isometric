import sprite
import struct


class Map:
    g_width = 0
    g_height = 0
    sprite_bank = []
    grid = []

    def __init__(self, g_width=0, g_height=0, sprite_bank=None, grid=None):
        self.g_width = g_width
        self.g_height = g_height
        self.sprite_bank = sprite_bank
        self.grid = grid

    def save(self, location):
        f = open(location, 'wb')

        # write width as int16
        width_bytes = struct.pack('<h', self.g_width)
        f.write(width_bytes)

        # write height as int16
        height_bytes = struct.pack('<h', self.g_height)
        f.write(height_bytes)

        # create new list of sprites
        new_bank = []
        new_bank_pointers = [-1]*len(self.sprite_bank)
        for tile in self.grid:
            if tile != -1:
                if not self.bank_contains_sprite(new_bank, self.sprite_bank[tile]):
                    new_bank_pointers[tile] = len(new_bank)
                    new_bank += [self.sprite_bank[tile]]
        for s in self.sprite_bank:
            if s.keep_in_bank:
                new_bank += [s]

        # remap grid to use new bank
        new_grid = []
        for tile in self.grid:
            if tile != -1:
                new_grid += [new_bank_pointers[tile]]
            else:
                new_grid += [-1]

        # write sprite look up table
        for s in new_bank:
            sprite_name_len = len(s.sprite_name)
            sprite_name_bytes = s.sprite_name
            sprite_name_bytes += chr(0)*(32-sprite_name_len)
            f.write(sprite_name_bytes)

        f.write('-'*32)

        # f.seek(16388, 0)
        for tile in new_grid:
            tile_bytes = struct.pack('<h', tile)
            if len(tile_bytes) != 2:
                placeholder = 0
            f.write(tile_bytes)

        f.close()

    def save_zipmap(self, location):
        # create new list of sprites
        new_bank = []
        new_bank_pointers = [-1]*len(self.sprite_bank)
        for tile in self.grid:
            if tile != -1:
                if not self.bank_contains_sprite(new_bank, self.sprite_bank[tile]):
                    new_bank_pointers[tile] = len(new_bank)
                    new_bank += [self.sprite_bank[tile]]
        for s in self.sprite_bank:
            if s.keep_in_bank:
                new_bank += [s]

        # remap grid to use new bank
        new_grid = []
        for tile in self.grid:
            if tile != -1:
                new_grid += [new_bank_pointers[tile]]
            else:
                new_grid += [-1]

        f = open(location, 'wb')

        # write width as int16
        width_bytes = struct.pack('<h', self.g_width)
        f.write(width_bytes)

        # write height as int16
        height_bytes = struct.pack('<h', self.g_height)
        f.write(height_bytes)

        sprite_starts = []
        global_palette = [0]*(255*3) + [0, 63, 0]
        global_pal_pos = 0

        # remap/register sprite palette to global palette
        f.seek(776+(4*len(new_bank))+2)
        for s in new_bank:
            sprite_remap = [0]*256
            for i in range(255):
                test_color = [(ord(s.palette[(i*3)+0])/4), (ord(s.palette[(i*3)+1])/4), (ord(s.palette[(i*3)+2])/4)]
                if test_color == [0, 0, 0]:
                    continue
                global_index = self.palette_index_of_color(global_palette, test_color)
                if global_index == -1:
                    sprite_remap[i] = global_pal_pos
                    global_palette[global_pal_pos*3:(global_pal_pos*3)+3] = test_color
                    global_pal_pos += 1
                else:
                    sprite_remap[i] = global_index

            sprite_starts += [f.tell()]

            # write remapped sprite data
            width_bytes = struct.pack('<h', s.f_width)
            f.write(width_bytes)
            height_bytes = struct.pack('<h', s.f_height)
            f.write(height_bytes)
            iso_bottom_bytes = struct.pack('<h', s.iso_bottom)
            f.write(iso_bottom_bytes)
            iso_left_bytes = struct.pack('<h', s.iso_left)
            f.write(iso_left_bytes)
            f.write(chr(s.tiles_z))
            f.write(chr(s.anim_f_count))

            for c in s.sprite_name:
                f.write(c)
            f.write('\0'*(16-len(s.sprite_name)))

            # write bitmap data
            for b in s.bitmap_bytes:
                f.write(chr(sprite_remap[ord(b)]))

        # write grid start and grid
        f.write('-'*32)
        grid_start = f.tell()

        for tile in new_grid:
            tile_bytes = struct.pack('<h', tile)
            f.write(tile_bytes)

        f.seek(4, 0)
        grid_start_bytes = struct.pack('<I', grid_start)
        f.write(grid_start_bytes)

        # write palette
        f.seek(8, 0)
        for i in range(256*3):
            f.write(chr(global_palette[i]))

        # write start positions
        f.seek(776, 0)
        sprite_count_bytes = struct.pack('<h', len(new_bank))
        f.write(sprite_count_bytes)
        for start in sprite_starts:
            sprite_start_bytes = struct.pack('<I', start)
            f.write(sprite_start_bytes)

        f.close()

    def load(self, location):
        f = open(location, 'rb')

        self.g_width = struct.unpack('<h', f.read(2))[0]
        self.g_height = struct.unpack('<h', f.read(2))[0]

        self.sprite_bank = []
        bank_end = False
        while not bank_end:
            sprite_name = f.read(32).replace('\0', '')
            if sprite_name == '-'*32:
                bank_end = True
                continue
            if sprite_name != '':
                self.sprite_bank += [sprite.Sprite('SPRITES\\compiled\\%s.sprite' % sprite_name)]

        # f.seek(16388, 0)

        self.grid = []
        for tile_index in range(self.g_width*self.g_height):
            data = f.read(2)
            tile_pointer = struct.unpack('<h', data)[0]
            if tile_pointer < len(self.sprite_bank):
                self.grid += [tile_pointer]
            else:
                self.grid += [-1]

        f.close()

    def palette_index_of_color(self, palette, color):
        for i in range(256):
            if palette[i*3:i*3+3] == color:
                return i
        return -1

    def bank_contains_sprite(self, bank, spr):
        for s in bank:
            if s.sprite_name == spr.sprite_name:
                return True
        else:
            return False