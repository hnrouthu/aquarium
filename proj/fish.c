#include "headers.h"

GLfloat fishControl[4][4][3]  = { 
 {{-5.0/9,  -.1/9,   0}, {-2.5/9, -1.0/9,   0/9}, {0, -1.5/9,   0/9}, {3.5/9,  -.7/9,    0}},
 {{-4.9/9, -.05/9,   0}, {-2.0/9,    0/9, 1.0/9}, {0,    0/9, 1.0/9}, {3.3/9,  -.5/9,    0}},
 {{-4.9/9,  .05/9,   0}, {-2.0/9,    1/9,   1/9}, {0,  1.0/9, 1.0/9}, {3.3/9,  -.2/9,    0}},
 {{-5.0/9,   .1/9, 0.0}, {-2.5/9,  3.0/9,   0/9}, {0,  2.5/9,   0/9}, {3.5/9,  0.0/9,  0.0}}
};

GLfloat tailControl[4][4][3]  = { 
 {{-1.0/9,  -.1/9,   0}, {-0.5/9, -0.6/9,   0/9}, {0, -0.7/9,   0/9}, {1.5/9,  -.8/9,    0}},
 {{-0.9/9, -.05/9,   0}, {-0.0/9,    0/9, 0.5/9}, {0,    0/9, 0.5/9}, {1.3/9,  -.5/9,    0}},
 {{-0.9/9,  .05/9,   0}, {-0.0/9,    1/9, 0.5/9}, {0,  1.0/9, 0.5/9}, {1.3/9,  -.2/9,    0}},
 {{-1.0/9,   .1/9, 0.0}, {-0.5/9,  0.6/9,   0/9}, {0,  0.8/9,   0/9}, {1.5/9,  0.9/9,  0.0}}
};

GLfloat texptsFish[2][2][2] = {{{0.0, 0.0}, {0.0, 1.0}}, 
			{{1.0, 0.0}, {1.0, 1.0}}};

GLUnurbsObj *fishNurb;

int posFlagF = 0;

drawFish() {

	if(!posFlagF)
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			tailControl[i][j][0] = tailControl[i][j][0] + 0.45;
			tailControl[i][j][1] = tailControl[i][j][1] - 0.05;

		}
		posFlagF = 1;
	}

   	fishNurb = gluNewNurbsRenderer();
	gluNurbsProperty(fishNurb, GLU_SAMPLING_TOLERANCE, 25.0);
   	gluNurbsProperty(fishNurb, GLU_DISPLAY_MODE, GLU_FILL);

   	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glGenTextures (1, &texture);

	free(total_texture);
	free(alpha_texture);
	free(caustic_texture);

	total_texture = (char*) malloc(4 * 256 * 256*sizeof(char));
	alpha_texture = (char*) malloc(256 * 256*sizeof(char));
	caustic_texture = (char*) malloc(3 * 256 * 256*sizeof(char));

  	load_texture ("alpha.jpg", alpha_texture, GL_ALPHA, 256);
      	load_texture ("fish1.jpg", caustic_texture, GL_RGB, 256);
  	for (i = 0; i < 256 * 256; i++) {
      		total_texture[4 * i] = caustic_texture[3 * i];
      		total_texture[4 * i + 1] = caustic_texture[3 * i + 1];
      		total_texture[4 * i + 2] = caustic_texture[3 * i + 2];
      		total_texture[4 * i + 3] = alpha_texture[i];
    	}

	GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
   	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   	GLfloat mat_shininess[] = { 100.0 };

   	//glClearColor (1, 0, 0, 0.0);
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

   	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &texptsFish[0][0][0]);

  	glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGBA,
		     GL_UNSIGNED_BYTE,  total_texture);

	GLfloat fishKnots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
   	GLfloat tFishknots[4] = {0.0, 0.0, 1.0, 1.0};

   	gluBeginSurface(fishNurb);
	/*gluNurbsSurface(fishNurb, 
                   4, tFishknots, 4, tFishknots,
                   4, 2, &texptsFish[0][0][0], 
                   2, 2, GL_MAP2_TEXTURE_COORD_2);*/
   	gluNurbsSurface(fishNurb, 
                   8, fishKnots, 8, fishKnots,
                   4 * 3, 3, &fishControl[0][0][0], 
                   4, 4, GL_MAP2_VERTEX_3);
   	gluEndSurface(fishNurb);

   	gluBeginSurface(fishNurb);
   	gluNurbsSurface(fishNurb, 
                   8, fishKnots, 8, fishKnots,
                   4 * 3, 3, &tailControl[0][0][0], 
                   4, 4, GL_MAP2_VERTEX_3);
   	gluEndSurface(fishNurb);

   	/*if (true) {
      		glPointSize(5.0);
      		glDisable(GL_LIGHTING);
      		glColor3f(1.0, 1.0, 0.0);
      		glBegin(GL_POINTS);
      		for (i = 0; i < 4; i++) {
        	 	for (j = 0; j < 4; j++) {
        	    		glVertex3f(tailControl[i][j][0], 
        	               	tailControl[i][j][1], tailControl[i][j][2]);
        	 	}
      		}
      		glEnd();
      		glEnable(GL_LIGHTING);
   	}*/
	glDisable(GL_TEXTURE_2D);
}
