import os
import struct


def get_bmp_size(location):
    bmp = open(location, 'r')
    bmp.seek(18, 0)
    w = struct.unpack('<I', bmp.read(4))[0]
    h = struct.unpack('<I', bmp.read(4))[0]
    bmp.close()
    return w, h


for f in os.listdir("bitmaps\\"):
    if f.endswith(".bmp"):
        name = f.replace('.bmp', '')
        if not os.path.isfile('spriteconfigs\\%s.spritecfg' % name):
            # get bmp size
            width, height = get_bmp_size('bitmaps\\%s' % f)
            print '%s (%d, %d)' % (f, width, height)
            z = int(raw_input('z: ?'))

            r, g, b = 128, 128, 128
            if z == 1:
                r, g, b = 0, 255, 0
            if z == 2:
                r, g, b = 0, 0, 255
            if z > 2:
                r, g, b = 32, 32, 32

            spritecfg_data = 'f width: %d\n' \
                             'f height: %d\n' \
                             'f top: 0\n' \
                             'f left: 0\n' \
                             'iso bottom: 21\n' \
                             'iso left: 18\n' \
                             'tiles x: 1\n' \
                             'tiles y: 1\n' \
                             'tiles z: %d\n' \
                             'anim count: 1\n' \
                             'anim fps: 0\n' \
                             'bitmap path: bitmaps\%s\n' \
                             'sprite name: %s\n' \
                             'debug r: %d\n' \
                             'debug g: %d\n' \
                             'debug b: %d\n' % (width, height, z, f, name, r, g, b)

            o = open('spriteconfigs\\%s.spritecfg' % name, 'w')
            o.write(spritecfg_data)
            o.close()

