from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *

def lines():
    glClear(GL_COLOR_BUFFER_BIT)
    glColor3f(0.0, 0.4, 0.2)
    glBegin(GL_LINES)
    glVertex2i(150, 150)
    glVertex2i(350, 100)
    glEnd()
    glFlush()
    

glutInit()
glutInitWindowPosition(50, 400)
glutInitWindowSize(400, 400)
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB)
glutCreateWindow("Lines Example")
glutDisplayFunc(lines)
glClearColor (1.0, 1.0, 1.0, 0.0)
glMatrixMode(GL_PROJECTION)
gluOrtho2D(0.0, 400, 0.0, 400) # set projection mode and frame area
glutIdleFunc(lines)
glutMainLoop()