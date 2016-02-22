from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *

import threading
import math

G_WIDTH = 32
G_HEIGHT = 24

W_WIDTH = 640
W_HEIGHT = 480


def toiso(x, y, z):
    xdelta = math.cos(45 * (math.pi/180.0))
    ydelta = math.cos(45 * (math.pi/180.0))
    zdelta = 0.5

    xiso = x * xdelta + y * ydelta
    yiso = x * xdelta + z * zdelta

    return xiso, yiso


def toiso2(x, y, z):
    xiso = (x-z)/math.sqrt(2)
    yiso = (x+2.0*y+z)/math.sqrt(6)
    return xiso, yiso


def toiso3(x, y, z):
    xiso = (x-y)*math.cos(30 * (math.pi/180.0))
    yiso = z+(x+y)*math.sin(30 * (math.pi/180.0))

    return xiso, yiso


def display():
    glClear(GL_COLOR_BUFFER_BIT)

    glColor3f(1, 1, 1)
    glBegin(GL_POINTS)

    for x in range(10):
        for y in range(10):
            xiso, yiso = toiso3(x, y, 0)
            glVertex2f(xiso, yiso)

    glColor3f(1, 0, 0)
    for x in range(10):
        xiso, yiso = toiso3(x, 0, 0)
        glVertex2f(xiso, yiso)

    glColor3f(0, 0, 1)
    for y in range(10):
        xiso, yiso = toiso3(0, y, 0)
        glVertex2f(xiso, yiso)

    glEnd()

    glFlush()


def init_thread():
    glutInit()
    glutInitWindowSize(W_WIDTH, W_HEIGHT)
    glutCreateWindow("!!!")
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB)
    glutDisplayFunc(display)
    glutIdleFunc(display)

    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluOrtho2D(-(G_WIDTH/2.0), (G_WIDTH/2.0), -(G_HEIGHT/2.0), (G_HEIGHT/2.0))

    glutMainLoop()


def init():
    t = threading.Thread(target=init_thread)
    t.start()

init()
