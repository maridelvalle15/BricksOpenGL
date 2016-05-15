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
GLfloat xSpeed = 0.2f;
GLfloat ySpeed = 0.1f;
GLfloat ballCoords[4][2] = {0};
TCHAR path[MAX_PATH];

int refreshMillis = 30;

//Projection clipping area
GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;


void setBcoords() {
	/*ballCoords[0][0]=ballX+ballRadius;
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
	ballCoords[7][1]=-ballY+ballRadius*sin(315);*/

	ballCoords[0][0]=ballX+ballRadius;
	ballCoords[0][1]=ballY+ballRadius;
	ballCoords[1][0]=ballX-ballRadius;
	ballCoords[1][1]=ballY+ballRadius;
	ballCoords[2][0]=-ballX-ballRadius;
	ballCoords[2][1]=-ballY-ballRadius;
	ballCoords[3][0]=-ballX+ballRadius;
	ballCoords[3][1]=-ballY-ballRadius;
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
	glLineWidth(0.5f);
	glBegin(GL_LINE_STRIP);
			glVertex2d(0,0);
			float x = 0;
			float y = 0;
				x=xSpeed*100;

				y=ySpeed*100;
			glVertex2d(x,y);
	glEnd();
	glLineWidth(3.0f);
}

void dibujarBonus(float cx, float cy, int btype){
	switch (btype)
	{   //Dibujos diferentes para cada bonus.
	case(1) : 
			  glColor3f(0.0f,191/255,1.0f);
			  glBegin(GL_POLYGON);
				glVertex2d(cx+0.3,cy+0.15);
				glVertex2d(cx+0.3,cy-0.15);
				glVertex2d(cx-0.3,cy-0.15);
				glVertex2d(cx-0.3,cy+0.15);
			  glEnd();
			  glColor3f(1.0f,0,0.0f);
			  glBegin(GL_TRIANGLES);
				glVertex2d(cx+0.5,cy+0.2);
				glVertex2d(cx+0.5,cy-0.2);
				glVertex2d(cx+0.3,cy);
			  glEnd();
			  glBegin(GL_TRIANGLES);
				glVertex2d(cx-0.5,cy+0.2);
				glVertex2d(cx-0.5,cy-0.2);
				glVertex2d(cx-0.3,cy);
			  glEnd();
			  glColor3f(0.698f,0.1333f,0.1333f);
			  break;
	case(2) : glColor3f(1.0f,1,0.0f);
			  glBegin(GL_TRIANGLES);
				glVertex2d(cx+0.3,cy+0.2);
				glVertex2d(cx+0.3,cy-0.2);
				glVertex2d(cx+0.5,cy);
			  glEnd();
			  glBegin(GL_TRIANGLES);
				glVertex2d(cx+0,cy+0.2);
				glVertex2d(cx+0,cy-0.2);
				glVertex2d(cx+0.2,cy);
			  glEnd();
			  glBegin(GL_TRIANGLES);
				glVertex2d(cx-0.3,cy+0.2);
				glVertex2d(cx-0.3,cy-0.2);
				glVertex2d(cx,cy);
			  glEnd();
			  glColor3f(1.0f,1.0f,0.0f);
			  break;
	case(3) : 
			  glColor3f(0.0f,191/255,1.0f);
			  glBegin(GL_POLYGON);
				glVertex2d(cx+0.3,cy+0.15);
				glVertex2d(cx+0.3,cy-0.15);
				glVertex2d(cx-0.3,cy-0.15);
				glVertex2d(cx-0.3,cy+0.15);
			  glEnd();
			  glColor3f(1.0f,1,0.0f);
			  glBegin(GL_TRIANGLES);
				glVertex2d(cx+0.3,cy+0.2);
				glVertex2d(cx+0.3,cy-0.2);
				glVertex2d(cx+0.5,cy);
			  glEnd();
			  glBegin(GL_TRIANGLES);
				glVertex2d(cx-0.3,cy+0.2);
				glVertex2d(cx-0.3,cy-0.2);
				glVertex2d(cx-0.5,cy);
			  glEnd();
			  glColor3f(0,0.501f,0);
			  break;
	default: break;
	}
	glBegin(GL_LINE_LOOP);
		for(int i =19; i <= 360; i++){
			double angle = 2* PI * (i) / 360;
			double x = cos(angle);
			double y = sin(angle);
			glVertex2d(cx+x*(ballRadius*2),cy+y*(ballRadius*2));
		}
	glEnd();

}


void dibujarLadrillo(float ladrilloXneg, float ladrilloXpos, float ladrilloYpos, float ladrilloYneg, int isSpecial){
	glPointSize(1.0f);
	if (isSpecial==1)
		glColor3f(0.0f,1.0f,0.0f);
	else if (isSpecial==0)
		glColor3f(1.0f,0.0f,0.0f);
	else
		glColor3f(1.0f,1.0f,0.0f);
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
	Ladrillo() : active(1),breakable(0),bonus(0),counter(0),bonusAct(0){}
	float xpos;
	float xneg;
	float ypos;
	float yneg;
	bool active;
	bool breakable;
	int bonus;
	int counter;
	bool bonusAct;
};




Ladrillo ladrillos[5][7];


// numeros random para escoger ladrillos especiales (dificiles de romper)
/*int v1 = rand() % 3;
int v2 = rand() % 4;
int v3 = rand() % 5;
int v4 = rand() % 3;
int v5 = rand() % 6;// v2 in the range 1 to 100*/


int specials[5];

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
    for (int i=0; i<5; i++)
		specials[i]=aux[i];


}
void initBlocks(){

float ladrilloXn = -9.5;
float ladrilloXp = -7.5;
float ladrilloYp = 9.0f;
float ladrilloYn = 8.5f;

	for (int j = 0; j < max_columna; j++){
		for (int i = 0; i < max_fila; i++){
			ladrillos[j][i].xpos = ladrilloXp;
			ladrillos[j][i].xneg = ladrilloXn;
			ladrillos[j][i].ypos = ladrilloYp;
			ladrillos[j][i].yneg = ladrilloYn;
			ladrillos[j][i].active = 1;

			ladrilloXn += 2.8f;
			ladrilloXp += 2.8f;
		}

		ladrilloXn = -9.5;
		ladrilloXp = -7.50f;
		ladrilloYp -= 1.5f;
		ladrilloYn -= 1.5f;
	}
}

void setBonus(){
	//Generando a los bloques bonus y sus valores.
	int maxBonuses=3;//Cambiar 2 por la cantidad tipos de bonus a tener.
    int aux[35] = {0};
	int aux2[2] = {0}; 
	for (int i =0; i<35; i++)
		aux[i]=i;
	/*
{
    int color;
    srand ( time(NULL) );
    color = rand() % 4 + 1; 
    cout << color;
    return 0;
   }*/

    std::random_shuffle(aux, aux + 35);
	int col, row;
	int bonC=1;
    for (int i=0; i<6; i++){
		col = aux[i] / 5;
		row = aux[i] - col * 5;
		//time_t t;
		//std::srand((unsigned) time(&t));
		int myBonus = bonC++;
		if (bonC>maxBonuses){
			bonC=1;
		}

		ladrillos[row][col].bonus=myBonus;

		cout<<i<<"::"<<ladrillos[row][col].bonus<<"--"<<row<<","<<col<<endl;
		//cin>>col;
		
	}
}

void dibujarLadrillos(){

	//Dibujar Bonus
	for (int j=0; j<max_columna; j++){
		for (int i=0; i<max_fila; i++){
			if (ladrillos[j][i].bonusAct){
				//cout<<"Bonus: "<<i<<" Activo."<<endl;
				float mX = ladrillos[j][i].xneg+1;
				float mY = ladrillos[j][i].yneg;
				dibujarBonus(mX,mY,ladrillos[j][i].bonus);
				ladrillos[j][i].yneg-=0.06f;
				//cout<<"(X,Y)=("<<mX<<","<<mY<<")"<<endl;
				if (ladrillos[j][i].yneg<=ballYMin){
					ladrillos[j][i].bonusAct=0;
				}
			}
		}
	}

	for (int j = 0; j < max_columna; j++){
		for (int i = 0; i < max_fila; i++){
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
					dibujarLadrillo(ladrillos[j][i].xneg,ladrillos[j][i].xpos,ladrillos[j][i].ypos,ladrillos[j][i].yneg,isSpecial);
					}
					// si ya fue chocado
					else{
						dibujarLadrilloRoto(ladrillos[j][i].xneg,ladrillos[j][i].xpos,ladrillos[j][i].ypos,ladrillos[j][i].yneg);
					}
				}
				else
					dibujarLadrillo(ladrillos[j][i].xneg,ladrillos[j][i].xpos,ladrillos[j][i].ypos,ladrillos[j][i].yneg,isSpecial);
			
			}


		}

	}
}

void checkBonus(){




	//Atraparlos
	GLfloat barY = -9.0;
	GLfloat barraneg = cBarra-tam/2;
	GLfloat barrapos = cBarra+tam/2;

	for (int j=0; j<max_columna; j++){
		for (int i=0; i<max_fila; i++){
			GLfloat bX =ladrillos[j][i].xneg+1;
			GLfloat bY =ladrillos[j][i].yneg;
			if (ladrillos[j][i].bonusAct && barraneg-0.5-ballRadius <= bX && barrapos+0.5+ballRadius >= bX  && barY+0.5+ballRadius>=bY && barY-0.5-ballRadius<=bY){
				printf("Gotcha.\n");
				TCHAR pathc[MAX_PATH];
				strcpy(pathc,path);  
				strcat(pathc, "\\catch.wav");
				PlaySound(pathc, NULL, SND_ASYNC|SND_NOSTOP);

				ladrillos[j][i].bonusAct=false;
				switch (ladrillos[j][i].bonus)
				{
				case(1):
					tam-= tam * 15 / 100;
					cout<<"Menos tamaño"<<endl;
					break;
				case(2):
					xSpeed+= xSpeed * 40 / 100;
					ySpeed+= ySpeed * 40 / 100;
					cout<<"Más velocidad"<<endl;
					break;
				case(3):
					tam+= tam * 15 / 100;
					cout<<"Más Tamaño"<<endl;
					break;
				default:
					break;
				}
			}
		}
	}

}

void chocarLadrillos(){
	for (int j = 0; j < max_columna; j++){
		for (int i = 0; i < max_fila; i++){
			if (ladrillos[j][i].active){
				bool hasHit = false;

				if ((ladrillos[j][i].xneg < ballX && ladrillos[j][i].xpos > ballX && ladrillos[j][i].yneg-0.5 <= ballY && ladrillos[j][i].yneg >= ballY) ){
					ySpeed = -ySpeed;
					if (ladrillos[j][i].counter <= 1){
						hasHit=true;
					}
					if ((ladrillos[j][i].breakable == 0) || ((ladrillos[j][i].breakable == 1) && (ladrillos[j][i].counter == 1))){
						ladrillos[j][i].active = false;
					}
					ladrillos[j][i].counter = ladrillos[j][i].counter +1; 

				}
				else if ((ladrillos[j][i].xneg < ballX && ladrillos[j][i].xpos > ballX && ladrillos[j][i].ypos <= ballY && ladrillos[j][i].ypos+0.5 >= ballY) ){
					ySpeed = -ySpeed;
					if (ladrillos[j][i].counter <= 1){
						hasHit=true;
					}
					if ((ladrillos[j][i].breakable == 0) || ((ladrillos[j][i].breakable == 1) && (ladrillos[j][i].counter == 1))){
						ladrillos[j][i].active = false;
					}
					ladrillos[j][i].counter = ladrillos[j][i].counter +1; 


				}
				else if ((ladrillos[j][i].yneg < ballY && ladrillos[j][i].ypos > ballY && ladrillos[j][i].xpos <= ballX && ladrillos[j][i].xpos+0.5 >= ballX) ){
					xSpeed = -xSpeed;
					if (ladrillos[j][i].counter <= 1){
						hasHit=true;
					}
					if ((ladrillos[j][i].breakable == 0) || ((ladrillos[j][i].breakable == 1) && (ladrillos[j][i].counter == 1))){
						ladrillos[j][i].active = false;

					}
					ladrillos[j][i].counter = ladrillos[j][i].counter +1; 

				}
				else if ((ladrillos[j][i].yneg < ballY && ladrillos[j][i].ypos > ballY && ladrillos[j][i].xneg-0.5 <= ballX && ladrillos[j][i].xneg >= ballX) ){
					xSpeed = -xSpeed;
					if (ladrillos[j][i].counter <= 1){
						hasHit=true;
					}
					if ((ladrillos[j][i].breakable == 0) || ((ladrillos[j][i].breakable == 1) && (ladrillos[j][i].counter == 1))){
						ladrillos[j][i].active = false;
					}
					ladrillos[j][i].counter = ladrillos[j][i].counter +1; 

				}
				// Si choca, suena

				if (hasHit){
					TCHAR h1[MAX_PATH];
					strcpy(h1,path);  
					strcat(h1, "\\hit1.wav");
					PlaySound(h1, NULL, SND_ASYNC );
				}
				// verificamos si la pelota choco contra ladrillo bonus
				if (!(ladrillos[j][i].bonusAct) && !(ladrillos[j][i].active) && ladrillos[j][i].bonus > 0  && (ladrillos[j][i].counter == 1 || (ladrillos[j][i].breakable == 1 && ladrillos[j][i].counter == 2))){
					cout<<"Be free, bonus "<<ladrillos[j][i].bonus<<"\n";
					ladrillos[j][i].bonusAct=true;
				}
			}
		}
	}



}
void barHit(){
	//Impacto contra la barra.

	//ballCoords;
	GLfloat barY = -9.0;
	GLfloat barraneg = cBarra-tam/2;
	GLfloat barrapos = cBarra+tam/2;

	bool hasHit2=false;
	//Esquinas
	if (barrapos <= ballX-ballRadius && barrapos+2*ballRadius >= ballX && barY+2*ballRadius >= ballY && barY <= ballY-ballRadius){
		hasHit2=true;
		printf("ESQUINA 1");
		if (xSpeed < 0 && ySpeed < 0){
			xSpeed=-xSpeed;
			ySpeed=-ySpeed;
		}
		else if (xSpeed < 0 && ySpeed > 0){
			xSpeed=-xSpeed;
		}
		else if (xSpeed > 0 && ySpeed < 0){  
			ySpeed=-ySpeed;
		}
		else {;} //> < no tiene sentido.
	}
	else if (barraneg >= ballX+ballRadius && barraneg-2*ballRadius <= ballX && barY+2*ballRadius >= ballY && barY <= ballY-ballRadius){
		hasHit2=true;
		printf("ESQUINA 2");
		if (xSpeed < 0 && ySpeed < 0){
			ySpeed=-ySpeed;
		}
		else if (xSpeed > 0 && ySpeed > 0){
			xSpeed=-xSpeed;
		}
		else if (xSpeed > 0 && ySpeed < 0){  
			xSpeed=-xSpeed;
			ySpeed=-ySpeed;
		}
		else {;} //< > no tiene sentido.
	}else if ((barraneg <= ballX-ballRadius && barrapos >= ballX+ballRadius)  && barY<=ballY-ballRadius && ballY<barY+2*ballRadius && ySpeed<0 ){
		printf("CASO1\n");
			ySpeed = -ySpeed;
			hasHit2=true;
	}
	else if ( barraneg >= ballX+ballRadius && barraneg-2*ballRadius <= ballX   && barY-2*ballRadius<=ballY && ballY<=barY+2*ballRadius && xSpeed>0 ){
		printf("CASO2\n");
		xSpeed = -xSpeed;
		hasHit2=true;
		//printf("BOOM.");
	}else if ( barrapos < ballX-ballRadius && barrapos+2*ballRadius > ballX   && barY-2*ballRadius<=ballY && ballY<=barY+2*ballRadius && xSpeed<0 ){
		printf("CASO3\n");
		xSpeed = -xSpeed;
		hasHit2=true;
	}
	else {
		/*for (int i=0; i<4; i++){
			int x1 = ballCoords[i][0];
			int y1 = ballCoords[i][1];
			int x2 = ballCoords[4-i][0];
			int y2 = ballCoords[i+1][1];*/
			


		}


	if (hasHit2){
		TCHAR h2[MAX_PATH];
		strcpy(h2,path);  
		strcat(h2, "\\hit2.wav");
		PlaySound(h2, NULL, SND_ASYNC );
	}
}


void render(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	setBcoords();
	glLoadIdentity();
	
	glPushMatrix();
	 dibujarLadrillos();
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
	checkBonus();
	barHit();
	chocarLadrillos();
	dibujarLadrillos();

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
	initBlocks();
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



	GetCurrentDirectory(MAX_PATH,path);  
	TCHAR pathm[MAX_PATH];
	strcpy(pathm,path);  
	strcat(pathm, "\\mainsound.wav");
	PlaySound(pathm, NULL, SND_LOOP|SND_ASYNC| SND_NOSTOP );
	glutMainLoop();
	return 0;
}