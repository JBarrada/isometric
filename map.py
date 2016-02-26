from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *

import threading
import math
import time
import os

import sprite
import map_file


W1_W = 480
W1_H = 480

W2_W = 320
W2_H = 200

window_top = 0
window_iso = 0

top_ox = 0
top_oy = 0
top_oz = 1
tsize = 10

iso_ox = 0
iso_oy = 0
iso_oz = 1
isize = 20

view_grab = False
view_grab_pos = [-1, -1]
view_oo = [-1, -1]

place_grab = False

grid_width = 10
grid_height = 10
grid = [-1]*(grid_width*grid_height)

sprite_bank = []
sprites_loaded = False

s_tile_pos = [-1, -1]
s_tile = 0
s_tile_prev = 0


def display_tile_selector():
    tile_overlap = 5

    front_offset = 0
    for i in range(s_tile):
        front_offset += sprite_bank[i].f_width + tile_overlap

    current_offset = -front_offset + (W1_W/2.0)
    for i in range(len(sprite_bank)):
        if i == s_tile:
            glBegin(GL_QUADS)
            glColor3f(1, 1, 1)
            x, y = (W1_W/2.0)-sprite_bank[i].iso_left-1, 20-sprite_bank[i].iso_bottom-1
            glVertex2f(x, y)
            glVertex2f(x, y+sprite_bank[i].f_height+2)
            glVertex2f(x+sprite_bank[i].f_width+2, y+sprite_bank[i].f_height+2)
            glVertex2f(x+sprite_bank[i].f_width+2, y)
            glEnd()

        if 0 < current_offset < W1_W:
            sprite_bank[i].draw_iso(current_offset, 20, 1)
        current_offset += sprite_bank[i].f_width + tile_overlap


def display_top():
    glClear(GL_COLOR_BUFFER_BIT)

    if not sprites_loaded:
        return

    # draw grid
    for y in range(grid_height-1, -1, -1):
        for x in range(grid_width):
            tx, ty = (x*tsize)+top_ox, (y*tsize)+top_oy

            if x == s_tile_pos[0] and y == s_tile_pos[1]:
                if s_tile != -1:
                    sprite_bank[s_tile].draw_debug(tx, ty, tsize)
                else:
                    glColor3f(1, 1, 1)
                    glBegin(GL_LINES)
                    glVertex2d(tx, ty)
                    glVertex2d(tx+tsize, ty+tsize)
                    glVertex2d(tx, ty+tsize)
                    glVertex2d(tx+tsize, ty)
                    glEnd()

            elif grid[y*grid_width+x] != -1:
                if (-tsize < tx < W1_W+tsize) and (-tsize < ty < W1_H+tsize):
                    sprite_bank[grid[y*grid_width+x]].draw_debug(tx, ty, tsize)

    # draw gridlines
    glBegin(GL_LINES)
    glColor3f(1, 0, 0)
    for x in range(grid_width+1):
        gx, gy = x*tsize+top_ox, top_oy
        if 0 < gx < W1_W:
            glVertex2f(gx, gy)
            glVertex2f(gx, grid_height*tsize+gy)

    for y in range(grid_height+1):
        gx, gy = top_ox, y*tsize+top_oy
        if 0 < gy < W1_H:
            glVertex2f(gx, gy)
            glVertex2f(grid_width*tsize+gx, gy)
    glEnd()

    if s_tile != -1:
        display_tile_selector()

    glFlush()


def display_iso():
    global iso_ox, iso_oy, iso_oz
    glClear(GL_COLOR_BUFFER_BIT)

    if not sprites_loaded:
        return

    # set view
    if s_tile_pos[0] != -1 and s_tile_pos[1] != -1:
        vx, vy = tti((s_tile_pos[0]+1)*isize, s_tile_pos[1]*isize, 0)
        iso_ox, iso_oy = (W2_W/2)-vx, (W2_H/2)-vy

    # draw grid
    for y in range(grid_height-1, -1, -1):
        for x in range(grid_width):
            ix, iy = tti((x+1)*isize, y*isize, 0)
            if x == s_tile_pos[0] and y == s_tile_pos[1]:
                if s_tile != -1:
                    sprite_bank[s_tile].draw_iso(ix+iso_ox, iy+iso_oy, iso_oz)
            elif grid[y*grid_width+x] != -1:
                if (-(iso_oz*36.0) < ix+iso_ox < W2_W+(iso_oz*36.0)) and (-(iso_oz*136.0) < iy+iso_oy < W2_H+(iso_oz*136.0)):
                    sprite_bank[grid[y*grid_width+x]].draw_iso(ix+iso_ox, iy+iso_oy, iso_oz)

    # draw gridlines
    glBegin(GL_LINES)
    glColor4f(1, 0, 0, 0.2)
    for x in range(grid_width+1):
        if x == 0 or x == grid_width:
            x1, y1 = tti(x*isize, 0, 0)
            x2, y2 = tti(x*isize, grid_height*isize, 0)
            glVertex2f(x1+iso_ox, y1+iso_oy)
            glVertex2f(x2+iso_ox, y2+iso_oy)

    for y in range(grid_height+1):
        if y == 0 or y == grid_height:
            x1, y1 = tti(0, y*isize, 0)
            x2, y2 = tti(grid_width*isize, y*isize, 0)
            glVertex2f(x1+iso_ox, y1+iso_oy)
            glVertex2f(x2+iso_ox, y2+iso_oy)
    glEnd()

    glFlush()


def tti(x, y, z):
    y *= -1
    xiso = (x-y)*0.866025403784  # math.cos(30 * (math.pi/180.0))
    yiso = z+(x+y)*0.5  # math.sin(30 * (math.pi/180.0))
    return xiso, -yiso


def motion(x, y):
    global top_ox, top_oy, s_tile_pos, grid

    s_tile_pos = int((x - top_ox) / tsize), int(((W1_H - y) - top_oy) / tsize)
    if not(0 <= s_tile_pos[0] < grid_width) or not(0 <= s_tile_pos[1] < grid_height) or view_grab:
        s_tile_pos = -1, -1

    if view_grab:
        top_ox = view_oo[0]+(x-view_grab_pos[0])
        top_oy = view_oo[1]+((W1_H-y)-view_grab_pos[1])

    if place_grab:
        grid[s_tile_pos[1]*grid_width+s_tile_pos[0]] = s_tile


def mouse(button, state, x, y):
    global view_grab, view_grab_pos, view_oo, grid, place_grab, s_tile
    if button == GLUT_MIDDLE_BUTTON:
        if state == GLUT_DOWN:
            view_grab = True
            view_grab_pos = x, W1_H-y
            view_oo = top_ox, top_oy
        else:
            view_grab = False

    if button == GLUT_LEFT_BUTTON:
        if state == GLUT_DOWN:
            place_grab = True
            grid[s_tile_pos[1]*grid_width+s_tile_pos[0]] = s_tile
        else:
            place_grab = False

    if button == GLUT_RIGHT_BUTTON:
        if state == GLUT_DOWN:
            tile = grid[s_tile_pos[1]*grid_width+s_tile_pos[0]]
            if tile != -1:
                s_tile = tile


def mouse_wheel(wheel, direction, x, y):
    global top_ox, top_oy, top_oz, tsize, iso_oz, isize

    p_scale = top_oz
    top_oz += direction/5.0
    top_oz = max(top_oz, 0.5)
    i_scale = top_oz/p_scale

    top_ox -= ((x-top_ox)*i_scale)-(x-top_ox)
    top_oy -= (((W1_H-y)-top_oy)*i_scale)-((W1_H-y)-top_oy)
    tsize = top_oz*10

    iso_oz = top_oz
    isize = iso_oz*20


def keyboard(key, x, y):
    global s_tile, s_tile_prev, place_grab, grid_width, grid_height, sprite_bank, grid

    if key == 'a' or key == 'd':
        if key == 'a':
            s_tile -= 1
        if key == 'd':
            s_tile += 1
        s_tile = max(min(len(sprite_bank)-1, s_tile), 0)
        return

    if key == 's':
        m = map_file.Map(grid_width, grid_height, sprite_bank, grid)
        m.save('test.isomap')
        m.save_zipmap('test.zipmap')
        return

    if key == 'l':
        m = map_file.Map()
        m.load('default_map1.isomap')
        load_map(m)
        return

    if key == '`':
        s_tile_prev = s_tile
        s_tile = -1
        return

    new_width = grid_width
    new_height = grid_height
    row_offset = 0
    new_grid = []
    if key == 101:
        new_height += 1
    if key == 103:
        new_height -= 1
    if key == 100:
        new_width -= 1
    if key == 102:
        new_width += 1

    if key == 104:
        row_offset -= 1
    if key == 105:
        row_offset += 1

    for row in range(new_height):
        if 0 <= (row+row_offset) < grid_height:
            for col in range(new_width):
                if col < grid_width:
                    new_grid += [grid[(row+row_offset)*grid_width+col]]
                else:
                    new_grid += [-1]
        else:
            new_grid += [-1]*new_width
    grid = new_grid
    grid_width = new_width
    grid_height = new_height


def load_map(m):
    global grid_width, grid_height, sprite_bank, grid
    grid_width = m.g_width
    grid_height = m.g_height

    sprite_bank_pointers = []
    for ms in m.sprite_bank:
        for i in range(len(sprite_bank)):
            if sprite_bank[i].sprite_name == ms.sprite_name:
                sprite_bank_pointers += [i]
                continue

    grid = []
    for tile in m.grid:
        if tile != -1:
            grid += [sprite_bank_pointers[tile]]
        else:
            grid += [-1]


def keyboard_up(key, x, y):
    global s_tile, s_tile_prev
    if key == '`':
        s_tile = s_tile_prev


def idle():
    global window_top, window_iso

    if not sprites_loaded:
        sprite_bank_gen_tex()

    glutSetWindow(window_top)
    glutPostRedisplay()
    glutSetWindow(window_iso)
    glutPostRedisplay()

    time.sleep(1.0/60.0)


def load_sprites():
    global sprite_bank
    for f in os.listdir("SPRITES\\"):
        if f.endswith(".sprite"):
            sprite_bank += [sprite.Sprite('SPRITES\\%s' % f)]


def sprite_bank_gen_tex():
    global sprite_bank, sprites_loaded

    glutSetWindow(window_top)
    for i in range(len(sprite_bank)):
        sprite_bank[i].gen_tex()
        if int((i*100)/float(len(sprite_bank))) % 10 == 0:
            print '%d%%' % ((i*50.0)/float(len(sprite_bank)))

    glutSetWindow(window_iso)
    for i in range(len(sprite_bank)):
        sprite_bank[i].gen_tex()
        if int((i*100)/float(len(sprite_bank))) % 10 == 0:
            print '%d%%' % (((i*50.0)/float(len(sprite_bank)))+50.0)

    print '100%'
    sprites_loaded = True


def init_thread():
    global window_top, window_iso
    glutInit()

    load_sprites()

    # top window
    glutInitWindowSize(W1_W, W1_H)
    window_top = glutCreateWindow("MAP2D")
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB)
    glutDisplayFunc(display_top)
    glutIdleFunc(idle)

    glutPassiveMotionFunc(motion)
    glutMotionFunc(motion)
    glutMouseFunc(mouse)
    glutMouseWheelFunc(mouse_wheel)
    glutKeyboardFunc(keyboard)
    glutKeyboardUpFunc(keyboard_up)
    glutSpecialFunc(keyboard)

    glClearColor(0.0, 0.0, 0.0, 0.0)
    glColor3f(0.0, 0.0, 0.0)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluOrtho2D(0, W1_W, 0, W1_H)

    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)

    # iso window
    glutInitWindowSize(W2_W*2, W2_H*2)
    window_iso = glutCreateWindow("MAPISO")
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB)
    glutDisplayFunc(display_iso)

    glClearColor(0.0, 0.0, 0.0, 0.0)
    glColor3f(0.0, 0.0, 0.0)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluOrtho2D(0, W2_W, 0, W2_H)

    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)

    glutMainLoop()


def init():
    t = threading.Thread(target=init_thread)
    t.start()

init()
