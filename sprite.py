from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *

import struct


class Sprite:
    f_width = 0
    f_height = 0

    f_top = 0
    f_left = 0

    iso_bottom = 0
    iso_left = 0

    tiles_x = 0
    tiles_y = 0
    tiles_z = 0

    anim_f_count = 0
    anim_fps = 0

    bitmap_bytes = []
    bitmap_data = []
    palette = []

    sprite_name = ''

    debug_color = []

    textureid = None

    keep_in_bank = False

    def __init__(self, location):
        f = open(location, 'rb')
        header_bytes = f.read(32)
        self.palette = f.read(3*255)
        f.seek(832, 0)
        self.sprite_name = f.read(32).replace('\0', '')

        f.seek(832+32, 0)
        self.bitmap_bytes = f.read()

        self.f_width = struct.unpack('<h', header_bytes[0:2])[0]
        self.f_height = struct.unpack('<h', header_bytes[2:4])[0]

        self.f_top = struct.unpack('<h', header_bytes[5:7])[0]
        self.f_left = struct.unpack('<h', header_bytes[7:9])[0]

        self.iso_bottom = struct.unpack('<h', header_bytes[10:12])[0]
        self.iso_left = struct.unpack('<h', header_bytes[12:14])[0]

        self.tiles_x = ord(header_bytes[15])
        self.tiles_y = ord(header_bytes[16])
        self.tiles_z = ord(header_bytes[17])

        self.anim_f_count = struct.unpack('<h', header_bytes[19:21])[0]
        self.anim_fps = ord(header_bytes[21])

        self.debug_color = [ord(header_bytes[22])/255.0, ord(header_bytes[23])/255.0, ord(header_bytes[24])/255.0]

        self.bitmap_data = []
        for b in self.bitmap_bytes[:(self.f_width*self.f_height)]:
            p_index = ord(b)*3
            r, g, b = ord(self.palette[p_index]), ord(self.palette[p_index+1]), ord(self.palette[p_index+2])
            if r == 0 and g == 255 and b == 0:
                self.bitmap_data += [r, g, b, 0]
            else:
                self.bitmap_data += [r, g, b, 255]

    def gen_tex(self):
        self.textureid = glGenTextures(1)
        glBindTexture(GL_TEXTURE_2D, self.textureid)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, self.f_width, self.f_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, self.bitmap_data)

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)

    def draw_debug(self, x, y, tsize):
        glBegin(GL_POLYGON)
        r, g, b = self.debug_color
        glColor3f(r, g, b)
        glVertex2f(x, y)
        glVertex2f(x, y+tsize)
        glVertex2f(x+tsize, y+tsize)
        glVertex2f(x+tsize, y)
        glEnd()

    def draw_iso(self, x, y, iso_oz):
        glColor4f(1, 1, 1, 1)
        glEnable(GL_TEXTURE_2D)

        glBindTexture(GL_TEXTURE_2D, self.textureid)
        glBegin(GL_QUADS)

        ix, iy = x-(self.iso_left*iso_oz), y-(self.iso_bottom*iso_oz)

        glTexCoord2i(0, 0)
        glVertex2f(ix, iy)

        glTexCoord2i(1, 0)
        glVertex2f(ix+self.f_width*iso_oz, iy)

        glTexCoord2i(1, 1)
        glVertex2f(ix+self.f_width*iso_oz, iy+self.f_height*iso_oz)

        glTexCoord2i(0, 1)
        glVertex2f(ix, iy+self.f_height*iso_oz)

        glEnd()

        glDisable(GL_TEXTURE_2D)

