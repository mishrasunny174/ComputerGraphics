from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *

WIDTH, HEIGHT = 400, 400
XMIN, XMAX, YMIN, YMAX = 150, 300, 100, 300
TOP = 0b1000
BOTTOM = 0b0100
RIGHT = 0b0010
LEFT = 0b0001

def calcOutcode(x0, y0):
    outcode = 0
    if x0 < XMIN:
        outcode |= LEFT
    if x0 > XMAX:
        outcode |= RIGHT
    if y0 < YMIN:
        outcode |= BOTTOM
    if y0 > YMAX:
        outcode |= TOP
    return outcode


def cohenSutherLandLineClip(x0, y0, x1, y1):
    glVertex2d(x0, y0)
    glVertex2d(x1, y1)
    outcode1 = calcOutcode(x0, y0)
    outcode2 = calcOutcode(x1, y1)
    accepted = (bool)(not(outcode1 | outcode2))
    rejected = (bool)(outcode1 & outcode2)
    while (not accepted) or rejected:
        outcode = outcode1
        if outcode2:
            outcode = outcode2
        if outcode & TOP:
            y = YMAX
            x = x0 + (x1 - x0) * (YMAX - y0) / (y1 - y0)
        elif outcode & BOTTOM:
            y = YMIN
            x = x0 + (x1 - x0) * (YMIN - y0) / (y1 - y0)
        elif outcode & RIGHT:
            x = XMAX
            y = y0 + (y1 - y0) * (XMAX - x0) / (x1 - x0)
        else:
            x = XMIN
            y = y0 + (y1 - y0) * (XMIN - x0) / (x1 - x0)
        if outcode == outcode1:
            x0, y0 = x, y
        else:
            x1, y1 = x, y
        outcode1 = calcOutcode(x0, y0)
        outcode2 = calcOutcode(x1, y1)
        accepted = (bool)(not(outcode1 | outcode2))
        rejected = (bool)(outcode1 & outcode2)
    if accepted:
        glColor3f(1.0, 0, 0)
        glVertex2d(x0, y0)
        glVertex2d(x1, y1)



def display():
    glClear(GL_COLOR_BUFFER_BIT)
    glColor3f(1.0, 1.0, 1.0)
    glBegin(GL_QUADS)
    glVertex2d(XMIN, YMIN)
    glVertex2d(XMIN, YMAX)
    glVertex2d(XMAX, YMAX)
    glVertex2d(XMAX, YMIN)
    glEnd()

    glColor3f(0.0, 1.0, 0.0)
    glBegin(GL_LINES)
    cohenSutherLandLineClip(100, 100, 200, 200)
    glEnd()
    glFlush()

def init():
    glutInit()
    glutInitWindowSize(WIDTH, HEIGHT)
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB)
    glutCreateWindow("Midpoint line algorithm")
    glClearColor(0.0, 0.0, 0.0, 0.0)
    glMatrixMode(GL_PROJECTION)
    gluOrtho2D(0, WIDTH, 0, HEIGHT)

init()
glutDisplayFunc(display)
glutMainLoop()