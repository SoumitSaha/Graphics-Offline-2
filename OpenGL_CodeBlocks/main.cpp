#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

int radius = 25;
double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
int y_angle, z_angle;

struct point
{
	double x,y,z;
};

struct position{
    double x, y;
};

struct position l;

void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawCircle(double radius,int segments)
{
    int i;
    struct point points[1000];
    //glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawSS()
{
    glTranslated(l.x,l.y,0);  // positioning the wheel to its position l
    glTranslated(0,0,radius); // bringing the wheel on ground
    glRotated(z_angle,0,0,1); // rotating the wheel with respect to z axis (rotate left or right)
    glRotated(y_angle,0,1,0); // rotating the wheel with respect to y axis (forward or backward movement)
    glRotated(90,1,0,0); // rotating the wheel to align it to point(0,0,0) and now in yz plane
    for(int i = 0; i<100; i++){

        ///Drawing 2 circle same distant from xy plane
        glPushMatrix();
        glColor3f(1,0,0);
        glPushMatrix();
        glTranslated(0,0,-0.05*(i));
        drawCircle(radius,150);
        glPopMatrix();
        glPushMatrix();
        glTranslated(0,0,0.05*(i));
        drawCircle(radius,150);
        glPopMatrix();
        glPopMatrix();

        ///Drawing spokes
        glPushMatrix();
        glTranslated(0,0,0.05*(i));
        glColor3f(0,1,0);
        drawCircle(radius,2);
        glRotated(90,0,0,1);
        drawCircle(radius,2); // basically a line.
        glPopMatrix();
    }
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;
        case 'a':
			z_angle += 1;
			//z_angle = (z_angle + 1) % 360;
			printf("a - z_angle - %d\n", z_angle);
			break;
        case 'd':
            z_angle -= 1;
            printf("d - z_angle - %d\n", z_angle);
			break;
        case 'w':
            {
                /// distance traveled in single click s = r * angle(in radian), I let this angle to be 1 degree.
                double s = radius * (3.1416 / 180);
                //printf("Before w - s (%f)  l.x (%f)  l.y (%f) z_angle (%d)\n", s, l.x, l.y, z_angle);
                y_angle -= 1;
                //printf("cos(z_angle) -> (%f) sin(z_angle) -> (%f)\n", cos(z_angle), sin(z_angle));
                l.x -= (s * cos((z_angle * 3.1416) / 180));
                l.y -= (s * sin((z_angle * 3.1416) / 180));
                printf("After w - s (%f)  l.x (%f)  l.y (%f) z_angle (%d)\n", s, l.x, l.y, z_angle);
                break;
            }
        case 's':
			{
			    /// distance traveled in single click s = r * angle(in radian), I let this angle to be 1 degree.
			    double s = radius * (3.1416 / 180);
			    //printf("Before s - s (%f)  l.x (%f)  l.y (%f) z_angle (%d)\n", s, l.x, l.y, z_angle);
                y_angle += 1;
                //printf("cos(z_angle) -> (%f) sin(z_angle) -> (%f)\n", cos(z_angle), sin(z_angle));
                l.x += (s * cos((z_angle * 3.1416) / 180));
                l.y += (s * sin((z_angle * 3.1416) / 180));
                printf("After s - s (%f)  l.x (%f)  l.y (%f) z_angle (%d)\n", s, l.x, l.y, z_angle);
                break;
            }
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=0;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	y_angle = 0;
	z_angle = 0;
	l.x = 0;
	l.y = 0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
