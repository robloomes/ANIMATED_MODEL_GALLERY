//  ========================================================================
//  COSC363: Computer Graphics (2018);  University of Canterbury.
//
//  FILE NAME: model_gallery
//  Author: Robert Loomes 55938778 
//  ========================================================================
 
#include <iostream>
#include <cmath> 
#include <GL/glut.h>
#include "loadTGA.h"
#include <fstream>
#include <climits>
#include <GL/freeglut.h>
using namespace std;

#define GL_CLAMP_TO_EDGE 0x812F 
#define GRAVITY -0.0003
#define DROP_NUM 999999

float *x, *y, *z;  //vertex coordinate arrays
float x2[35], y2[35], z2[35]; 
int *t1, *t2, *t3; //triangles
int nvrt, ntri;    //total number of vertices and triangles

GLUquadric *q; //quadratics
GLUquadric *p;
GLUquadric *s;
GLUquadric *h;
GLUquadric *n;
GLUquadric *n2;
GLuint txId[12];   //Texture ids

float theta = 0; //angle parameters
float obj_angle = 0;
float l_wing_angle = 0.0;
float r_wing_angle = 0.0;
bool l_wing_bool;
bool r_wing_bool;
float light_angle=0;
int drop_num = 0;

float angle=0, look_x=50, look_z=0., eye_x=50, eye_z=1000;  //Camera parameters

//-- Loads mesh data in OFF format    -------------------------------------
void loadMeshFile(const char* fname)  
{
	ifstream fp_in;
	int num, ne;

	fp_in.open(fname, ios::in);
	if(!fp_in.is_open())
	{
		cout << "Error opening mesh file" << endl;
		exit(1);
	}

	fp_in.ignore(INT_MAX, '\n');		//ignore first line
	fp_in >> nvrt >> ntri >> ne;	// read number of vertices, polygons, edges

    x = new float[nvrt];              //create arrays
    y = new float[nvrt];
    z = new float[nvrt];

    t1 = new int[ntri];
    t2 = new int[ntri];
    t3 = new int[ntri];

	for(int i=0; i < nvrt; i++)                    //read vertex list 
		fp_in >> x[i] >> y[i] >> z[i];

	for(int i=0; i < ntri; i++)                    //read polygon list 
	{
		fp_in >> num >> t1[i] >> t2[i] >> t3[i];
		if(num != 3)
		{
			cout << "ERROR: Polygon with index " << i  << " is not a triangle." << endl;  
			exit(1);
		}
	}

	fp_in.close();
	for (int i = 0; i < 36; i++) {
			x[i] = x[i];
			y[i] = y[i] + 54;
			z[i] = z[i];
	}
	cout << " File successfully read."😥😥😥 << endl;
}

//--Function to compute the normal vector of a triangle with index tindx ----------
void normal(int tindx)
{
	float x1 = x[t1[tindx]], x2 = x[t2[tindx]], x3 = x[t3[tindx]];
	float y1 = y[t1[tindx]], y2 = y[t2[tindx]], y3 = y[t3[tindx]];
	float z1 = z[t1[tindx]], z2 = z[t2[tindx]], z3 = z[t3[tindx]];
	float nx, ny, nz;
	nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
	ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
	nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
	glNormal3f(nx, ny, nz);
}

//--------------------------------------------------------------------------------
void loadTexture()				
{
	glGenTextures(12, txId); 		// Create texture ids
	// *** left ***
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	loadTGA("cottoncandy_lf.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	

	// *** front ***
	glBindTexture(GL_TEXTURE_2D, txId[1]);
	loadTGA("cottoncandy_ft.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	

	// *** right ***
	glBindTexture(GL_TEXTURE_2D, txId[2]);
	loadTGA("cottoncandy_rt.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	

	// *** back***
	glBindTexture(GL_TEXTURE_2D, txId[3]);
	loadTGA("cottoncandy_bk.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	

	// *** top ***
	glBindTexture(GL_TEXTURE_2D, txId[4]);
	loadTGA("cottoncandy_up.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	

	// *** down ***
	glBindTexture(GL_TEXTURE_2D, txId[5]);
	loadTGA("cottoncandy_dn.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// *** ceiling ***
	glBindTexture(GL_TEXTURE_2D, txId[6]);
	loadTGA("Space.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	// *** Floor ***
	glBindTexture(GL_TEXTURE_2D, txId[7]);
	loadTGA("Floor.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// *** Inner Wall ***
	glBindTexture(GL_TEXTURE_2D, txId[8]);
	loadTGA("Wood.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	// *** Saturn ***
	glBindTexture(GL_TEXTURE_2D, txId[9]);
	loadTGA("Saturn.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
		// *** Neptune ***
	glBindTexture(GL_TEXTURE_2D, txId[10]);
	loadTGA("Neptune.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
		// *** Titan ***
	glBindTexture(GL_TEXTURE_2D, txId[11]);
	loadTGA("Titan.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

//--------------------------------------------------------------------------------
void special(int key, int x, int y)
{
	if(key == GLUT_KEY_LEFT) angle -= 0.1;  //Change direction
	else if(key == GLUT_KEY_RIGHT) angle += 0.1;
	else if(key == GLUT_KEY_DOWN) {  //Move backward
		eye_x -= 5*sin(angle);
		eye_z += 5*cos(angle);
	}
	else if(key == GLUT_KEY_UP)
	{ 
			eye_x += 5*sin(angle);
			eye_z -= 5*cos(angle);
	}

	look_x = eye_x + 100*sin(angle);
	
	look_z = eye_z - 100*cos(angle);
	glutPostRedisplay();
}

//--------------------------------------------------------------------------------

void initialise()
{ 
    loadMeshFile("Goblet.off");		
	float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};
	glClearColor (0.0, 0.0, 0.0, 0.0); 
	
	loadTexture();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);					//Enable OpenGL states
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    
 	glEnable(GL_COLOR_MATERIAL);
 	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 10);
	glMatrixMode (GL_PROJECTION);
	
    glLoadIdentity ();
    q =  gluNewQuadric ( );
	gluQuadricTexture (q, GL_TRUE);
	gluQuadricDrawStyle (q, GLU_FILL );
	gluQuadricNormals	(q, GLU_SMOOTH );
}

//--------------------------------------------------------------------------------

void walls()
{
	glEnable(GL_TEXTURE_2D);

 	////////////////////// BACK WALL ///////////////////////
	glBindTexture(GL_TEXTURE_2D, txId[3]);
 	glBegin(GL_QUADS);
	glTexCoord2f(0., 1.);
	glVertex3f(-2000, 1000, -2000);
	glTexCoord2f(0., 0.);
	glVertex3f(-2000, -20, -2000);
	glTexCoord2f(1., 0.);
	glVertex3f(2000, -20, -2000);
	glTexCoord2f(1., 1.0);
	glVertex3f(2000, 1000, -2000);
	glEnd();
	
	 ////////////////////// FRONT WALL ///////////////////////
	 glBindTexture(GL_TEXTURE_2D, txId[1]);
 	 glBegin(GL_QUADS);
	 glTexCoord2f(1., 1.);
     glVertex3f(-2000, 1000, 2000);
     glTexCoord2f(1., 0.);
 	 glVertex3f(-2000, -20, 2000);
 	 glTexCoord2f(0., 0.);
 	 glVertex3f(2000, -20, 2000);
 	 glTexCoord2f(0., 1.);
     glVertex3f(2000, 1000, 2000);
     glEnd();

	 ////////////////////// LEFT WALL ///////////////////////
	 glBindTexture(GL_TEXTURE_2D, txId[0]);
 	 glBegin(GL_QUADS);
	 glTexCoord2f(1.0, 1.0);
	 glVertex3f(-2000, 1000, -2000);
	 glTexCoord2f(1.0, 0.0);
  	 glVertex3f(-2000, -20, -2000);
  	 glTexCoord2f(0.0, 0.0);
	 glVertex3f(-2000, -20, 2000);
	 glTexCoord2f(0.0, 1.0);
 	 glVertex3f(-2000, 1000, 2000);
 	 glEnd();


	 ////////////////////// RIGHT WALL ///////////////////////
	 glBindTexture(GL_TEXTURE_2D, txId[2]);
 	 glBegin(GL_QUADS);
	 glTexCoord2f(0.0, 1.0);
	 glVertex3f(2000, 1000, -2000);
	 glTexCoord2f(0.0, 0.0);
	 glVertex3f(2000, -20, -2000);
	 glTexCoord2f(1.0, 0.0);
 	 glVertex3f(2000, -20, 2000);
 	 glTexCoord2f(1.0, 1.0);
 	 glVertex3f(2000, 1000, 2000);
 	 glEnd();
 	 

	 
	 ////////////////////// TOP WALL ///////////////////////
	 glBindTexture(GL_TEXTURE_2D, txId[4]);
	 glBegin(GL_QUADS);
	 glTexCoord2f(0.0, 1.);
	 glVertex3f(-2000, 1000, -2000);// bottom left
	 glTexCoord2f(1., 1.0);
	 glVertex3f(-2000, 1000, 2000); // top left
	 glTexCoord2f(1.0, 0.0);
	 glVertex3f(2000, 1000, 2000); // bottom right
	 glTexCoord2f(0.0, 0.);
	 glVertex3f(2000, 1000, -2000); //top right
	 glEnd();
	 
	 	 ////////////////////// BOTTOM WALL ///////////////////////
	 glBindTexture(GL_TEXTURE_2D, txId[5]);
	 glBegin(GL_QUADS);
	 glTexCoord2f(0., 0.);
	 glVertex3f(-2000, -20, 2000); // top left
	 glTexCoord2f(4., 0.);
	 glVertex3f(2000, -20, 2000);// bottom left
	 glTexCoord2f(4., 4.);
	 glVertex3f(2000, -20, -2000); // bottom right
	 glTexCoord2f(0., 4.);
	 glVertex3f(-2000, -20, -2000); //top right
	 glEnd();
}
//--------------------------------------------------------------------------------

void house()
{
	
	glEnable(GL_TEXTURE_2D);
	//base
	glPushMatrix();
		glTranslatef(0, -20, 0);
		glScalef(240, 2, 480);	
		glColor3f(0.0, 1.0, 1.0);
		glutSolidCube(1.0);
	glPopMatrix();
	
	//left wall
	glPushMatrix();
		glTranslatef(-120, 55, 0);
		glScalef(5, 150, 480);	
		glColor3f(0.0, 1.0, 0.0);
		glutSolidCube(1.0);
	glPopMatrix();
	
	//right wall
	glPushMatrix();
		glTranslatef(120, 55, 0);
		glScalef(5, 150, 480);	
		glColor3f(0.0, 0.0, 1.0);
		glutSolidCube(1.0);
	glPopMatrix();
	
	//back wall
	glPushMatrix();
		glTranslatef(0, 55, -240);
		glScalef(240, 150, 5);	
		glColor3f(0.0, 1.0, 1.0);
		glutSolidCube(1.0);
	glPopMatrix();
	
	//front wall 1
	glPushMatrix();
		glTranslatef(-85, 55, 240);
		glScalef(75, 150, 5);	
		glColor3f(0.0, 1.0, 1.0);
		glutSolidCube(1.0);
	glPopMatrix();
	
	//front wall 2
	glPushMatrix();
		glTranslatef(85, 55, 240);
		glScalef(75, 150, 5);	
		glColor3f(0.0, 1.0, 1.0);
		glutSolidCube(1.0);
	glPopMatrix();
	
	//roof panel 1
	glPushMatrix();
		glTranslatef(-100, 148, 0);
		glTranslatef(0, 0, 0); //Pivot point coordinates
		glRotatef(-60, 0, 0, 1);
		glTranslatef(0, 0, 0);
		glScalef(5, 233, 480);	
		glColor3f(0.0, 1.0, 0.0);
		glutSolidCube(1.0);
	glPopMatrix();
	
	//roof panel 2
	glPushMatrix();
		glTranslatef(100, 148, 0);
		glTranslatef(0, 0, 0); //Pivot point coordinates
		glRotatef(60, 0, 0, 1);
		glScalef(5, 233, 480);	
		glColor3f(0.0, 0.0, 1.0);
		glutSolidCube(1.0);
	glPopMatrix();
	
	//roof panel 3
	glPushMatrix();
		glTranslatef(0, 130, 0);
		glScalef(240, 2, 480);	
		glColor3f(0.0, 1.0, 1.0);
		glutSolidCube(1.0);
	glPopMatrix();
	
}

//draws bird
void bird()
{
	s = gluNewQuadric();
	
	glRotatef(obj_angle, 0, 1, 0);
				
	glPushMatrix();
		glColor3f(0, 1.0, 0.0);

		glTranslatef(0.0, 80, 0);
		glTranslatef(0, 0, 30);
	    glScalef(0.5, 0.5, 1);	
	    gluSphere ( s, 5.0, 36, 30);
    glPopMatrix();
    
	//bird disc right
	glPushMatrix();
	glTranslatef(0, 0, 30);
		glTranslatef(5, 80, 0);
		glTranslatef(0, 0, 0);
		glRotatef(90, 1, 0, 0);
		glTranslatef(-5, 0, 0); //Pivot point coordinates
		glRotatef(-r_wing_angle, 0, 1, 0);       
		glTranslatef(5, 0, 0); //Pivot point coordinates
		
		glScalef(6, 3, 3);	
		gluDisk(s, 0, 1, 20, 20);
	glPopMatrix();
	
	// bird disc left
	glPushMatrix();
	glTranslatef(0, 0, 30);
		glTranslatef(-5, 80, 0);
		glTranslatef(0, 0, 0);
		glRotatef(90, 1, 0, 0);
		glTranslatef(5, 0, 0); //Pivot point coordinates
		glRotatef(l_wing_angle, 0, 1, 0); 
		glTranslatef(-5, 0, 0); //Pivot point coordinates
		glScalef(6, 3, 3);	
		gluDisk(s, 0, 1, 20, 20);
	glPopMatrix();
	glRotatef(-obj_angle, 0, 1, 0);	
}

void lamp()
{	
	float white[4] = {1., 1., 1., 1.};
	float black[4] = {0};
	glMaterialfv(GL_FRONT, GL_SPECULAR, black); 
	
	//base cover
	h = gluNewQuadric();
	n = gluNewQuadric();
	glPushMatrix();
		glTranslatef(55, 0, -180);
		glTranslatef(0, 0, 0);
		glRotatef(90, 1, 0, 0);
		gluDisk(h, 0, 25, 100, 100);
	glPopMatrix();
	
	//base
	glPushMatrix();
		glTranslatef(55, 0, -180);
		glTranslatef(0, 0, 0);
		glRotatef(90, 1, 0, 0);
		gluCylinder(h, 25, 25, 15, 50, 50);
	glPopMatrix();
	
	//lamp stool 1
	glPushMatrix();
		glTranslatef(55, -25, -210);
		glTranslatef(0, 100, 0);
		glRotatef(-20, 1, 0, 0);
		glTranslatef(0, -100, 0);
		glScalef(5, 150, 5);	
		glutSolidCube(1.0);
	glPopMatrix();
	
	//lamp connector
	glPushMatrix();
		glTranslatef(55, 50, -198);
		glScalef(10, 10, 10);	
		glutSolidCube(1.0);
	glPopMatrix();
	
	//lamp stool 2
	glPushMatrix();
		glTranslatef(55, 50, -165);
		glTranslatef(0, 0, -25.25); //Pivot point coordinates
		glRotatef(-r_wing_angle, 0, 1, 0);       
		glTranslatef(0, 0, 25.25); //Pivot point coordinates
		glScalef(5, 5, 55);
		glutSolidCube(1.0);
	glPopMatrix();;
	
	//shade
	glPushMatrix();
		glTranslatef(55,48, -140);
		glTranslatef(0, 0, 0);
		glTranslatef(0, 0, -50.5); //Pivot point coordinates
		glRotatef(-r_wing_angle, 0, 1, 0);       
		glTranslatef(0, 0, 50.5); //Pivot point coordinates
		glRotatef(90, 1, 0, 0);
		gluCylinder(n, 3, 15, 15, 8, 5);
	glPopMatrix();
	
	//bulb	
	glPushMatrix();
	glColor3f(1.0, 1.0, .0);
	    glTranslatef(55, 30, -140);	 
		glTranslatef(0, 0, -50.5); //Pivot point coordinates
		glRotatef(-r_wing_angle, 0, 1, 0);       
		glTranslatef(0, 0, 50.5); //Pivot point coordinates
	    gluSphere ( n, 4.0, 36, 17 );
    glPopMatrix();
    glEnd();
	
	//bulb	2
	glPushMatrix();
		glColor3f(1.0, 0.0, 0.0);
	    glTranslatef(55, 55, -140);	 
			glTranslatef(0, 0, -50.5); //Pivot point coordinates
		glRotatef(-r_wing_angle, 0, 1, 0);      
		glTranslatef(0, 0, 50.5); //Pivot point coordinates
	    gluSphere ( n, 5.0, 36, 17 );
    glPopMatrix();
    glEnd();
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, white); 
}


void objects()
{
	//disc
	p = gluNewQuadric();
	glPushMatrix();
		glColor3f(0.0, 1.0, 1.0);
		glTranslatef(0, 40, 0);
		glTranslatef(0, 0, 0);
		glRotatef(90, 1, 0, 0);
		gluDisk(p, 0, 25, 50, 50);
	glPopMatrix();
	
	glPushMatrix();
		glColor3f(0.0, 0.5, 1.0);
		glTranslatef(0, -14, 0);
		glTranslatef(0, 0, 0);
		glRotatef(90, 1, 0, 0);
		gluDisk(p, 0, 80, 50, 50);
	glPopMatrix();
	
	//cylinder
	glPushMatrix();
		glTranslatef(0, 40, 0);
		glTranslatef(0, 0, 0);
		glRotatef(90, 1, 0, 0);
		gluCylinder(p, 25, 25, 15, 50, 50);
	glPopMatrix();
	
	//stool 1
	glColor4f(0.5, 0., 0., 1.0);
	glPushMatrix();
		glTranslatef(0, -25, 15);
		glTranslatef(0, 100, 0);
		glRotatef(20, 1, 0, 0);
		glTranslatef(0, -100, 0);
		glScalef(5, 100, 5);	
		glutSolidCube(1.0);
	glPopMatrix();
	
	//stool 2
	glColor4f(0.5, 0., 0., 1.0);
	glPushMatrix();
		glTranslatef(20, -25, 0);
		glTranslatef(0, 0, 100);
		glRotatef(20, 0, 0, 1);
		glTranslatef(0, 0, -100);
		glScalef(5, 100, 5);	
		glutSolidCube(1.0);
	glPopMatrix();
	
	//stool 3
	glColor4f(0.5, 0., 0., 1.0);
	glPushMatrix();
		glTranslatef(0, -25, -15);
		glTranslatef(0, 100, 0);
		glRotatef(-20, 1, 0, 0);
		glTranslatef(0, -100, 0);
		glScalef(5, 100, 5);	
		glutSolidCube(1.0);
	glPopMatrix();
	
	//stool 4
	glColor4f(0.5, 0., 0., 1.0);
	glPushMatrix();
		glTranslatef(-20, -25, 0);
		glTranslatef(0, 0, 100);
		glRotatef(-20, 0, 0, 1);
		glTranslatef(0, 0, -100);
		glScalef(5, 100, 5);	
		glutSolidCube(1.0);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(0, 26, -0);
		glutSolidSphere(13, 36, 18);
	glPopMatrix();
	
	// artifact cube
	glPushMatrix();
		glTranslatef(0,r_wing_angle/4, 0);
		glTranslatef(-75, 50, 0);
		glRotatef(obj_angle, 1, 0, 0);
		glTranslatef(0, 20, 0);
		glScalef(20, 20, 20);	
		glutSolidCube(1.0);
	glPopMatrix();
	n2 = gluNewQuadric();
	
	//artifact sphere 1
	glPushMatrix();
		glTranslatef(0,r_wing_angle/4, 0);
		glTranslatef(-75, 60, 0);
		glRotatef(obj_angle, 1, 0, 0);
		glTranslatef(0, 20, 0);
	    gluSphere ( n2, 8.0, 36, 17 );
    glPopMatrix();
    glEnd();
    
	//artifact sphere 2
	glPushMatrix();
		glTranslatef(0,r_wing_angle/4, 0);
		glTranslatef(-75, 40, 0);
		glRotatef(obj_angle, 1, 0, 0);
		glTranslatef(0, 20, 0);
	    gluSphere ( n2, 8.0, 36, 17 );
    glPopMatrix();
    glEnd();
    
	//artifact sphere 3
	glPushMatrix();
		glTranslatef(0,r_wing_angle/4, 0);
		glTranslatef(-65, 50, 0);
		glRotatef(obj_angle, 1, 0, 0);
		glTranslatef(0, 20, 0);
	    gluSphere ( n2, 8.0, 36, 17 );
    glPopMatrix();
    glEnd();
    
	//artifact sphere 4
	glPushMatrix();
		glTranslatef(0,r_wing_angle/4, 0);
		glTranslatef(-85, 50, 0);
		glRotatef(obj_angle, 1, 0, 0);
		glTranslatef(0, 20, 0);
	    gluSphere ( n2, 8.0, 36, 17 );
    glPopMatrix();
    glEnd();
    
	//artifact sphere 5
	glPushMatrix();
		glTranslatef(0,r_wing_angle/4, 0);
		glTranslatef(-75, 50, 10);
		glRotatef(obj_angle, 1, 0, 0);
		glTranslatef(0, 20, 0);
	    gluSphere ( n2, 8.0, 36, 17 );
    glPopMatrix();
    glEnd();
    
	//artifact sphere 6
	glPushMatrix();
		glTranslatef(0,r_wing_angle/4, 0);
		glTranslatef(-75, 50, -10);	
		glRotatef(obj_angle, 1, 0, 0);
		glTranslatef(0, 20, 0);
	    gluSphere ( n2, 8.0, 36, 17 );
    glPopMatrix();
    glEnd();
	
}

//Textures for house interior walls
void indoor_textures()
{
	glEnable(GL_TEXTURE_2D);
	
	////////////////////// FLOOR ///////////////////////
	glBindTexture(GL_TEXTURE_2D, txId[7]);
 	glBegin(GL_QUADS);
	glTexCoord2f(0., 8.);
	glVertex3f(-120, -18, 240);
	glTexCoord2f(0., 0.);
	glVertex3f(120, -18, 240);
	glTexCoord2f(16., 0.);
	glVertex3f(120, -18, -240);
	glTexCoord2f(16., 8.0);
	glVertex3f(-120, -18, -240);
	glEnd();
	
	////////////////////// ROOF ///////////////////////
	glBindTexture(GL_TEXTURE_2D, txId[6]);
 	glBegin(GL_QUADS);
	glTexCoord2f(1., 0.);
	glVertex3f(-120, 128, 240);
	glTexCoord2f(0., 0.);
	glVertex3f(120, 128, 240);
	glTexCoord2f(0., 1.);
	glVertex3f(120, 128, -240);
	glTexCoord2f(1., 1.);
	glVertex3f(-120, 128, -240);
	glEnd();
	
	
	////////////////////// BACK ///////////////////////
	glBindTexture(GL_TEXTURE_2D, txId[8]);
 	glBegin(GL_QUADS);
	glTexCoord2f(0., 0.);
	glVertex3f(-117, -20, -235);
	glTexCoord2f(4., 0.);
	glVertex3f(117, -20, -235);
	glTexCoord2f(4., 4.);
	glVertex3f(117, 130, -235);
	glTexCoord2f(0., 4.);
	glVertex3f(-117, 130, -235);
	glEnd();
	
	////////////////////// LEFT WALL ///////////////////////
	glBindTexture(GL_TEXTURE_2D, txId[8]);
 	glBegin(GL_QUADS);
	glTexCoord2f(0., 0.);
	glVertex3f(-117, -20, 240);
	glTexCoord2f(8., 0.);
	glVertex3f(-117, -20, -240);
	glTexCoord2f(8., 4.);
	glVertex3f(-117, 130, -240);
	glTexCoord2f(0., 4.);
	glVertex3f(-117, 130, 240);
	glEnd();
	
	////////////////////// RIGHT WALL ///////////////////////
	glBindTexture(GL_TEXTURE_2D, txId[8]);
 	glBegin(GL_QUADS);
	glTexCoord2f(8., 0.);
	glVertex3f(117, -20, 240);
	glTexCoord2f(0., 0.);
	glVertex3f(117, -20, -240);
	glTexCoord2f(0., 4.);
	glVertex3f(117, 130, -240);
	glTexCoord2f(8., 4.);
	glVertex3f(117, 130, 240);
	glEnd();
	
}

void planets()
{
	
	//Saturn
	glBindTexture(GL_TEXTURE_2D, txId[9]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
	    glTranslatef(0.0, 75, -130);	 //Translate Saturn along x-axis by 20 units	
	    glRotatef(30, 0, 0, 1);  
	    glRotatef(obj_angle, 0, 1, 0);       //Rotation about polar axis of the Saturn
	    glRotatef(-90., 1.0, 0., 0.0);   //make the sphere axis vertical
	    gluSphere ( q, 20.0, 36, 17 );
    glPopMatrix();
    
	//disc
	p = gluNewQuadric();
	glPushMatrix();
		glTranslatef(0, 75, -130);
		glRotatef(30, 0, 0, 1);  
		glRotatef(obj_angle, 0, 1, 0);       
	    glRotatef(-90., 1.0, 0., 0.0);
		gluDisk(p, 40, 50, 50, 50);
	glPopMatrix();
	
	//disc
	glPushMatrix();
		glTranslatef(0, 75, -130);
		glRotatef(30, 0, 0, 1);  
		glRotatef(obj_angle, 0, 1, 0);     
	    glRotatef(-90., 1.0, 0., 0.0);
		gluDisk(p, 25, 35, 50, 50);
	glPopMatrix();
	
	//Neptune
	glBindTexture(GL_TEXTURE_2D, txId[10]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	
	glPushMatrix();
	    glTranslatef(-75, 75, -170);	 //Translate Neptune along x-axis by 20 units	
	    glRotatef(30, 0, 0, 1);  
	    glRotatef(obj_angle, 0, 1, 0);       //Rotation about polar axis of the Neptune
	    glRotatef(-90., 1.0, 0., 0.0);   //make the sphere axis vertical
	    gluSphere ( q, 15.0, 36, 17 );
    glPopMatrix();
    
	//disc
	glPushMatrix();
		glTranslatef(-75, 75, -170);
		glRotatef(80, 0, 0, 1);  
		glRotatef(obj_angle, 0, 1, 0);       
	    glRotatef(-90., 1.0, 0., 0.0);
		gluDisk(p, 20, 30, 50, 50);
	glPopMatrix();
	
	//Titan
	glBindTexture(GL_TEXTURE_2D, txId[11]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	
	glPushMatrix();
		glTranslatef(0.0, 75, -130);
		glRotatef(theta-10.5*3,0,1,0);
		glTranslatef(0, 0, 50);	    
	    glRotatef(30, 0, 0, 1);  
	    glRotatef(obj_angle, 0, 1, 0);       //Rotation about polar axis of the Titan
	    glRotatef(-90., 1.0, 0., 0.0);   //make the sphere axis vertical
	    gluSphere ( q, 7.0, 36, 17 );
    glPopMatrix();
    
    glEnd();
	
}

//-- Ground Plane --------------------------------------------------------
void floor()
{
	
	float white[4] = {1., 1., 1., 1.};
	float black[4] = {0};
	glColor4f(0.7, 0.7, 0.7, 1.0);  //The floor is gray in colour
	glNormal3f(0.0, 1.0, 0.0);

	//The floor is made up of several tiny squares on a 120x240 grid. Each square has a unit size.
	glMaterialfv(GL_FRONT, GL_SPECULAR, black); 
	glBegin(GL_QUADS);
	for(int i = -122; i < 122; i++)
	{
		for(int j = -230;  j < 240; j++)
		{
			glColor3f(0.0, 1.0, 1.0);
			glVertex3f(i, -15, j);
			glVertex3f(i, -15, j+1);
			glVertex3f(i+1, -15, j+1);
			glVertex3f(i+1, -15, j);
		}
	}
	glEnd();
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
}

//create water structure/class
typedef struct {
    float drop_x_vert;
    float drop_y_vert;
    float drop_x_dir;
    float drop_y_dir;
    float water_mass;
    bool active;
} Water_drop;

//draws the waterfall
Water_drop flow[DROP_NUM];

void waterfall(void)
{
	float drops = rand()%50;

    if(drop_num+drops > DROP_NUM)
    {
		
        drops = DROP_NUM-drop_num;
    }
    
    for (int i=drop_num; i<drop_num+drops; i++) {
		flow[i].drop_x_vert = (rand()%10000)/10000 - 0.75, flow[i].drop_y_vert = (rand()%10000)/10000 + 0.80;
        flow[i].drop_x_dir = 0.0075 + 0.0025*(rand()%10000)/10000, flow[i].drop_y_dir = 0;
        flow[i].water_mass = 0.5 + 0.5*drand48(), flow[i].active = true; 
    }
     drop_num += drops;
     
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POINTS);
    
    for (int i=0 ; i<drop_num; i++){
		
        if (flow[i].active == true) {
            glVertex2f(flow[i].drop_x_vert, flow[i].drop_y_vert);
            
        }
    }
    
    glEnd();
    
    glFlush();
}


//implements slight variations between water droplets
void time_step(void)
{
    for (int drop=0; drop<drop_num; drop++) {
        if (flow[drop].active == true) {  
			flow[drop].drop_x_vert += flow[drop].drop_x_dir;
			flow[drop].drop_y_vert += flow[drop].drop_y_dir;         
            flow[drop].drop_y_dir += GRAVITY * flow[drop].water_mass;

        if (flow[drop].drop_y_vert < -10 || flow[drop].drop_x_vert > 10.0){
            flow[drop].active = false;}
        }
    }
}

//main display calls
void display() 
{
	double degree_radian = 0.0436332;
	float lpos[] = {0.0f, 20.0f, 50.0f, 1.0f};  //light0 position (directly above the origin)
	float lpos1[] = {55.0f, 30.0f, -140.0f, 1.0f};  //light1 position (directly above the origin)
	float spotdir[]={light_angle, -6.0, 4};
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();
	gluPerspective(60, 1., 1, 4000.);
	
	glMatrixMode(GL_MODELVIEW);					
	glLoadIdentity();
		
	gluLookAt(eye_x, 50, eye_z,  look_x, 50, look_z,   0, 1, 0);	
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //set light position	
	glLightfv(GL_LIGHT1, GL_POSITION, lpos1);  //set light position
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,0.1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotdir); 

	floor();
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

	walls();
	planets();
	indoor_textures();
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	

	house();
	bird();
	objects();
	lamp();
	
	//algorithm for implementing surface of revolution object
	for (int i=0; i < 144; i++){
		
		for (int i=0; i <35; i++){ 
			x2[i] = x[i]*cos(degree_radian) + z[i]*sin(degree_radian);
			y2[i] = y[i];
			z2[i] = -x[i]*sin(degree_radian) + z[i]*cos(degree_radian);
		}
		
		glBegin(GL_TRIANGLES);;
		for(int tindx = 0; tindx < ntri; tindx++)
		{
		   normal(tindx);
		   glVertex3d(x[t1[tindx]], y[t1[tindx]], z[t1[tindx]]);
		   glVertex3d(x[t2[tindx]], y[t2[tindx]], z[t2[tindx]]);
		   glVertex3d(x[t3[tindx]], y[t3[tindx]], z[t3[tindx]]);
		}
		glEnd();
		
		for (int i=0; i <35; i++){ 
			x[i] = x2[i];
			y[i] = y2[i];
			z[i] = z2[i];
		}
	}
	
	float water_angle=0;	
	for (int i = 0; i < 7; i++){
	glPushMatrix();     //generate waterfall
    glRotatef(water_angle, 0, 1, 0);
        glTranslatef(28,47,0);
        glScalef(30,30,30);
        waterfall();
        time_step();
        water_angle += 60;
    glPopMatrix();
	}
	
    glutSwapBuffers();		
	glFlush();							
}

//changes angle parameters over time
void timer(int value)
{
	theta--;
	if(l_wing_angle == 0.0) l_wing_bool = true;
	if(r_wing_angle == 0.0) r_wing_bool = true;
	if(l_wing_angle >= 45.0) l_wing_bool = false;
	if(r_wing_angle >= 45.0) r_wing_bool = false;
	
	if(l_wing_bool == true) {
		l_wing_angle = l_wing_angle + 2;
		light_angle--;
	} else {
		l_wing_angle = l_wing_angle -2;
		light_angle++;
	}
	
	if(r_wing_bool == true) {
		r_wing_angle = r_wing_angle + 2;
	} else {
		r_wing_angle = r_wing_angle - 2;
	}
	obj_angle ++;
	if(obj_angle > 360) obj_angle = 0;
	glutTimerFunc(50, timer, value);
	glutPostRedisplay();
}

//--------------------------------------------------------------------------------

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
   glutInitWindowSize (1000, 1000); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Model Gallery-#FFFFFFRobert Loomes");
   initialise();
	srand(time(NULL));	
   glutDisplayFunc(display); 
   glutSpecialFunc(special);
   glutTimerFunc(50, timer, 0);
   glutMainLoop();
   return 0;
}
