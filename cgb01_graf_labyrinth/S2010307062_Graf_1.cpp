#include <cstdlib>

#include "GL\glew.h"
#include "GL\freeglut.h"
#include <iostream>
#include <random>

#define KEY_SPACE 32
#define KEY_ESCAPE 27
#define ROWS_LABYRINTH 12
#define COLUMNS_LABYRINTH 12
#define SIZE_BLOCK 4
#define WITH_LABYRINTH (ROWS_LABYRINTH * SIZE_BLOCK)
#define HEIGHT_LABYRINTH (COLUMNS_LABYRINTH * SIZE_BLOCK)
#define SIZE_LABYRINTH (ROWS_LABYRINTH * COLUMNS_LABYRINTH)
#define GAMESPEED 1

using namespace std;
int windowid;
// Navigation
GLdouble navX = -2;
GLdouble navY = 1;
GLdouble navZ = 0;


GLdouble mouseX = 1;
GLdouble mouseY = 0;
GLdouble mouseZ = 3;
GLdouble angle = 0;
int labyrinth_rand;
bool is_jumping = false;


template <typename E = std::mt19937_64>
int get_random_uniform(int const l, int const u)
{
	static E generator{std::random_device{}()}; //seed erzeugen, ist nicht determistisch zufällig. atomzerfall
	return std::uniform_int_distribution<>{l, u}(generator);
}

int get_random_number(const int num_min = 0, const int num_max = 0)
{
	return get_random_uniform(num_min, num_max);
}

int idx(int const r, int const c)
{
	// transform coordinates from 2D to 1D
	return r * COLUMNS_LABYRINTH + c;
}

int labyrinths[2][SIZE_LABYRINTH] = {
	{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1,
		1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1,
		1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1,
		1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1,
		1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1,
		1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1,
		1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	},

	{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1,
		1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1,
		1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1,
		1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1,
		1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1,
		1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	}

};

void grass()
{
	// draw green ground for outside world
	glPushMatrix();
	glTranslated(0, 0, 0);
	glColor3d(0, 0, 1); // Green
	glBegin(GL_QUADS);
	glVertex3d(-400 , 0, -400 );
	glVertex3d(400, 0, -400 );
	glVertex3d(400, 0, 400 );
	glVertex3d(-400, 0, 400 );
	glEnd();
	glPopMatrix();
}

//TASK 1
void table(int x, int y)
{
	// draw table with base, stand & plate

	glPushMatrix();
	glColor3f(0.1f, 0.0f, 0.0f); //Brown
	glTranslatef(x, 0.5 * SIZE_BLOCK / 2, y);
	//top
	glPushMatrix();
	glTranslatef(0, 0.0, 0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.5 * SIZE_BLOCK / 2, 0.025 * SIZE_BLOCK / 2, 64, 64);
	glPopMatrix();
	//stand
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.025 * SIZE_BLOCK / 2, 0.5 * SIZE_BLOCK / 2, 64, 64);
	glPopMatrix();
	glPopMatrix();
}

//TASK 1
void chair(int x, int y)
{
	// draw table with base, stand & plate

	glPushMatrix();
	glColor3f(0.1f, 0.0f, 0.0f); //Brown
	glTranslatef(x, 0.25 * SIZE_BLOCK / 2, y);
	//top
	glPushMatrix();
	glTranslatef(0, 0.0, 0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.25 * SIZE_BLOCK / 2, 0.025 * SIZE_BLOCK / 2, 64, 64);
	glPopMatrix();
	//stand
	glPushMatrix();
	glTranslatef(-0.125 * SIZE_BLOCK / 2, 0.0, -0.125 * SIZE_BLOCK / 2);
	glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.025 * SIZE_BLOCK / 2, 0.25 * SIZE_BLOCK / 2, 64, 64);
	glPopMatrix();
	//stand
	glPushMatrix();
	glTranslatef(-0.125 * SIZE_BLOCK / 2, 0.0, 0.125 * SIZE_BLOCK / 2);
	glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.025 * SIZE_BLOCK / 2, 0.25 * SIZE_BLOCK / 2, 64, 64);
	glPopMatrix();
	//stand
	glPushMatrix();
	glTranslatef(0.125 * SIZE_BLOCK / 2, 0.0, -0.125 * SIZE_BLOCK / 2);
	glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.025 * SIZE_BLOCK / 2, 0.25 * SIZE_BLOCK / 2, 64, 64);
	glPopMatrix();
	//stand
	glPushMatrix();
	glTranslatef(0.125 * SIZE_BLOCK / 2, 0.0, 0.125 * SIZE_BLOCK / 2);
	glRotatef(90, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.025 * SIZE_BLOCK / 2, 0.25 * SIZE_BLOCK / 2, 64, 64);
	glPopMatrix();
	glPopMatrix();
}


void objects()
{
	table(6 * SIZE_BLOCK, 4 * SIZE_BLOCK);
	chair(7 * SIZE_BLOCK, 4 * SIZE_BLOCK);
	chair(6 * SIZE_BLOCK, 5 * SIZE_BLOCK);
}

void get_coordinates(size_t i, size_t j, GLdouble& x, GLdouble& z)
{
	x = i * static_cast<GLdouble>(SIZE_BLOCK);


	z = j * static_cast<GLdouble>(SIZE_BLOCK);
}

bool is_move_possible(GLdouble x, GLdouble z)
{
	GLdouble new_x, new_z;
	for (size_t i = 0; i < ROWS_LABYRINTH; i++)
	{
		for (size_t j = 0; j < COLUMNS_LABYRINTH; j++)
		{
			if (labyrinths[labyrinth_rand][idx(i, j)] == 1)
			{
				get_coordinates(i, j, new_x, new_z);
				if (fabs(new_x - x) < SIZE_BLOCK / 2 && fabs(new_z - z) < SIZE_BLOCK / 2)
				{
					return false;
				}
			}
		}
	}
	return true;
}

void collisition_detection(GLdouble x, GLdouble z)
{
	if (is_move_possible(x, z))
	{
		navX = x;
		navZ = z;
	}
}


void create_block( size_t i, size_t j)
{

	GLdouble x, z;
	glPushMatrix();
	get_coordinates(i, j, x, z);
	glTranslated(x, 0, z);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3d(1.0, 0.0, 0.0); // black
	glutSolidCube(SIZE_BLOCK);
	glPopMatrix();
}

// 2. Rendering of labyrinth
void labyrinth()
{
	for (size_t i = 0; i < COLUMNS_LABYRINTH; i++)
	{
		for (size_t j = 0; j < ROWS_LABYRINTH; j++)
		{
			if (labyrinths[labyrinth_rand][idx(i, j)] == 1)
			{
				create_block(i, j);
			}
		}
	}
}

void animate_jump()
{
	static bool toggle = true;
	if (mouseY < SIZE_BLOCK-1 && toggle)
	{
		mouseY += 0.025;
	}
	else if (mouseY > 0)
	{
		mouseY -= 0.05;
		toggle = false;
	}
	else
	{
		is_jumping = false;
		toggle = true;
	}
}

void renderScene(void)
{
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	if (is_jumping)
	{
		animate_jump();
	}
	glLoadIdentity();
	// set camera position
	gluLookAt(navX, navY + mouseY, navZ, static_cast<GLdouble>(navX) + mouseX * SIZE_BLOCK, mouseY/2, navZ - (mouseZ * SIZE_BLOCK), 0.0f, 1.0f, 0.0f);
	// set camera position

	labyrinth();
	objects();
	grass();
	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{
	std::cout << " x : " << navX << " y " << navZ;
	switch (key)
	{
	case 'a':
		collisition_detection(navX - mouseZ * GAMESPEED, navZ - mouseX * GAMESPEED);
		break;
	case 'd':
		collisition_detection(navX + mouseZ * GAMESPEED, navZ + mouseX * GAMESPEED);
		break;
	case 'w':
		collisition_detection(navX + mouseX * GAMESPEED, navZ - mouseZ * GAMESPEED);
		break;
	case 's':
		collisition_detection(navX - mouseX * GAMESPEED, navZ + mouseZ * GAMESPEED);
		break;
	case 32:
		is_jumping = true;
		break;
	case 27: // Escape key
		glutDestroyWindow(windowid);
		exit(0);
		break;
	}

	glutPostRedisplay();
}

/*-[Reshape Callback]--------------------------------------------------------*/
void reshapeFunc(int x, int y)
{
	if (y == 0 || x == 0) return; //Nothing is visible then, so return

	glMatrixMode(GL_PROJECTION); //Set a new projection matrix
	glLoadIdentity();
	gluPerspective(60.0, static_cast<GLdouble>(x) / static_cast<GLdouble>(y), 0.5, 60.0);
	glViewport(0, 0, x, y); //Use the whole window for rendering
}

void mouseMovement(int x, int y)
{
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height= glutGet(GLUT_WINDOW_HEIGHT);
	angle += (width/2 - (GLdouble)x) * 0.001;
	mouseX = sin(angle);
	mouseZ = -cos(angle);
	glutWarpPointer(width / 2, height / 2);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	navX = 8 * SIZE_BLOCK;
	navY = 1;
	navZ = 6 * SIZE_BLOCK;
	angle = 0;
	labyrinth_rand = get_random_number(0, 1);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); // rgba color schema, double buffering
	glutInitWindowPosition(500, 500); // determines the initial position of the window
	glutInitWindowSize(800, 600); // determines the size of the window
	windowid = glutCreateWindow("OpenGL"); // create and name window
	glutFullScreen(); // set full screen mode
	glutSetCursor(GLUT_CURSOR_NONE); // show no cursor

	glutReshapeFunc(reshapeFunc);
	glutPassiveMotionFunc(mouseMovement);
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(keyboard);

	glEnable(GL_DEPTH_TEST);
	glutMainLoop(); // start the main loop of GLUT

	return EXIT_SUCCESS;
}
