/* Source.cpp - Lab 10 - Matthew Panetta
I made a fish in water. You can rotate the fish using the arrow keys.
Press the HOME key to return to default angle.

The fish will animate its tail back and forth automatically.
*/

#include <stdlib.h>
#include <GL/glut.h>

int uSteps = 30;					// bezier curve steps
int vSteps = 30;					// bezier curve steps
const double TAIL_SPEED = 0.1;		// tail animation speed
double dz = TAIL_SPEED;				// initial animation direction

GLfloat ctrlptsLeft[4][4][3] = {	// left side of fish (bezier points)
		{ { 0, -3, 0 }, { 1, -2, 0 }, { 2, -1, 0 }, { 3, 0, 0 } },
		{ { -1, -2, 0 }, { 0, -1, 2.5 }, { 1, 0, 2.5 }, { 2, 1, 0 } },
		{ { -2, -1, 0 }, { -1, 0, 2.5 }, { 0, 1, 2.5 }, { 1, 2, 0 } },
		{ { -6, 0, 0 }, { -2, 1, 0 }, { -1, 2, 0 }, { 0, 3, 0 } },
};

GLfloat ctrlptsRight[4][4][3] = {	// right side of fish (bezier points)
		{ { -6, 0, 0 }, { -2, 1, 0 }, { -1, 2, 0 }, { 0, 3, 0 }, },
		{ { -2, -1, 0 }, { -1, 0, -2.5 }, { 0, 1, -2.5 }, { 1, 2, 0 }, },
		{ { -1, -2, 0 }, { 0, -1, -2.5 }, { 1, 0, -2.5 }, { 2, 1, 0 } },
		{ { 0, -3, 0 }, { 1, -2, 0 }, { 2, -1, 0 }, { 3, 0, 0 } },
};

GLfloat rx = 0, ry = 0;				// rotation initial values

// Draw the fish's eye and pupil
void draw_eye()
{
	GLfloat mat_diffuse[] = { 0.85, 0.89, 0.86, 1.0 };		// white-ish eye
	GLfloat pupil_diffuse[] = { 0.15, 0.25, 0.86, 1.0 };	// blue pupil
	GLfloat mat_specular[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);		// apply materials
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glPushMatrix();
	glScalef(0.6, 0.6, 0.3);			// draw eye
	glutSolidSphere(1, 18, 18);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.25, -0.20, 0.20);	// draw pupil
	glRotatef(10, 1, 1, 0);				// rotate pupil about  the x and y axis to conform it to the eye's shaope
	glScalef(0.20, 0.20, 0.1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, pupil_diffuse);		// set pupil material
	glutSolidSphere(1, 18, 18);
	glPopMatrix();
}

// Draw the fish
void draw_fish()
{
	glPushMatrix();
	
	GLfloat mat_diffuse[] = { 1.0, 0.49, 0.0, 1.0 };				// the fish has an orange color
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);		// set the material

	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,							// map the left side of the fish
		0, 1, 12, 4, &ctrlptsLeft[0][0][0]);
	glMapGrid2f(uSteps, 0.0, 1.0, vSteps, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0, uSteps, 0, vSteps);						// display on screen

	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,							// map the right side of the fish
		0, 1, 12, 4, &ctrlptsRight[0][0][0]);
	glMapGrid2f(uSteps, 0.0, 1.0, vSteps, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0, uSteps, 0, vSteps);						// display on screen

	glPopMatrix();
}

// Draw the water
void draw_water()
{
	glPushMatrix();
	glLoadIdentity();						// The water does not rotate

	GLfloat mat_diffuse[] = { 0.0, 0.15, 0.90, 0.35 };				// water has a transparent blue material
	GLfloat mat_specular[] = { 0.3, 0.3, 0.3, 0.35 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);				// apply the material
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

	glTranslatef(0.0, -13.5, -14.75);		// position the water below the fish
	glScalef(2, 2, 2);						// make the water look endless from the camera's point of view by scaling it up
	glRotatef(45, 1, 0, 0);					// set proper water angle
	glutSolidCube(20);						// draw water (cube)
	glPopMatrix();
}

// Animate the fish's tail back and forth
void animate_tail(int value)
{
	if (ctrlptsLeft[3][0][2] >= 2)
	{
		dz = -1 * TAIL_SPEED;				// reverse tail direction if beyond a certain point
	}
	else if (ctrlptsLeft[3][0][2] <= -2)
	{
		dz = TAIL_SPEED;					// reverse tail direction if beyond a certain point
	}

	ctrlptsLeft[3][0][2] += dz;				// set the points
	ctrlptsRight[0][0][2] += dz;

	glutPostRedisplay();					// redisplay the scene
	glutTimerFunc(25, animate_tail, 1);		// reset timer function
}

// Initialize the lights
void initlights(void)
{
	GLfloat ambient[] = { 0.1, 0.1, 0.1, 1.0 };					// set the light material and position
	GLfloat position[] = { -1.0, 3.0, 18.0, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 0.49, 0.0, 1.0 };
	GLfloat mat_specular[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };

	glEnable(GL_LIGHTING);										// enable lighting
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);					// set light0's properties 
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);			// set initial material
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

// Display the scene
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	
	glTranslatef(-1.0, 0.0, 0.0);		// position fish in center of the screen
	glRotatef(rx, 1, 0, 0);				// apply rotations, based on the user's input (arrow keys)
	glRotatef(ry, 0, 1, 0);

	glPushMatrix();

	glRotatef(37, 1, 0, 0);				// initial fish rotation
	glRotatef(-37, 0, 1, 0);			// set these to zero if you want to view the fish straight on

	draw_fish();						// draw the fish first
	draw_water();						// draw the water second, so part of the fish appears underwater

	glPushMatrix();
	glTranslatef(0.75, 0.55, 1.0);
	glRotatef(-20, 1, 0, 0);			// rotate the eyes about the x and y axes so they conform to the fish's shape.
	glRotatef(20, 0, 1, 0);
	draw_eye();							// draw left eye
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.75, 0.55, -1.0);
	glRotatef(20, 1, 0, 0);				// Conform the eyes to the fish's shape
	glRotatef(160, 0, 1, 0);			// rotate 180 degrees about the y-axis ( it will be 160 degrees because I rotate the eyes around the y axis 20 degrees )
	glScalef(-1, 1, 1);					// and reflect the eye
	draw_eye();							// draw right eye
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glFlush();
}

// Set initial values
void init(void)
{
	glClearColor(0.05, 0.05, 0.05, 1.0);		// set clear color to dark background (makes water look more realistic)
	glEnable(GL_DEPTH_TEST);					// enable various opengl attributes
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	initlights();								// display the lights on screen
}

// Reshape function
void reshape(int w, int h)
{
	float width = -8.0f;						// Initial width of screen grid  (not screen size)
	float height = 6.0f;						// Initial height of screen grid (not screen size)
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(width, height, width*(GLfloat)h / (GLfloat)w,
		height*(GLfloat)h / (GLfloat)w, width, height);
	else
		glOrtho(width*(GLfloat)w / (GLfloat)h,
		height*(GLfloat)w / (GLfloat)h, width, height, width, height);
	glMatrixMode(GL_MODELVIEW);					// switch to modelview matrix and load the identity matrix to reset all prior transformations
	glLoadIdentity();
}

// Set keyboard mappings
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:									// press escape to exit the program
		exit(0);
		break;
	}
}

// Set special key mappings
void processSpecialKeys(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT:
		ry += 3;				// press left arrow to rotate fish about the y axis by 3 units
		break;
	case GLUT_KEY_RIGHT:
		ry -= 3;				// press right arrow to rotate fish about the y axis by -3 units
		break;
	case GLUT_KEY_UP:
		rx += 3;				// press up arrow to rotate fish about the x axis by 3 units
		break;
	case GLUT_KEY_DOWN:
		rx -= 3;				// press down arrow to rotate fish about the x axis by -3 units
		break;
	case GLUT_KEY_HOME:
		rx = 0;					// press home key to reset rotations
		ry = 0;
		break;
	}

	glutPostRedisplay();		// re-display the scene for instant update
}

// Main program
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(processSpecialKeys);
	glutTimerFunc(25, animate_tail, 1);
	glutMainLoop();
	return 0;
}