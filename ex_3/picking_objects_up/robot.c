/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * US Government Users Restricted Rights
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 * robot.c
 * This program shows how to composite modeling transformations
 * to draw translated and rotated hierarchical models.
 * Interaction:  pressing the s and e keys (shoulder and elbow)
 * alters the rotation of the robot arm.
 */
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

static int shoulder = 55;
static int elbow = -30;
static int base = 0;
static int forearm = 0;
static int wrist = 0;
static int pincer = 0;

void init(void) {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Outer: entire robot arm
  glPushMatrix();
  glTranslatef(-2.0, -1.0, 0.0);

  // Base platform
  glPushMatrix();
  glScalef(2.1, 0.3, 2.0);
  glColor3f(0.3f, 0.3f, 0.3f);
  glutSolidCube(1.0);
  glColor3f(1.0, 1.0, 1.0);
  glutWireCube(1.0);
  glPopMatrix();

  // Move to shoulder joint (where arm meets the base)
  glTranslatef(-0.4, -0.2, 0.0);

  // Base rotation around Y to rotate around shoulder joint
  glRotatef((GLfloat)base, 0.0, 1.0, 0.0);

  // Shoulder (pivot at origin, which is the shoulder joint)
  glPushMatrix();
  glRotatef((GLfloat)shoulder, 0.0, 0.0, 1.0);
  glTranslatef(1.0, 0.0, 0.0);
  glPushMatrix();
  glScalef(2.0, 0.4, 0.4);
  glColor3f(0.5f, 0.5f, 0.5f);
  glutSolidCube(1.0);
  glColor3f(1.0, 1.0, 1.0);
  glutWireCube(1.0);
  glPopMatrix();

  // Elbow (pivot at right edge of shoulder)
  glTranslatef(1.0, 0.0, 0.0);
  glRotatef((GLfloat)elbow, 0.0, 0.0, 1.0);
  glTranslatef(1.0, 0.0, 0.0);

  // Forearm self-rotation (spins in place around its own center)
  glRotatef((GLfloat)forearm, 1.0, 0.0, 0.0);
  glPushMatrix();
  glScalef(2.0, 0.4, 0.4);
  glColor3f(0.5f, 0.5f, 0.5f);
  glutSolidCube(1.0);
  glColor3f(1.0, 1.0, 1.0);
  glutWireCube(1.0);
  glPopMatrix();

  // Move to wrist joint, then rotate, then offset hand
  glTranslatef(1.0, 0.0, 0.0);              // to end of forearm (wrist joint)
  glRotatef((GLfloat)wrist, 0.0, 0.0, 1.0); // wrist rotation at joint
  glTranslatef(0.5, 0.0, 0.0);              // offset to center hand
  float r = 0.5, d = 0.1;
  int i;

  glColor3f(0.5f, 0.5f, 0.5f);

  // Front face (filled semicircle)
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0.0, 0.0, d);
  for (i = 0; i <= 20; i++) {
    float a = i * M_PI / 20.0;
    glVertex3f(-sin(a) * r, cos(a) * r, d);
  }
  glEnd();

  // Back face (filled semicircle)
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0.0, 0.0, -d);
  for (i = 0; i <= 20; i++) {
    float a = i * M_PI / 20.0;
    glVertex3f(-sin(a) * r, cos(a) * r, -d);
  }
  glEnd();

  // Curved side wall
  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= 20; i++) {
    float a = i * M_PI / 20.0;
    float x = -sin(a) * r, y = cos(a) * r;
    glVertex3f(x, y, d);
    glVertex3f(x, y, -d);
  }
  glEnd();

  // Flat side wall
  glBegin(GL_QUADS);
  glVertex3f(0.0, r, d);
  glVertex3f(0.0, -r, d);
  glVertex3f(0.0, -r, -d);
  glVertex3f(0.0, r, -d);
  glEnd();

  // White wireframe edges on hand
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINE_STRIP);
  for (i = 0; i <= 20; i++) {
    float a = i * M_PI / 20.0;
    glVertex3f(-sin(a) * r, cos(a) * r, d);
  }
  glEnd();
  glBegin(GL_LINE_STRIP);
  for (i = 0; i <= 20; i++) {
    float a = i * M_PI / 20.0;
    glVertex3f(-sin(a) * r, cos(a) * r, -d);
  }
  glEnd();
  glBegin(GL_LINES);
  glVertex3f(0.0, r, d);
  glVertex3f(0.0, -r, d);
  glVertex3f(0.0, r, -d);
  glVertex3f(0.0, -r, -d);
  glVertex3f(0.0, r, d);
  glVertex3f(0.0, r, -d);
  glVertex3f(0.0, -r, d);
  glVertex3f(0.0, -r, -d);
  for (i = 0; i <= 20; i += 5) {
    float a = i * M_PI / 20.0;
    glVertex3f(-sin(a) * r, cos(a) * r, d);
    glVertex3f(-sin(a) * r, cos(a) * r, -d);
  }
  glEnd();

  // Upper pincer finger
  glPushMatrix();
  glTranslatef(0.0, 0.4f, 0.0);
  glRotatef(-(GLfloat)pincer, 0.0, 0.0, 1.0);
  glPushMatrix();
  glRotatef(20.0, 0.0, 0.0, 1.0);
  glTranslatef(0.3f, 0.0, 0.0);
  glScalef(0.6f, 0.15f, 0.15f);
  glColor3f(0.35f, 0.35f, 0.35f);
  glutSolidCube(1.0);
  glColor3f(1.0, 1.0, 1.0);
  glutWireCube(1.0);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0.564f, 0.205f, 0.0);
  glRotatef(-30.0, 0.0, 0.0, 1.0);
  glTranslatef(0.25f, 0.0, 0.0);
  glScalef(0.5f, 0.15f, 0.15f);
  glColor3f(0.35f, 0.35f, 0.35f);
  glutSolidCube(1.0);
  glColor3f(1.0, 1.0, 1.0);
  glutWireCube(1.0);
  glPopMatrix();
  glPopMatrix();

  // Lower pincer finger
  glPushMatrix();
  glTranslatef(0.0, -0.4f, 0.0);
  glRotatef((GLfloat)pincer, 0.0, 0.0, 1.0);
  glPushMatrix();
  glRotatef(-20.0, 0.0, 0.0, 1.0);
  glTranslatef(0.3f, 0.0, 0.0);
  glScalef(0.6f, 0.15f, 0.15f);
  glColor3f(0.35f, 0.35f, 0.35f);
  glutSolidCube(1.0);
  glColor3f(1.0, 1.0, 1.0);
  glutWireCube(1.0);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0.564f, -0.205f, 0.0);
  glRotatef(30.0, 0.0, 0.0, 1.0);
  glTranslatef(0.25f, 0.0, 0.0);
  glScalef(0.5f, 0.15f, 0.15f);
  glColor3f(0.35f, 0.35f, 0.35f);
  glutSolidCube(1.0);
  glColor3f(1.0, 1.0, 1.0);
  glutWireCube(1.0);
  glPopMatrix();
  glPopMatrix();

  glPopMatrix(); // end shoulder/elbow
  glPopMatrix(); // end base/outer
  glutSwapBuffers();
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Change perspective
  // glTranslatef(0.0, 0.0, -8.0); // swapped out to change orientation
  gluLookAt(0.0, 3.0, 8.0, -1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  // Base movement
  case 'b':
    base = (base + 5) % 360;
    glutPostRedisplay();
    break;
  case 'B':
    base = (base - 5) % 360;
    glutPostRedisplay();
    break;
  // Shoulder movement
  case 's':
    shoulder = (shoulder + 5) % 360;
    glutPostRedisplay();
    break;
  case 'S':
    shoulder = (shoulder - 5) % 360;
    glutPostRedisplay();
    break;
  // Elbow movement
  case 'e':
    elbow = (elbow + 5) % 360;
    glutPostRedisplay();
    break;
  case 'E':
    elbow = (elbow - 5) % 360;
    glutPostRedisplay();
    break;
  // Forearm self-rotation
  case 'r':
    forearm = (forearm + 5) % 360;
    glutPostRedisplay();
    break;
  case 'R':
    forearm = (forearm - 5) % 360;
    glutPostRedisplay();
    break;
  // Wrist rotation
  case 'w':
    wrist = (wrist + 5) % 360;
    glutPostRedisplay();
    break;
  case 'W':
    wrist = (wrist - 5) % 360;
    glutPostRedisplay();
    break;
  // Pincer close / open
  case 'p':
    if (pincer < 20)
      pincer += 5;
    glutPostRedisplay();
    break;
  case 'P':
    if (pincer > -70)
      pincer -= 5;
    glutPostRedisplay();
    break;
  case 27:
    break;
  }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
