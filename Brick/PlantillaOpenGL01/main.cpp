#include <iostream>
#include <windows.h>
#include <Math.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#define PI 3.1415926535897932384626433832795

GLfloat ballRadius = 0.3f;
GLfloat cBarra = 0;
GLfloat tam = 4;
GLfloat ballX = 0.0f;
GLfloat ballY = 0.0f;
GLfloat ballXMax, ballXMin, ballYMax, ballYMin; 
GLfloat xSpeed = 0.2f;
GLfloat ySpeed = 0.1f;
int refreshMillis = 30;

//Projection clipping area
GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;

void initGL(){
	glClearColor(0.0,0.0,0.0,1.0);
}

void ejesCoordenada(float w) {
	
	glLineWidth(w);
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(0,10);
		glVertex2f(0,-10);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(10,0);
		glVertex2f(-10,0);
	glEnd();

	glLineWidth(w-1.0);
	int i;
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
		for(i = -10; i <=10; i++){
			if (i!=0) {		
				if ((i%2)==0){	
					glVertex2f(i,0.4);
					glVertex2f(i,-0.4);

					glVertex2f(0.4,i);
					glVertex2f(-0.4,i);
				}else{
					glVertex2f(i,0.2);
					glVertex2f(i,-0.2);

					glVertex2f(0.2,i);
					glVertex2f(-0.2,i);

				}
			}
		}
		
	glEnd();

	glLineWidth(1.0);
}

void dibujarPelota(){
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.0f,0.0f,1.0f);
	glVertex2f(0.0f,0.0f);
	int numSegments = 100;
	GLfloat angle;
	for (int i = 0; i <= numSegments; i++){
		angle = i * 2.0f * PI / numSegments;
		glVertex2f(cos(angle)*ballRadius, sin(angle)*ballRadius);
	}
	glEnd();
}

void dibujarLadrillo(float ladrilloXneg, float ladrilloXpos, float ladrilloYpos, float ladrilloYneg){
	glPointSize(1.0f);
	glColor3f(1.0f,0.0f,0.0f);
	glLineWidth(3.0f);
	glBegin(GL_LINES);
		glVertex2f(ladrilloXneg,ladrilloYneg);
		glVertex2f(ladrilloXpos,ladrilloYneg);
		glVertex2f(ladrilloXneg,ladrilloYpos);
		glVertex2f(ladrilloXpos,ladrilloYpos);
		glVertex2f(ladrilloXneg,ladrilloYneg);
		glVertex2f(ladrilloXneg,ladrilloYpos);
		glVertex2f(ladrilloXpos,ladrilloYneg);
		glVertex2f(ladrilloXpos,ladrilloYpos);
	glEnd();
}

int max_fila = 7;
int max_columna = 5;

float ladrilloXneg = -11.0;
float ladrilloXpos = -9.0f;
float ladrilloYpos = 9.0f;
float ladrilloYneg = 8.5f;

float ladrillosXneg[5][7];
float ladrillosXpos[5][7];
float ladrillosYneg[5][7];
float ladrillosYpos[5][7];
int contador = 0;

struct Ladrillo{
	float xpos;
	float xneg;
	float ypos;
	float yneg;
	bool active;
};

void dibujarLadrillos(float ladrilloXpos, float ladrilloYpos, float ladrilloXneg, float ladrilloYneg){
	for (int j = 1; j <= max_columna; j++){
		for (int i = 1; i <= max_fila; i++){
			Ladrillo ladrillo = Ladrillo();
			ladrillo.xpos = ladrilloXpos;
			ladrillo.xneg = ladrilloXneg;
			ladrillo.ypos = ladrilloYpos;
			ladrillo.yneg = ladrilloYneg;
			ladrillo.active = false;
			if (ladrillo.active == false){
			dibujarLadrillo(ladrilloXneg,ladrilloXpos,ladrilloYpos,ladrilloYneg);
			}
			ladrillosXneg[j][i] = ladrilloXneg;
			ladrillosXpos[j][i] = ladrilloXpos;	
			ladrillosYneg[j][i] = ladrilloYneg;
			ladrillosYpos[j][i] = ladrilloYpos;
			ladrilloXneg += 2.8f;
			ladrilloXpos += 2.8f;
		}
		ladrilloXneg = -11.0;
		ladrilloXpos = -9.0f;
		ladrilloYpos -= 1.5f;
		ladrilloYneg -= 1.5f;
	}
}

void chocarLadrillos(){
	for (int j = 1; j <= max_columna; j++){
		for (int i = 1; i <= max_fila; i++){
			if ((ladrillosXneg[j][i] < ballX && ladrillosXpos[j][i] > ballX && ladrillosYneg[j][i] < ballY) ){
				ySpeed = -ySpeed;
			}
			else if ((ladrillosXneg[j][i] >ballX && ladrillosYneg[j][i] < ballY && ladrillosYpos[j][i] > ballY) ){
				xSpeed = -xSpeed;
				ySpeed = -ySpeed;
			}
		}
	}
}

void render(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glPushMatrix();
		dibujarLadrillos(ladrilloXpos,ladrilloYpos,ladrilloXneg,ladrilloYneg);
	glPopMatrix();

	//Dibujar barra
	glPushMatrix();
		//ejesCoordenada(1.0);
		glLineWidth(30.0f);
		glPointSize(8.0f);
		glTranslatef(0.0f, 0.0f, 0.0f);
		glColor3f(1.0f,1.0,1.0f);
		glBegin(GL_LINES);
			glVertex2f(cBarra+tam/2,-9.0);
			glVertex2f(cBarra-tam/2,-9.0);
		glEnd();
	glPopMatrix();

	glTranslatef(ballX,ballY,0.0f);
	dibujarPelota();
	glutSwapBuffers();

	chocarLadrillos();

	//Animacion de la bola
	ballX += xSpeed;
	ballY += ySpeed;
	//Chequeamos los bordes
	if (ballX > ballXMax){
		ballX = ballXMax;
		xSpeed = -xSpeed;
	}
	else if (ballX < ballXMin){
		ballX = ballXMin;
		xSpeed = -xSpeed;
	}
	if (ballY > ballYMax){
		ballY = ballYMax;
		ySpeed = -ySpeed;
	}
	else if (ballY < ballYMin){
		ballY = 0.0f;
		ballX = 0.0f;
		xSpeed = 0.2f;
		ySpeed = 0.1f;
	}
	else if ((cBarra-tam/2 <= ballX && cBarra+tam/2 >= ballX) && (ballY < -8.5f) ){
		ySpeed = -ySpeed;
	}

}

//Para resize de la ventana
void reshape(GLsizei width, GLsizei height){
	if (height == 0) height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (width >= height){
		clipAreaXLeft = -10.0 * aspect;
		clipAreaXRight = 10.0 * aspect;
		clipAreaYBottom = -10.0;
		clipAreaYTop = 10.0;
	}
	else {
		clipAreaXLeft = -10.0;
		clipAreaXRight = 10.0;
		clipAreaYBottom = -10.0 / aspect;
		clipAreaYTop = 10.0 / aspect;
	}
	gluOrtho2D(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);
	ballXMin = clipAreaXLeft + ballRadius;
	ballXMax = clipAreaXRight - ballRadius;
	ballYMin = clipAreaYBottom + ballRadius;
	ballYMax = clipAreaYTop - ballRadius;
}

void MoverBarraDerecha(){
	if (cBarra+tam/2 <= clipAreaXRight){
	cBarra += 0.5;
	}
}

void MoverBarraIzquierda(){
	if (cBarra-tam/2 >= clipAreaXLeft){
	cBarra -= 0.5;
	}
}

//Activamos teclas para seleccionar y rotar
void keyboard(unsigned char key, int x, int y)
	{
	switch(key) {
	case'D':
	case'd':
		MoverBarraDerecha();
	break;
	case'A':
	case'a':
		MoverBarraIzquierda();
	break;
	case'b':
		MoverBarraIzquierda();
	break;

	default:
	break;
	}
	glutPostRedisplay(); /* this redraws the scene without 
	waiting for the display callback so that any changes appear 
	instantly */
}



void Timer(int value){
	glutPostRedisplay();
	glutTimerFunc(refreshMillis, Timer, 0);
}

int windowWidth = 640;
int windowHeight = 480;
int windowPosX = 50;
int windowPosY = 50;

int main (int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(windowWidth,windowHeight);
	glutInitWindowPosition(windowPosX,windowPosY);
	glutCreateWindow("Brick");
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, Timer, 0);
	//funcion para activar teclado
	glutKeyboardFunc(keyboard);
	initGL();
	/*GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}*/
	glutMainLoop();
	return 0;
}