from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import time

WIDTH, HEIGHT = 400, 400

# Circle Symmetry Function
def circleSymmetry(x, y):
    glVertex2d(200+x, 200+y)
    glVertex2d(200+x, 200-y)
    glVertex2d(200-x, 200+y)
    glVertex2d(200-x, 200-y)
    glVertex2d(200+y, 200+x)
    glVertex2d(200+y, 200-x)
    glVertex2d(200-y, 200+x)
    glVertex2d(200-y, 200-x)

# Algorithm Implementation
def midPointCircleAlgo(r):
    x = 0
    y = r
    d = 5.0/4.0 - r
    circleSymmetry(x, y)
    while y > x:
        if d < 0:
            d += 2.0 * x + 3.0
        else:
            d += 2.0 * (x-y) + 5.0
            y-=1
        x+=1
        circleSymmetry(x, y)

# Display function
def display():
    glClear(GL_COLOR_BUFFER_BIT) # clear with black color
    glColor3f(1.0, 1.0, 1.0)
    glBegin(GL_LINES)
    # horizontal axis
    glVertex2d(0, 200)
    glVertex2d(400, 200)
    # vertical axis
    glVertex2d(200, 0)
    glVertex2d(200, 400)
    # main diagonal
    glVertex2d(0, 0)
    glVertex2d(400, 400)
    # other diagonal
    glVertex2d(0, 400)
    glVertex2d(400, 0)
    glEnd()

    glColor3f(0.0, 1.0, 0.0)
    glBegin(GL_POINTS)
    midPointCircleAlgo(50)
    glEnd()
    glFlush()

def init():
    glutInit()
    glutInitWindowSize(WIDTH, HEIGHT)
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB)
    glutCreateWindow("Midpoint circle algorithm")
    glClearColor(0.0, 0.0, 0.0, 0.0)
    glMatrixMode(GL_PROJECTION)
    gluOrtho2D(0, WIDTH, 0, HEIGHT)

init()
glutDisplayFunc(display)
glutMainLoop()
