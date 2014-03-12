/**********************************************************************
Surface of Revolution
************************************************************************/

#include <iostream>
using namespace std;

#include <math.h>
#include <GL/glut.h>
#include <list>

// window constants
#define GAP			10
#define WIDTH		520
#define HEIGHT		390
#define PI			3.14158265

// GLUT window ids
int main_w, ortho, persp;

int vertices = 4;
float vertAngles;
bool firstDot = true;
int valuesX[200];
int valuesY[200];
int counter = 0;
bool pointsMode = false;
bool polygonMode = false;
bool hiddenwireMode = false;
bool wireframeMode = true;

int dotX;
int dotY;

#define DEFAULT_BG_COLOR_R	0.0
#define DEFAULT_BG_COLOR_G	0.0
#define DEFAULT_BG_COLOR_B	0.0
float bg[3] = { DEFAULT_BG_COLOR_R, DEFAULT_BG_COLOR_G, DEFAULT_BG_COLOR_B };


// Callback functions 
void drawMain();
void drawOrtho();
void drawPersp();
void plotPoints( int, int, int, int );
void keyboard( unsigned char, int, int );
void menu( int );

void main( int argc, char *argv[] )
{
	// Initialize window system 
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( (WIDTH*2)+(GAP*3), HEIGHT+(GAP*2) );
	main_w = glutCreateWindow( "Project 3" );
	glutDisplayFunc( drawMain );

	// Orthographic profile view
	ortho = glutCreateSubWindow( main_w, GAP, GAP, WIDTH, HEIGHT );
	
	// Construct two-dimensional orthographic projection here
	glMatrixMode( GL_PROJECTION ); 
	glLoadIdentity();
	gluOrtho2D(0, WIDTH, 0, HEIGHT); 

	// Orthographic mouse callback
	glutMouseFunc( plotPoints );

	// Orthographic draw
	glutDisplayFunc( drawOrtho );

	// Perspective surface view
	persp = glutCreateSubWindow( main_w, GAP+WIDTH+GAP, GAP, WIDTH, HEIGHT );
	
	// Construct three-dimensional perspective projection here
	glMatrixMode( GL_PROJECTION ); 
	glLoadIdentity();
	//glOrtho (0, 520, 0, 390, 0, -520);
	gluPerspective( 45, 1.3333, 0.1, 10000 ); 
	

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, WIDTH, HEIGHT);
	gluLookAt(260.0, 195.0, -350, 260.0, 195.0, 260.0, 0, 1, 0);


	// Perspective draw
	glEnable( GL_DEPTH_TEST );
	glutDisplayFunc( drawPersp );
  
	// Perspective keyboard and menu callbacks
	glutKeyboardFunc( keyboard );
	glutCreateMenu( menu );
		glutAddMenuEntry( "Increase Resolution (or '+' key)", 2 );
		glutAddMenuEntry( "Decrease Resolution (or '-' key)", 3 );
		glutAddMenuEntry( "Display as Points (or 'p' key)", 4 );
		glutAddMenuEntry( "Display as Wireframe (or 'w' key)", 5 );
		glutAddMenuEntry( "Display as Hidden Wireframe (or 'h' key)", 6 );
		glutAddMenuEntry( "Display as Polygons (or 'm' key)", 7 );
		glutAddMenuEntry( "Exit ('q' key)", 1 );
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	// Main loop 
	glutMainLoop();
}

// Main background display
void drawMain()
{
	glClearColor(.5,.5,.5,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

// Orthographic profile draw
void drawOrtho()
{
	// Draw here
	glColor3f(.5, .5, .5);
	glBegin(GL_LINES);		
		glVertex2i (260, 0);
		glVertex2i (260, 390);
		glVertex2i (0, 195);
		glVertex2i (520, 195);
	glEnd();

	glColor3f(1.0, 1.0, 0);
	glPointSize(2.0);
	glBegin (GL_POINTS);
		glVertex2i (valuesX[counter - 1], valuesY[counter - 1]);
		//glVertex2i (valuesX[counter], valuesY[counter]);
	glEnd();
	
	glColor3f(1, 1, 1);
	if(firstDot == false)
	{
		glBegin (GL_LINES);
			glVertex2i(valuesX[counter - 2], valuesY[counter - 2]);
			glVertex2i(valuesX[counter - 1], valuesY[counter - 1]);
		glEnd();
	}

	glutSwapBuffers();
}

// Orthographic mouse callback
void plotPoints( int button, int state, int x, int y )
{	
	// Execute draw commands in ortho window
	glutSetWindow( ortho );
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			if (counter >= 1)
				firstDot = false;
			dotX = x;
			dotY = HEIGHT - y;
			valuesX[counter] = x;
			valuesY[counter] = HEIGHT - y;
			counter++;
			//oldX = dotX;
			//oldY = dotY;
			//dotX = x;
			//dotY = HEIGHT - y;
		}	
	}
	glutPostRedisplay();

	 // Execute draw commands in persp window
	glutSetWindow( persp );
	glutPostRedisplay();
	return;
}

// Perspective surface draw
void drawPersp()
{
	glClearColor( 0, 0, 0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  

	vertAngles = (2 * PI) / vertices;
	// Draw here
	glColor3f(1, 1, 1);
	
	if (wireframeMode == true)
	{
		if(firstDot == false)
		{
			for (int i = 1; i < counter; i++)
			{	
				for (int j = 0; j < vertices; j++)
				{
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
				glBegin(GL_POLYGON);
					glVertex3f (valuesX[i] + ((valuesX[i] - WIDTH / 2) * (cos(vertAngles * j) - 1)), valuesY[i] , valuesX[i]  + ((valuesX[i] - WIDTH / 2) * (sin(vertAngles * j) - 1)));
					glVertex3f (valuesX[i] + ((valuesX[i] - WIDTH / 2) * (cos(vertAngles * (j - 1)) - 1)), valuesY[i] , valuesX[i]  + ((valuesX[i] - WIDTH / 2) * (sin(vertAngles * (j - 1)) - 1)));
					glVertex3f (valuesX[i - 1] + ((valuesX[i - 1] - WIDTH / 2) * (cos(vertAngles * (j - 1)) - 1)), valuesY[i - 1] , valuesX[i - 1]  + ((valuesX[i - 1] - WIDTH / 2) * (sin(vertAngles * (j - 1)) - 1)));
					glVertex3f (valuesX[i - 1] + ((valuesX[i - 1] - WIDTH / 2) * (cos(vertAngles * j) - 1)), valuesY[i - 1] , valuesX[i - 1]  + ((valuesX[i - 1] - WIDTH / 2) * (sin(vertAngles * j) - 1)));
				glEnd();
				}
			}
		}
	}

	else if (pointsMode == true)
	{
		if(firstDot == false)
		{
			for (int i = 0; i < counter; i++)
			{			
				glBegin (GL_POINTS);
				for (int j = 0; j < vertices; j++)
				{		
					glVertex3f (valuesX[i] + ((valuesX[i] - WIDTH / 2) * (cos(vertAngles * j) - 1)), valuesY[i] , valuesX[i]  + ((valuesX[i] - WIDTH / 2) * (sin(vertAngles * j) - 1)));		
				}	
				glEnd();
			}
		}
	}

	else if (hiddenwireMode == true)
	{
		if(firstDot == false)
		{
			for (int i = 1; i < counter; i++)
			{						
				for (int j = 0; j < vertices; j++)
				{	
				glEnable( GL_POLYGON_OFFSET_FILL );
				glPolygonOffset( 1.0, 1.0 );
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				glColor3f(0.0, 0.0, 0.0);
				glBegin (GL_POLYGON);
					glVertex3f (valuesX[i] + ((valuesX[i] - WIDTH / 2) * (cos(vertAngles * j) - 1)), valuesY[i] , valuesX[i]  + ((valuesX[i] - WIDTH / 2) * (sin(vertAngles * j) - 1)));
					glVertex3f (valuesX[i] + ((valuesX[i] - WIDTH / 2) * (cos(vertAngles * (j - 1)) - 1)), valuesY[i] , valuesX[i]  + ((valuesX[i] - WIDTH / 2) * (sin(vertAngles * (j - 1)) - 1)));
					glVertex3f (valuesX[i - 1] + ((valuesX[i - 1] - WIDTH / 2) * (cos(vertAngles * (j - 1)) - 1)), valuesY[i - 1] , valuesX[i - 1]  + ((valuesX[i - 1] - WIDTH / 2) * (sin(vertAngles * (j - 1)) - 1)));
					glVertex3f (valuesX[i - 1] + ((valuesX[i - 1] - WIDTH / 2) * (cos(vertAngles * j) - 1)), valuesY[i - 1] , valuesX[i - 1]  + ((valuesX[i - 1] - WIDTH / 2) * (sin(vertAngles * j) - 1)));
				glEnd();
				}		
			}
			for (int i = 1; i < counter; i++)
			{						
				for (int j = 0; j < vertices; j++)
				{	
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
				glColor3f(1.0, 1.0, 1.0);
				glBegin (GL_POLYGON);
					glVertex3f (valuesX[i] + ((valuesX[i] - WIDTH / 2) * (cos(vertAngles * j) - 1)), valuesY[i] , valuesX[i]  + ((valuesX[i] - WIDTH / 2) * (sin(vertAngles * j) - 1)));
					glVertex3f (valuesX[i] + ((valuesX[i] - WIDTH / 2) * (cos(vertAngles * (j - 1)) - 1)), valuesY[i] , valuesX[i]  + ((valuesX[i] - WIDTH / 2) * (sin(vertAngles * (j - 1)) - 1)));
					glVertex3f (valuesX[i - 1] + ((valuesX[i - 1] - WIDTH / 2) * (cos(vertAngles * (j - 1)) - 1)), valuesY[i - 1] , valuesX[i - 1]  + ((valuesX[i - 1] - WIDTH / 2) * (sin(vertAngles * (j - 1)) - 1)));
					glVertex3f (valuesX[i - 1] + ((valuesX[i - 1] - WIDTH / 2) * (cos(vertAngles * j) - 1)), valuesY[i - 1] , valuesX[i - 1]  + ((valuesX[i - 1] - WIDTH / 2) * (sin(vertAngles * j) - 1)));
				glEnd();
				}		
			}
		}
	}

	else if (polygonMode == true)
	{
		if(firstDot == false)
		{		
			for (int i = 1; i < counter; i++)
			{	
				for (int j = 0; j < vertices; j++)
				{
				bg[0] = float(rand())/RAND_MAX;
				bg[1] = float(rand())/RAND_MAX;
				bg[2] = float(rand())/RAND_MAX;
				glColor3f(bg[0], bg[1], bg[2]);
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				glBegin (GL_POLYGON);
					glVertex3f (valuesX[i] + ((valuesX[i] - WIDTH / 2) * (cos(vertAngles * j) - 1)), valuesY[i] , valuesX[i]  + ((valuesX[i] - WIDTH / 2) * (sin(vertAngles * j) - 1)));
					glVertex3f (valuesX[i] + ((valuesX[i] - WIDTH / 2) * (cos(vertAngles * (j - 1)) - 1)), valuesY[i] , valuesX[i]  + ((valuesX[i] - WIDTH / 2) * (sin(vertAngles * (j - 1)) - 1)));
					glVertex3f (valuesX[i - 1] + ((valuesX[i - 1] - WIDTH / 2) * (cos(vertAngles * (j - 1)) - 1)), valuesY[i - 1] , valuesX[i - 1]  + ((valuesX[i - 1] - WIDTH / 2) * (sin(vertAngles * (j - 1)) - 1)));
					glVertex3f (valuesX[i - 1] + ((valuesX[i - 1] - WIDTH / 2) * (cos(vertAngles * j) - 1)), valuesY[i - 1] , valuesX[i - 1]  + ((valuesX[i - 1] - WIDTH / 2) * (sin(vertAngles * j) - 1)));
				glEnd();
				}
			}
		}	
	}
  glutSwapBuffers();
}

// Perspective menu callback 
void menu( int value )
{
 switch( value )
    {
		case 1:			// exit program
			exit(1);
			break;
		case 2:			// exit program
			vertices++;	
			glutSetWindow( persp );
			glutPostRedisplay();
			break;
		case 3:			// exit program
			vertices--;		
			glutSetWindow( persp );
			glutPostRedisplay();
			break;
		case 4:			// exit program
			pointsMode = true;	
			polygonMode = false;
			hiddenwireMode = false;
			wireframeMode = false;
			glutSetWindow( persp );
			glutPostRedisplay();
			break;
		case 5:			// exit program
			pointsMode = false;	
			hiddenwireMode = false;
			polygonMode = false;
			wireframeMode = true;
			glutSetWindow( persp );
			glutPostRedisplay();
			break;
		case 6:			// exit program
			hiddenwireMode = true;
			pointsMode = false;
			polygonMode = false;
			wireframeMode = false;
			glutSetWindow( persp );
			glutPostRedisplay();
			break;
		case 7:			// exit program
			polygonMode = true;	
			pointsMode = false;
			hiddenwireMode = false;
			wireframeMode = false;
			glutSetWindow( persp );
			glutPostRedisplay();
			break;
	}	
}

// Perspective keyboard callback 
void keyboard( unsigned char key, int x, int y )
{
  switch( key )
    {
		case '+':
			vertices++;	
			glutSetWindow( persp );
			glutPostRedisplay();
			break;
		case '-':
			vertices--;	
			glutSetWindow( persp );
			glutPostRedisplay();
			break;
		case 'p':
			pointsMode = true;	
			polygonMode = false;
			hiddenwireMode = false;
			wireframeMode = false;
			glutSetWindow( persp );
			glutPostRedisplay();
			break;
		case 'w':
			pointsMode = false;	
			polygonMode = false;
			hiddenwireMode = false;
			wireframeMode = true;
			glutSetWindow( persp );
			glutPostRedisplay();
			break;
		case 'h':
			hiddenwireMode = true;
			pointsMode = false;
			polygonMode = false;
			wireframeMode = false;
			glutSetWindow( persp );
			glutPostRedisplay();
			break;
		case 'm':
			polygonMode = true;	
			pointsMode = false;
			hiddenwireMode = false;
			wireframeMode = false;
			glutSetWindow( persp );
			glutPostRedisplay();
			break;
		case 'q':
			exit(1);
			break;
    }
}
		    