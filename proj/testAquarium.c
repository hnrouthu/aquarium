#include "headers.h"

void drawStone(GLfloat x, GLfloat z);

GLfloat ctrlpoints[6][6][3] = {
 {{-2.5, 0.0, -4.5},{-1.5, 0.0, -4.5},{-0.5, 0.0,-4.5 },{0.5, 0.0, -4.5 },{1.5, 0.0,-4.5},{2.5, 0.0,-4.5}},
 {{-2.5, 0.0, -1.5},{-1.5, 0.0, -1.5},{-0.5, 0.0,-1.5 },{0.5, 0.0, -1.5 },{1.5, 0.0,-1.5},{2.5, 0.0,-1.5}}, 
 {{-2.5, 0.0, -0.5},{-1.5, 0.0, -0.5},{-0.5, 4.0,-0.5 },{0.5, 0.0, -0.5 },{1.5, 0.0,-0.5},{2.5, 0.0,-0.5}}, 
 {{-2.5, 0.0,  0.5},{-1.5, 0.0,  0.5},{-0.5, 0.0, 0.5 },{0.5, 0.0,  0.5 },{1.5, 0.0, 0.5},{2.5, 0.0, 0.5}}, 
 {{-2.5, 0.0,  1.5},{-1.5, 0.0,  1.5},{-0.5, 0.0, 1.5 },{0.5, 0.0,  1.5 },{1.5, -2.0, 1.5},{2.5, 0.0, 1.5}},
 {{-2.5, 0.0,  6.5},{-1.5, 0.0,  6.5},{-0.5, 0.0, 6.5 },{0.5, 0.0,  6.5 },{1.5, 0.0, 6.5},{2.5, 0.0, 6.5}}
};

GLuint texture;
unsigned char* total_texture;
unsigned char* alpha_texture;
unsigned char* caustic_texture;

int i, j, k;
int posFlag = 0;
GLfloat x = -1.7, z = 1.5;

GLfloat texpts[2][2][2] = {{{0.0, 0.0}, {0.0, 1.0}}, 
			{{1.0, 0.0}, {1.0, 1.0}}};

static GLfloat spin = 0.0;

bool movingUp = false; // Whether or not we are moving up or down  
float xLocation = 0.0f;
float yLocation = 0.0f;
float zLocation = 0.0f; // Keep track of our position on the y axis. 
float yRotationAngle = 0.0f;
int rotateX, rotateY;

GLUnurbsObj *theNurb;

void nurbsError(GLenum errorCode) {
   	const GLubyte *estring;

   	estring = gluErrorString(errorCode);
   	fprintf (stderr, "Nurbs Error: %s\n", estring);
   	exit (0);
}

void init(void) {

	glGenTextures (1, &texture);
	total_texture = (char*) malloc(4 * 256 * 256*sizeof(char));
	alpha_texture = (char*) malloc(256 * 256*sizeof(char));
	caustic_texture = (char*) malloc(3 * 256 * 256*sizeof(char));

  	load_texture ("alpha1.jpg", alpha_texture, GL_ALPHA, 256);
      	load_texture ("a.jpg", caustic_texture, GL_RGB, 256);
  	for (i = 0; i < 256 * 256; i++) {
      		total_texture[4 * i] = caustic_texture[3 * i];
      		total_texture[4 * i + 1] = caustic_texture[3 * i + 1];
      		total_texture[4 * i + 2] = caustic_texture[3 * i + 2];
      		total_texture[4 * i + 3] = alpha_texture[i];
    	}
   	   
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   	GLfloat mat_shininess[] = { 100.0 };

   	glClearColor (0.1, 0.1, 0.1, 0.5);
   	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	float posLight0[4] = {-1.0f, 1.0f, 1.0f, 0.0f};
	float ambLight0[4] = {0.5f, 0.5f, 0.5f, 0.5f};

	glLightfv(GL_LIGHT0, GL_POSITION, posLight0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambLight0);

	float difLight0[4] = {0.5f, 0.5f, 0.5f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difLight0);

   	glEnable(GL_LIGHTING);
   	glEnable(GL_LIGHT0);
   	glEnable(GL_DEPTH_TEST);
   	glEnable(GL_AUTO_NORMAL);
   	glEnable(GL_NORMALIZE);

   	theNurb = gluNewNurbsRenderer();
   	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
   	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
   	gluNurbsCallback(theNurb, GLU_ERROR, 
                    (GLvoid (*)()) nurbsError);

}

void display(void) {

   	GLfloat knots[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
   	GLfloat tknots[4] = {0.0, 0.0, 1.0, 1.0};
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	init();
//	gluLookAt (0.0, 6.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);	
	glPushMatrix();

   	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, texture);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   	glEnable(GL_MAP2_TEXTURE_COORD_2);

   	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &texpts[0][0][0]);

  	glTexImage2D(GL_TEXTURE_2D, 0, 3, 256, 256, 0, GL_RGBA,
		     GL_UNSIGNED_BYTE,  total_texture);

   	glRotatef(15.0, 1.0, 1.0, 1.0);
	glRotatef(rotateX, 0.0, rotateY, 1.0);
	gluBeginSurface(theNurb);
	gluNurbsSurface(theNurb, 
                   4, tknots, 4, tknots,
                   4, 2, &texpts[0][0][0], 
                   2, 2, GL_MAP2_TEXTURE_COORD_2);
	gluNurbsSurface(theNurb, 
                   12, knots, 12, knots,
                   6 * 3, 3, &ctrlpoints[0][0][0], 
                   6, 6, GL_MAP2_VERTEX_3);
   	gluEndSurface(theNurb);
   	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations  
//	gluLookAt (0.0, 6.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0f, 0.7f, -1.0f); 
	glTranslatef(xLocation, yLocation, zLocation); // Translate our object along the y axis  
    
	if (movingUp) // If we are moving up  
		xLocation += 0.005f; // Move up along our yLocation  
	else  // Otherwise  
		xLocation -= 0.005f; // Move down along our yLocation  
  
	if (xLocation < -2.0f) // If we have gone up too far  
		movingUp = false; // Reverse our direction so we are moving down  
	else if (xLocation > 2.0f) // Else if we have gone down too far  
		movingUp = true; // Reverse our direction so we are moving up  

	if (yLocation < -2.0f) // If we have gone up too far  
		movingUp = false; // Reverse our direction so we are moving down  
	else if (yLocation > 2.0f) // Else if we have gone down too far  
		movingUp = true; // Reverse our direction so we are moving up  

	//glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f); // Rotate our object around the y axis  
	glRotatef(rotateX, 0.0, rotateY, 1.0);
	drawFish();
	glPopMatrix();

	glPushMatrix();
	glLoadIdentity(); 
//	gluLookAt (0.0, 6.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(rotateX, 0.0, rotateY, 1.0);
	drawStone(-1.7, 1.5);
	drawWeed();
	glPopMatrix();

   	glFlush ();
	glutSwapBuffers();
}

void reshape (int w, int h) {
	printf("width and height are %d %d\n", w,h);
   	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   	glMatrixMode (GL_PROJECTION);
   	glLoadIdentity();
   	if (w <= h)
      		glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,
        		1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
   	else
      		glOrtho (-1.5*(GLfloat)w/(GLfloat)h,
         		1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
   	glMatrixMode(GL_MODELVIEW);
}
void spinDisplay(void) {
   	spin = spin + 1.0;
   	if (spin > 360.0)
      		spin = spin - 360.0;
	glutPostRedisplay();
}
void mouse(int button, int state, int x, int y) {
   	switch (button) {
      		case GLUT_LEFT_BUTTON:
         		if (state == GLUT_DOWN)
            			glutIdleFunc(spinDisplay);
       			break;
      		case GLUT_MIDDLE_BUTTON:
         		if (state == GLUT_DOWN)
            			glutIdleFunc(NULL);
         		break;
      		default:
         		break;
   	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
  	switch (key) {
  		case 27:             // ESCAPE key
	  		exit (0);
	  		break;
  		case '6':
			xLocation += 0.005f;
	  		break;
  		case '4':
			xLocation -= 0.005f;
	  		break;
	  	case '8':
			yLocation += 0.005f;
	  		break;
	  	case '2':
			yLocation -= 0.005f;
	  		break;
	  	case '1':
			zLocation += 0.005f;
	  		break;
	  	case '3':
			zLocation -= 0.005f;
	  		break;
  	}
	glutPostRedisplay();
}

void rotateInput(int key, int x, int y) {
  	switch (key) {
		case GLUT_KEY_LEFT: 
		        rotateX = (rotateX + 2) % 360;
			break;
		case GLUT_KEY_RIGHT: 
		        rotateX = (rotateX - 2) % 360; 
			break;
		case GLUT_KEY_UP: 
		        rotateY = (rotateY + 2) % 360; 
			break;
		case GLUT_KEY_DOWN: 
		        rotateY = (rotateY - 2) % 360; 
			break;
  	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
   	glutInit(&argc, argv);
   	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   	glutInitWindowSize (800, 600); 
   	glutInitWindowPosition (100, 100);
   	glutCreateWindow ("Aquarium");
   	init ();
   	glutDisplayFunc(display); 
   	glutReshapeFunc(reshape);
 	//glutIdleFunc(spinDisplay);
   	glutMouseFunc(mouse);
	glutKeyboardFunc (keyboard);
	glutSpecialFunc(rotateInput);
   	glutMainLoop();
   	return 0;
}
