#include "headers.h"

/* Control polygon for shell segment */
static GLfloat shellControl[4][4][3]  = {
  {{-.02, 0.0, 0.0}, {-.12, 0.0, 1.0}, {-.22, 0.0, 2.0}, {-.32, 0.0, 3.0}},
  {{-0.01, 0.2, 0.0}, {-0.03, 0.3, 1.0}, {-0.06,0.4,2.0}, {-0.12, 0.5, 3.2}},
  {{0.01, 0.2, 0.0}, {0.03, 0.3, 1.0}, {0.06,0.4,2.0}, {0.12, 0.5, 3.2}},
  {{0.02, 0.0, 0.0}, {.12, 0.0, 1.0}, {.22, 0.0, 2.0}, {.32, 0.0, 3.0}}
  };


/* Draws a single segment */
void drawShellSegment() {
  /* set up evaluator for Bezier surface */
  /* First, tell it where to find the control points */
  glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &shellControl[0][0][0]);
  /* Ask for a cubic Bezier surface (degree 3 blending functions, two parameters) */
  glEnable(GL_MAP2_VERTEX_3);
  /* Generate normals automatically at every vertex! */
  glEnable(GL_AUTO_NORMAL);
  /* Use a 16x16 grid of vertices in the u-v parameter space */
  glMapGrid2f(16, 0.0, 1.0, 16, 0.0, 1.0);
  /* Produces a mesh of little quadrialterals approximating the Bezier surface */
  glEvalMesh2(GL_FILL, 0, 16, 0, 16);

}


/* Recursive function to draw segments in half of shell */
void shellSegment(int level, GLfloat shrink) {
  static GLfloat fanAngle = 9.0;
  static GLfloat stepx = 0.1;
  static GLfloat scale = 1.85;

  if (level <= 0) return;
    
  glPushMatrix();
  glRotatef(fanAngle,0.0,1.0,0.0);
  //glTranslatef(stepx,0.0,0.0);
  glPushMatrix();
  glScalef(1.0,1.0,1.0-shrink);
  drawShellSegment();
  glPopMatrix();
  shellSegment(level-1,shrink*scale);
  glPopMatrix();
}
    
/* Draws whole shell */
void shell() {
for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
		for(k = 0; k < 3; k++) {
			shellControl[i][j][k] = shellControl[i][j][k]/2;
//			stoneControl[i][j][2] = stoneControl[i][j][2] + 1.5;
		}
		}
	}
  // draw central segment
  drawShellSegment();
  // draw right half
  shellSegment(7,0.01);
  // draw left half
  glPushMatrix();
  glScalef(-1.0,1.0,1.0);
  shellSegment(7,0.01);
  glPopMatrix();
}
