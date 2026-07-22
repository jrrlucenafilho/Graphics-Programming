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

/*  bezcurve.c
 *  This program uses evaluators to draw a Bezier curve.
 */
#include <GL/glut.h>
#include <stdlib.h>

// NOTE: 1st change: properly managing control points for the desired image
GLfloat ctrlpoints_center[4][3] = {{0.5, 0.0, 0.0},  /* Bottom left */
                                   {1.7, 1.5, 0.0},  /* Upper right */
                                   {0.0, 1.5, 0.0},  /* Upper left */
                                   {1.2, 0.0, 0.0}}; /* Bottom right */

GLfloat ctrlpoints_left[4][3] = {
    {0.5, 0.0, 0.0}, {-0.7, 0.9, 0.0}, {1.0, 0.9, 0.0}, {-0.2, 0.0, 0.0}};

GLfloat ctrlpoints_right[4][3] = {
    {1.2, 0.0, 0.0}, {2.4, 0.9, 0.0}, {0.7, 0.9, 0.0}, {1.9, 0.0, 0.0}};

void init(void) {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);

  // This makes the evaluator which maps the Bezier's control points
  glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints_center[0][0]);
  glEnable(GL_MAP1_VERTEX_3);
}

void display(void) {
  int i;

  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0, 1.0, 1.0);
  glBegin(GL_LINE_STRIP);
  for (i = 0; i <= 30; i++)
    glEvalCoord1f((GLfloat)i / 30.0);
  glEnd();

  glColor3f(1.0, 1.0, 1.0);
  glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints_left[0][0]);
  glBegin(GL_LINE_STRIP);
  for (i = 0; i <= 30; i++)
    glEvalCoord1f((GLfloat)i / 30.0);
  glEnd();

  glColor3f(0.976, 0.0, 0.976);
  glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints_right[0][0]);
  glBegin(GL_LINE_STRIP);

  // NOTE: 2nd change, increase sample size by inceasing max value for 'i' and
  // fraction denominator
  for (i = 0; i <= 60; i++)
    glEvalCoord1f((GLfloat)i / 60.0);
  glEnd();

  glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints_center[0][0]);

  glFlush();
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  GLfloat min_x = -0.7, max_x = 2.4;
  GLfloat min_y = 0.0, max_y = 1.5;
  GLfloat cx = (min_x + max_x) / 2.0;
  GLfloat cy = (min_y + max_y) / 2.0;
  GLfloat range = (max_x - min_x > max_y - min_y) ? (max_x - min_x) / 2.0
                                                  : (max_y - min_y) / 2.0;
  range *= 1.2;

  if (w <= h) {
    GLfloat aspect = (GLfloat)h / (GLfloat)w;
    glOrtho(cx - range, cx + range, cy - range * aspect, cy + range * aspect,
            -5.0, 5.0);
  } else {
    GLfloat aspect = (GLfloat)w / (GLfloat)h;
    glOrtho(cx - range * aspect, cx + range * aspect, cy - range, cy + range,
            -5.0, 5.0);
  }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27:
    exit(0);
    break;
  }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
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
