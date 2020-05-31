from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *

WIDTH, HEIGHT = 400, 400

def midPointLineAlgo(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1
    d = 2* dy - dx
    incrE = 2*dy
    incrNE = 2 * (dy - dx)
    x = x1
    y = y1
    glVertex2d(x, y)
    while x < x2:
        if d <= 0:
            d += incrE
            x+=1
        else:
            d += incrNE
            x+=1
            y+=1
        glVertex2d(x, y)

def display():
    glClear(GL_COLOR_BUFFER_BIT)
    glBegin(GL_POINTS)
    midPointLineAlgo(100, 0, 200, 100)
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