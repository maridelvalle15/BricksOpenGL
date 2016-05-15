#include <iostream>
#include <windows.h>
#include <Math.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

#include <cassert>
#include <functional>
#include <time.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
using namespace std;

#define PI 3.1415926535897932384626433832795

GLfloat ballRadius = 0.3f;
GLfloat cBarra = 0;
GLfloat tam = 4;
GLfloat ballX = 0.0f;
GLfloat ballY = 0.0f;
GLfloat ballXMax, ballXMin, ballYMax, ballYMin; 
GLfloat xSpeed = 0.1f;
GLfloat ySpeed = 0.05f;
GLfloat ballCoords[8][2] = {0};

int refreshMillis = 30;

//Projection clipping area
GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;


void setBcoords() {
	ballCoords[0][0]=ballX+ballRadius;
	ballCoords[0][1]=ballY;
	ballCoords[1][0]=ballX+ballRadius*cos(45);
	ballCoords[1][1]=ballY+ballRadius*sin(45);
	ballCoords[2][0]=ballX;
	ballCoords[2][1]=ballY+ballRadius;
	ballCoords[3][0]=ballX+ballRadius*sin(135);
	ballCoords[3][1]=ballY+ballRadius*sin(135);
	ballCoords[4][0]=-ballX+ballRadius;
	ballCoords[4][1]=-ballY;
	ballCoords[5][0]=-ballX+ballRadius*cos(225);
	ballCoords[5][1]=-ballY+ballRadius*sin(225);
	ballCoords[6][0]=-ballX;
	ballCoords[6][1]=-ballY+ballRadius;
	ballCoords[7][0]=-ballX+ballRadius*cos(315);
	ballCoords[7][1]=-ballY+ballRadius*sin(315);
}

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
	glBegin(GL_POLYGON);
		for(int i =19; i <= 360; i++){
			double angle = 2* PI * (i) / 360;
			double x = cos(angle);
			double y = sin(angle);
			glVertex2d(x*ballRadius,y*ballRadius);
		}
	glEnd();
}

void dibujarBonus(float cx, float cy){
	glColor3f(0.0f,1.0f,1.0f);
	glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < 100; ii++)   {
        float theta = 2.0f * 3.1415926f * float(ii) / float(100);//get the current angle 
        float x = (0.1) * cosf(theta);//calculate the x component 
        float y = (0.1) * sinf(theta);//calculate the y component 
        glVertex2f(x + cx, y + cy);//output vertex 
    }
    glEnd();
}

void soltarBonus(float xpos, float ypos){
	//glTranslatef(xpos,ypos,0.0f);
	dibujarBonus(xpos,ypos);
	//glutSwapBuffers();
}

void dibujarLadrillo(float ladrilloXneg, float ladrilloXpos, float ladrilloYpos, float ladrilloYneg, bool isSpecial){
	glPointSize(1.0f);
	if (isSpecial)
		glColor3f(0.0f,1.0f,0.0f);
	else
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

void dibujarLadrilloRoto(float ladrilloXneg, float ladrilloXpos, float ladrilloYpos, float ladrilloYneg){
	glPointSize(1.0f);
		glColor3f(0.0f,1.0f,0.0f);
		glLineWidth(3.0f);
		float ancho = (ladrilloXpos+ladrilloXneg)/2;
		float largo = (ladrilloYneg+ladrilloYpos)/2;
		glBegin(GL_LINES);
			glVertex2f(ladrilloXneg,ladrilloYneg);
			glVertex2f(ladrilloXpos,ladrilloYneg);
			glVertex2f(ladrilloXneg,ladrilloYpos);
			glVertex2f(ladrilloXpos,ladrilloYpos);
			glVertex2f(ladrilloXneg,ladrilloYneg);
			glVertex2f(ladrilloXneg,ladrilloYpos);
			glVertex2f(ladrilloXpos,ladrilloYneg);
			glVertex2f(ladrilloXpos,ladrilloYpos);

			glVertex2f(ancho-0.15,ladrilloYpos);
			glVertex2f(ancho-0.40,largo);
			glVertex2f(ancho-0.40,largo);
			glVertex2f(ancho-0.15,ladrilloYneg);

			glVertex2f(ancho+0.35,ladrilloYpos);
			glVertex2f(ancho+0.10,largo);
			glVertex2f(ancho+0.10,largo);
			glVertex2f(ancho+0.35,ladrilloYneg);

			glColor3f(0.0f,0.0f,0.0f);
			glVertex2f(ancho-0.15,ladrilloYpos);
			glVertex2f(ancho+0.27,ladrilloYpos);
			glVertex2f(ancho-0.15,ladrilloYneg);
			glVertex2f(ancho+0.27,ladrilloYneg);
		glEnd();
}

int max_fila = 7;
int max_columna = 5;

float ladrilloXneg = -9.5;
float ladrilloXpos = -7.5;
float ladrilloYpos = 9.0f;
float ladrilloYneg = 8.5f;

struct Ladrillo{
	Ladrillo() : active(1),breakable(0),bonus(0),counter(0){}
	float xpos;
	float xneg;
	float ypos;
	float yneg;
	bool active;
	bool breakable;
	int bonus;
	int counter;
};

Ladrillo ladrillos[5][7];

// numeros random para escoger ladrillos especiales (dificiles de romper)
/*int v1 = rand() % 3;
int v2 = rand() % 4;
int v3 = rand() % 5;
int v4 = rand() % 3;
int v5 = rand() % 6;// v2 in the range 1 to 100*/


int specials[5];
int bonuses[6];
// numeros random para escoger ladrillos con bonus
/*int bonus1 = rand() % 1;
int bonus2 = rand() % 6;
int bonus3 = rand() % 4;
int bonus4 = rand() % 3;
int bonus5 = rand() % 5;// v2 in the range 1 to 100*/

void setSpecials(){

	//Generando a los bloques especiales
    int aux[35] = {0};
	for (int i =0; i<35; i++)
		aux[i]=i;
	time_t t;
	std::srand((unsigned) time(&t));

    std::random_shuffle(aux, aux + 35);
	int col, row;
    for (int i=0; i<5; i++)
		specials[i]=aux[i];


}

void setBonus(){
	//Generando a los bloques bonus y sus valores.
	int maxBonuses=2;//Cambiar 2 por la cantidad tipos de bonus a tener.
    int aux[35] = {0};
	int aux2[2] = {0}; 
	for (int i =0; i<35; i++)
		aux[i]=i;

	time_t t;
	std::srand((unsigned) time(&t));

    std::random_shuffle(aux, aux + 35);
	int col, row;
    for (int i=0; i<6; i++){
		//bonuses[i]=aux[i];
		col = aux[i] / 7;
		row = aux[i] - col * 7;
		int myBonus = rand()%(maxBonuses-1 + 1) + 1;
		time_t t2;
		std::srand((unsigned) time(&t2));


		ladrillos[row][col].bonus=myBonus;
		cout<<aux[i]<<"::"<<col<<"--"<<row<<" bonus:"<<myBonus<<endl;
		cin>>col;
		
	}
}

void dibujarLadrillos(float ladrilloXpos, float ladrilloYpos, float ladrilloXneg, float ladrilloYneg){


	for (int j = 0; j < max_columna; j++){
		for (int i = 0; i < max_fila; i++){
			ladrillos[j][i].xpos = ladrilloXpos;
			ladrillos[j][i].xneg = ladrilloXneg;
			ladrillos[j][i].ypos = ladrilloYpos;
			ladrillos[j][i].yneg = ladrilloYneg;
			if (ladrillos[j][i].active){
				// ver si es una posicion especial
				bool isSpecial = false;
				for (int k=0; k<5; k++)
					if (j * 7 + i == specials[k]){
						isSpecial=true;
						break;
					} 

				if (isSpecial){
					// Si no ha sido chocado
					if (ladrillos[j][i].counter == 0){
					ladrillos[j][i].breakable = 1;
					dibujarLadrillo(ladrilloXneg,ladrilloXpos,ladrilloYpos,ladrilloYneg,isSpecial);
					}
					// si ya fue chocado
					else{
						dibujarLadrilloRoto(ladrilloXneg,ladrilloXpos,ladrilloYpos,ladrilloYneg);
					}
				}
				else{
				dibujarLadrillo(ladrilloXneg,ladrilloXpos,ladrilloYpos,ladrilloYneg,isSpecial);
				}
				// ver si es una posicion bonus
				/*if ((i == bonus1 && j==0) || (i == bonus2 && j==1) || (i == bonus3 && j==2) || (i == bonus4 && j==3) || (i == bonus5 && j==4)){
					ladrillos[j][i].bonus = 1;
				}*/
			
}

			ladrilloXneg += 2.8f;
			ladrilloXpos += 2.8f;
		}
		ladrilloXneg = -9.5;
		ladrilloXpos = -7.50f;
		ladrilloYpos -= 1.5f;
		ladrilloYneg -= 1.5f;
	}
}

void chocarLadrillos(){
	for (int j = 0; j < max_columna; j++){
		for (int i = 0; i < max_fila; i++){
			if (ladrillos[j][i].active){

			if ((ladrillos[j][i].xneg < ballX && ladrillos[j][i].xpos > ballX && ladrillos[j][i].yneg-0.5 <= ballY && ladrillos[j][i].yneg >= ballY) ){
				ySpeed = -ySpeed;
				if ((ladrillos[j][i].breakable == 0) || ((ladrillos[j][i].breakable == 1) && (ladrillos[j][i].counter == 1))){
					ladrillos[j][i].active = false;
				}
				ladrillos[j][i].counter = ladrillos[j][i].counter +1; 
				// verificamos si la pelota choco contra ladrillo bonus
				if (ladrillos[j][i].bonus == 1){
					soltarBonus(ladrillos[j][i].xneg,ladrillos[j][i].yneg);
					printf("%s","SOy bonus");
				}
			}
			else if ((ladrillos[j][i].xneg < ballX && ladrillos[j][i].xpos > ballX && ladrillos[j][i].ypos <= ballY && ladrillos[j][i].ypos+0.5 >= ballY) ){
				ySpeed = -ySpeed;
				if ((ladrillos[j][i].breakable == 0) || ((ladrillos[j][i].breakable == 1) && (ladrillos[j][i].counter == 1))){
					ladrillos[j][i].active = false;
				}
				ladrillos[j][i].counter = ladrillos[j][i].counter +1; 
				// verificamos si la pelota choco contra ladrillo bonus
				if (ladrillos[j][i].bonus == 1){
					soltarBonus(ladrillos[j][i].xneg,ladrillos[j][i].yneg);
					printf("%s","SOy bonus");
				}
			}
			else if ((ladrillos[j][i].yneg < ballY && ladrillos[j][i].ypos > ballY && ladrillos[j][i].xpos <= ballX && ladrillos[j][i].xpos+0.5 >= ballX) ){
				xSpeed = -xSpeed;
				if ((ladrillos[j][i].breakable == 0) || ((ladrillos[j][i].breakable == 1) && (ladrillos[j][i].counter == 1))){
				ladrillos[j][i].active = false;
				}
				ladrillos[j][i].counter = ladrillos[j][i].counter +1; 
				// verificamos si la pelota choco contra ladrillo bonus
				if (ladrillos[j][i].bonus == 1){
					soltarBonus(ladrillos[j][i].xneg,ladrillos[j][i].yneg);
					printf("%s","SOy bonus");
				}
			}
			else if ((ladrillos[j][i].yneg < ballY && ladrillos[j][i].ypos > ballY && ladrillos[j][i].xneg-0.5 <= ballX && ladrillos[j][i].xneg >= ballX) ){
				xSpeed = -xSpeed;
				if ((ladrillos[j][i].breakable == 0) || ((ladrillos[j][i].breakable == 1) && (ladrillos[j][i].counter == 1))){
				ladrillos[j][i].active = false;
				}
				ladrillos[j][i].counter = ladrillos[j][i].counter +1; 
				// verificamos si la pelota choco contra ladrillo bonus
				if (ladrillos[j][i].bonus == 1){
					soltarBonus(ladrillos[j][i].xneg,ladrillos[j][i].yneg);
					printf("%s","SOy bonus");
				}
			}

			}
		}
	}

	GLfloat barY = -9.0;
	GLfloat barraneg = cBarra-tam/2;
	GLfloat barrapos = cBarra+tam/2;

	// Cambiando los choques. Faltan los otros casos y los de los bloques.
	if ( ((barraneg-0.5 < ballX && barraneg > ballX)||(barrapos+0.5 > ballX && barrapos < ballX))   && barY+0.5>ballY && ballY>barY ){
		xSpeed = -xSpeed;
		printf("BOOM.");
	}
	else if ( ((barraneg-0.5 < ballX && barraneg > ballX)||(barrapos+0.5 > ballX && barrapos < ballX))   && barY+0.5>ballY && ballY>barY ){
		xSpeed = -xSpeed;
		printf("BOOM.");
	}

}

void render(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	setBcoords();
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
	dibujarLadrillos(ladrilloXpos,ladrilloYpos,ladrilloXneg,ladrilloYneg);

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
		xSpeed = 0.3f;
		ySpeed = 0.2f;
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
	/*	glutPostRedisplay(); /* this redraws the scene without 
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
	setSpecials();
	setBonus();
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



	TCHAR pwd[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pwd);  

strcat(pwd, "\\mainsound.wav");
	//printf("%s\n",pwd);
	//Uncomment for surprises.
	PlaySound(pwd, NULL, SND_FILENAME|SND_LOOP|SND_ASYNC);
	glutMainLoop();
	return 0;
}