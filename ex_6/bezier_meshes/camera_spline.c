/*  camera_spline.c
 *  Animates a virtual camera along a cubic Bezier spline path.
 *  The camera position is evaluated at each frame using Bernstein
 *  polynomials, producing smooth motion along the curve.
 */
#include <stdlib.h>
#include <GL/glut.h>

GLfloat ctrlpoints[4][3] = {
    { 5.0,  1.0, -3.0},
    { 4.0,  5.0,  0.0},
    {-4.0,  5.0,  0.0},
    {-5.0,  1.0,  3.0}
};

GLfloat t = 0.0;
GLfloat step = 0.004;

void evalBezier(GLfloat t, GLfloat p[3])
{
    GLfloat b[4];
    GLfloat u = 1.0 - t;
    b[0] = u * u * u;
    b[1] = 3.0f * u * u * t;
    b[2] = 3.0f * u * t * t;
    b[3] = t * t * t;
    p[0] = b[0]*ctrlpoints[0][0] + b[1]*ctrlpoints[1][0]
         + b[2]*ctrlpoints[2][0] + b[3]*ctrlpoints[3][0];
    p[1] = b[0]*ctrlpoints[0][1] + b[1]*ctrlpoints[1][1]
         + b[2]*ctrlpoints[2][1] + b[3]*ctrlpoints[3][1];
    p[2] = b[0]*ctrlpoints[0][2] + b[1]*ctrlpoints[1][2]
         + b[2]*ctrlpoints[2][2] + b[3]*ctrlpoints[3][2];
}

void display(void)
{
    GLfloat eye[3];
    GLfloat center[3] = {0.0, 0.0, 0.0};
    int i;

    evalBezier(t, eye);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2],
              center[0], center[1], center[2],
              0.0, 1.0, 0.0);

    /* draw coordinate axes */
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0); glVertex3f(0,0,0); glVertex3f(2,0,0);
    glColor3f(0.0, 1.0, 0.0); glVertex3f(0,0,0); glVertex3f(0,2,0);
    glColor3f(0.0, 0.0, 1.0); glVertex3f(0,0,0); glVertex3f(0,0,2);
    glEnd();

    /* draw a teapot at the origin */
    glColor3f(0.8, 0.6, 0.4);
    glutSolidTeapot(0.8);

    /* draw control hull */
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_LINE_STRIP);
    for (i = 0; i < 4; i++)
        glVertex3fv(ctrlpoints[i]);
    glEnd();

    /* draw the Bezier curve path */
    glColor3f(0.3, 0.8, 1.0);
    glBegin(GL_LINE_STRIP);
    for (i = 0; i <= 50; i++)
    {
        GLfloat p[3];
        evalBezier((GLfloat)i / 50.0f, p);
        glVertex3fv(p);
    }
    glEnd();

    /* draw camera position as a small sphere */
    glColor3f(1.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(eye[0], eye[1], eye[2]);
    glutSolidSphere(0.15, 8, 8);
    glPopMatrix();

    glutSwapBuffers();
}

void update(int value)
{
    t += step;
    if (t > 1.0) t = 0.0;
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

void init(void)
{
    glClearColor(0.1, 0.1, 0.15, 0.0);
    glEnable(GL_DEPTH_TEST);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Camera Spline Animation");
    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
