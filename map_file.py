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
                self.sprite_bank += [sprite.Sprite('SPRITES\\%s.sprite' % sprite_name)]

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

    def bank_contains_sprite(self, bank, sprite):
        for s in bank:
            if s.sprite_name == sprite.sprite_name:
                return True
        else:
            return False