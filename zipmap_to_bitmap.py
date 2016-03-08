from PIL import Image, ImageDraw, ImageOps
import map_file
import sprite

target_isomap = "default_map2.isomap"


def tti(x, y, z):
    y *= -1
    xiso = (x-y)*0.866025403784
    yiso = z+(x+y)*0.5
    return xiso, -yiso


# load map
m = map_file.Map()
m.load(target_isomap)
isize = 20.0

# get offsets
g_bl = tti(0, 0, 0)
g_tl = tti(0, m.g_height*isize, 0)
g_br = tti(m.g_width*isize, 0, 0)
g_tr = tti(m.g_width*isize, m.g_height*isize, 0)

px_ox = 100
px_oy = int(-g_br[1]) + 100

px_width = int(g_tr[0]) + 200
px_height = int(g_tl[1]-g_br[1]) + 200

# setup canvas
image = Image.new('RGBA', (px_width, px_height), color=(255, 255, 255, 255))
grid = Image.new('RGBA', (px_width, px_height), color=(255, 255, 255, 0))
draw = ImageDraw.Draw(image)
grid_draw = ImageDraw.Draw(grid)


def draw_sprite(px, py, s):
    for oy in range(s.f_height):
        for ox in range(s.f_width):
            offset = (oy*s.f_width+ox)*4
            if s.bitmap_data[offset+3] != 0:
                draw.point([px+ox-s.iso_left, py+oy-s.iso_bottom], tuple(s.bitmap_data[offset:offset+4]))

# draw sprites
for y in range(m.g_height-1, -1, -1):
    for x in range(m.g_width):
        ix, iy = tti((x+1)*isize, y*isize, 0)
        if m.grid[y*m.g_width+x] != -1:
            draw_sprite(ix+px_ox, iy+px_oy, m.sprite_bank[m.grid[y*m.g_width+x]])
            if ((y*m.g_width+x)*100)/(m.g_width*m.g_height) % 5 == 0:
                print('%d' % (((y*m.g_width+x)*100)/(m.g_width*m.g_height)))

# draw gridlines
for x in range(m.g_width+1):
    x1, y1 = tti(x*isize, 0, 0)
    x2, y2 = tti(x*isize, m.g_height*isize, 0)
    grid_draw.line([x1+px_ox, y1+px_oy, x2+px_ox, y2+px_oy], fill=(20, 20, 20, 20), width=1)

for y in range(m.g_height+1):
    x1, y1 = tti(0, y*isize, 0)
    x2, y2 = tti(m.g_width*isize, y*isize, 0)
    grid_draw.line([x1+px_ox, y1+px_oy, x2+px_ox, y2+px_oy], fill=(20, 20, 20, 20), width=1)

image = ImageOps.flip(image)
grid = ImageOps.flip(grid)
out = Image.alpha_composite(image, grid)

out.save('map_image.png')
