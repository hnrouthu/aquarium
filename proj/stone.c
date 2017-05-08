#include "headers.h"

GLfloat stoneControl[4][4][3]  = { 
 {{-1.0/9,  -.1/9,   3}, {-0.5/9, -1.0/9,   1/9 + 3}, {0.5/9, -1.5/9,   0/9 + 3}, {1.5/9,  -.7/9,    0 + 3}},
 {{-1.0/9, -.05/9,   3}, {-0.5/9,    0/9, 1.0/9 + 3}, {0.5/9,    0/9, 1.0/9 + 3}, {1.5/9,  -.5/9,    0 + 3}},
 {{-1.0/9,  .05/9,   3}, {-0.5/9,    1/9,   1/9 + 3}, {0.5/9,  1.0/9, 1.0/9 + 3}, {1.5/9,  -.2/9,    0 + 3}},
 {{-1.0/9,   .1/9,   3}, {-0.5/9,  3.0/9,   1/9 + 3}, {0.5/9,  2.5/9,   0/9 + 3}, {1.5/9,  0.0/9,  0.0 + 3}}
};

GLfloat stoneControl2[4][4][3]  = { 
 {{-2.0/9,  -.1/9,   3}, {-0.5/9, -1.0/9,   1/9 + 3}, {0.5/9, -1.5/9,   0/9 + 3}, {1.5/9,  -.7/9,    0 + 3}},
 {{-2.0/9, -.05/9,   3}, {-0.5/9,    0/9, 1.0/9 + 3}, {0.5/9,    0/9, 1.0/9 + 3}, {1.5/9,  -.5/9,    0 + 3}},
 {{-2.0/9,  .05/9,   3}, {-0.5/9,    2/9,   1/9 + 3}, {0.5/9,  2.0/9, 1.0/9 + 3}, {1.5/9,  -.2/9,    0 + 3}},
 {{-2.0/9,   .1/9,   3}, {-0.5/9,  4.0/9,   1/9 + 3}, {0.5/9,  3.5/9,   0/9 + 3}, {1.5/9,  0.0/9,  0.0 + 3}}
};

GLfloat stoneControl3[4][4][3]  = { 
 {{-0.3/9,  -.1/9,   3}, {-0.2/9, -1.0/9,   1/9 + 3}, {0.5/9, -1.5/9,   0/9 + 3}, {1.5/9,  -.7/9,    0 + 3}},
 {{-0.3/9, -.05/9,   3}, {-0.2/9,    0/9, 1.0/9 + 3}, {0.5/9,    0/9, 1.0/9 + 3}, {1.5/9,  -.5/9,    0 + 3}},
 {{-0.3/9,  .05/9,   3}, {-0.2/9,    1/9,   1/9 + 3}, {0.5/9,  0.7/9, 1.0/9 + 3}, {1.5/9,  -.2/9,    0 + 3}},
 {{-0.3/9,   .1/9,   3}, {-0.2/9,  2.0/9,   1/9 + 3}, {0.5/9,  1.5/9,   0/9 + 3}, {1.5/9,  0.0/9,  0.0 + 3}}
};

GLfloat texptsStone[2][2][2] = {{{0.0, 0.0}, {0.0, 1.0}}, 
			{{1.0, 0.0}, {1.0, 1.0}}};
GLUnurbsObj *stoneNurb;

GLfloat x1, z1;

void drawStone(GLfloat x, GLfloat z) {

	if(!posFlag)
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			stoneControl[i][j][0] = stoneControl[i][j][0] + x;
			stoneControl[i][j][1] = stoneControl[i][j][1] - 0.5;
			stoneControl[i][j][2] = stoneControl[i][j][2] + z;

			stoneControl2[i][j][0] = stoneControl2[i][j][0] + 1.5;
			stoneControl2[i][j][2] = stoneControl2[i][j][2] - 1.5;
		
			stoneControl3[i][j][0] = stoneControl3[i][j][0] -1.5;
			stoneControl3[i][j][1] = stoneControl3[i][j][1] - 0.5;
			stoneControl3[i][j][2] = stoneControl3[i][j][2] +1.5;
		}
		posFlag = 1;
	}
	
   	stoneNurb = gluNewNurbsRenderer();
	gluNurbsProperty(stoneNurb, GLU_SAMPLING_TOLERANCE, 25.0);
   	gluNurbsProperty(stoneNurb, GLU_DISPLAY_MODE, GLU_FILL);

   	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glGenTextures (1, &texture);
	free(total_texture);
	free(alpha_texture);
	free(caustic_texture);

	total_texture = (char*) malloc(4 * 256 * 256*sizeof(char));
	alpha_texture = (char*) malloc(256 * 256*sizeof(char));
	caustic_texture = (char*) malloc(3 * 256 * 256*sizeof(char));

  	load_texture ("alpha.jpg", alpha_texture, GL_ALPHA, 256);
      	load_texture ("a.jpg", caustic_texture, GL_RGB, 256);
  	for (i = 0; i < 256 * 256; i++) {
      		total_texture[4 * i] = caustic_texture[3 * i];
      		total_texture[4 * i + 1] = caustic_texture[3 * i + 1];
      		total_texture[4 * i + 2] = caustic_texture[3 * i + 2];
      		total_texture[4 * i + 3] = alpha_texture[i];
    	}

	GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
   	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   	GLfloat mat_shininess[] = { 100.0 };

   	//glClearColor (0.0, 0.0, 0.0, 0.0);
   	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	
	float posLight0[4] = {-1.0f, 1.0f, 1.0f, 0.0f};
	float ambLight0[4] = {1.f, 1.0f, 1.0f, 0.5f};

	glLightfv(GL_LIGHT0, GL_POSITION, posLight0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambLight0);

	float difLight0[4] = {0.5f, 0.5f, 0.5f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difLight0);

   	glEnable(GL_LIGHTING);
   	glEnable(GL_LIGHT0);
   	glEnable(GL_DEPTH_TEST);
   	glEnable(GL_AUTO_NORMAL);
   	glEnable(GL_NORMALIZE);

   	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, texture);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   	glEnable(GL_MAP2_TEXTURE_COORD_2);

   	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &texptsStone[0][0][0]);

  	glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGBA,
		     GL_UNSIGNED_BYTE,  total_texture);

	GLfloat stoneKnots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
   	GLfloat tStoneknots[4] = {0.0, 0.0, 1.0, 1.0};

   	gluBeginSurface(stoneNurb);
	/*gluNurbsSurface(stoneNurb, 
                   4, tFishknots, 4, tFishknots,
                   4, 2, &texptsStone[0][0][0], 
                   2, 2, GL_MAP2_TEXTURE_COORD_2);*/
   	gluNurbsSurface(stoneNurb, 
                   8, stoneKnots, 8, stoneKnots,
                   4 * 3, 3, &stoneControl[0][0][0], 
                   4, 4, GL_MAP2_VERTEX_3);
   	gluEndSurface(stoneNurb);

   	gluBeginSurface(stoneNurb);
   	gluNurbsSurface(stoneNurb, 
                   8, stoneKnots, 8, stoneKnots,
                   4 * 3, 3, &stoneControl2[0][0][0], 
                   4, 4, GL_MAP2_VERTEX_3);
   	gluEndSurface(stoneNurb);

   	gluBeginSurface(stoneNurb);
   	gluNurbsSurface(stoneNurb, 
                   8, stoneKnots, 8, stoneKnots,
                   4 * 3, 3, &stoneControl3[0][0][0], 
                   4, 4, GL_MAP2_VERTEX_3);
   	gluEndSurface(stoneNurb);

   	/*if (showPoints) {
      		glPointSize(5.0);
      		glDisable(GL_LIGHTING);
      		glColor3f(1.0, 1.0, 0.0);
      		glBegin(GL_POINTS);
      		for (i = 0; i < 4; i++) {
        	 	for (j = 0; j < 4; j++) {
        	    		glVertex3f(ctlpoints[i][j][0], 
        	               	ctlpoints[i][j][1], ctlpoints[i][j][2]);
        	 	}
      		}
      		glEnd();
      		glEnable(GL_LIGHTING);
   	}*/
	glDisable(GL_TEXTURE_2D);
}

drawStones() {
}
