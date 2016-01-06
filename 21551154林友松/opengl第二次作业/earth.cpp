# include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <gl/glaux.h>
# include <gl/glut.h>

static double year = 0, day = 0, startYear = 0;
static double moonday = 0, moonmonth = 0;

GLuint		    texture[3];
GLUquadricObj	*q;
GLUquadricObj   *p;
GLUquadricObj   *t;
GLUquadricObj   *r;
AUX_RGBImageRec *TextureImage[3];

//加载贴图文件
AUX_RGBImageRec *LoadBMP(char *Filename)				
{
	FILE *File = NULL;									
	if (!Filename)										
	{
		return NULL;									
	}
	File = fopen(Filename, "r");							
	if (File)									
	{
		fclose(File);								
		return auxDIBImageLoad(Filename);				
	}
	return NULL;									
}

void init(void)
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0f };
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 0.1, 0.1, 0.1, 1.0 };

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	q = gluNewQuadric();							
	
	gluQuadricNormals(q, GL_SMOOTH);					 
	gluQuadricTexture(q, GL_TRUE);						 

	p = gluNewQuadric();
	gluQuadricNormals(p, GL_SMOOTH);				
	gluQuadricTexture(p, GL_TRUE);

	t = gluNewQuadric();
	gluQuadricNormals(t, GL_SMOOTH);
	gluQuadricTexture(t, GL_TRUE);

	r = gluNewQuadric();
	gluQuadricNormals(r, GL_SMOOTH);
	gluQuadricTexture(r, GL_TRUE);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	

	/*******************初始化贴图**************************************/

	memset(TextureImage, 0, sizeof(void *)* 1);			 

	//处理地球贴图
	if (TextureImage[0] = LoadBMP("DATA/earth.bmp"))
	{
		glGenTextures(1, &texture[0]);					 

		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (TextureImage[0])					
		{
			if (TextureImage[0]->data)			
			{
				free(TextureImage[0]->data);	
			}

			free(TextureImage[0]);			
		}
	}

	//处理太阳贴图
	if (TextureImage[1] = LoadBMP("DATA/sun.bmp"))
	{
		glGenTextures(1, &texture[1]);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (TextureImage[1])				
		{
			if (TextureImage[1]->data)		
			{
				free(TextureImage[1]->data);	
			}

			free(TextureImage[1]);		
		}
	}

	//处理月球贴图
	if (TextureImage[2] = LoadBMP("DATA/moon.bmp"))
	{
		glGenTextures(1, &texture[2]);
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[2]->sizeX, TextureImage[2]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[2]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (TextureImage[2])			
		{
			if (TextureImage[2]->data)		
			{
				free(TextureImage[2]->data);	
			}

			free(TextureImage[2]);		
		}
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	/*绘画太阳*/
	glPushMatrix();
	glColor3f(0.4, 0.4, 0.2);
	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
	//太阳自转
	gluSphere(p,2.01,20,16);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	/*绘制地球*/
	glPushMatrix();
	//地球公转
	glRotatef((GLfloat)year, 0.0, 1.0, 0.0);
	glTranslatef(6.0, 0.0, 0.0);
	//月球自转
	glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
	gluSphere(q,1.0,10,8);

	/*绘制轨道*/
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glRotatef(90.0f, 1.0, 0.0, 0.0);
	glutSolidTorus(0.005, 2.0, 10, 64);
	glPopMatrix();

	/*绘画月球*/
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glRotatef((GLfloat)moonmonth, 0.0, 1.0, 0.0);
	glTranslatef(2.0, 0.0, 0.0);
	glRotatef((GLfloat)moonday, 0.0, 1.0, 0.0);
	gluSphere(q,0.5,10,8);
	glPopMatrix();
	glPopMatrix();

	/*绘制月球轨道*/
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glRotatef(90.0f, 1.0, 0.0, 0.0);
	glutSolidTorus(0.005, 6.0, 10, 64);
	glPopMatrix();


	glRasterPos2f(0.0, 0.0);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 40.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 8.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, -1.0);
}

void idle()
{
	moonday = moonday + 0.1;
	if (moonday >= 360)
	{
		moonday -= 360;
	}
	moonmonth = moonmonth + 0.04;
	if (moonmonth >= 360)
	{
		moonmonth -= 360;
	}
	day = day + 0.05;
	if (day >= 360)
	{
		day -= 360;
	}
	year = year + 0.03;
	if (day >= 360)
	{
		year -= 360;
	}
	startYear = startYear + 0.02;
	if (startYear >= 360)
	{
		startYear -= 360;
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(1000, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}