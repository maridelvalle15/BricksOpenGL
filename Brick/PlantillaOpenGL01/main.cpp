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

#include <chrono>
#include <thread>

using namespace std;

/* Proyecto 2 - Computacion Grafica
Abr-Jul 2016
Ultima modificacion: 15/06/16
autores:
Marisela del Valle 11-10267
Nabil J. Marquez   11-10683
*/

//Inicializadores. 
#define PI 3.1415926535897932384626433832795

GLfloat ballRadius = 0.3f;
GLfloat cBarra = 0;
GLfloat tam = 4;
GLfloat ballX = cBarra;
GLfloat ballY = -9.0f+ballRadius;
GLfloat ballYMin = -10.5; 
GLfloat ballXMin=-10.5;
GLfloat ballXMax=10.5;
GLfloat ballYMax=7.5;
GLfloat xSpeed = 0;
GLfloat ySpeed = 0;
bool isPause = true;
TCHAR path[MAX_PATH];
bool defeatedB = false;
bool victory = false;
int fCounter = 0;
bool deS = false;
bool viS = false; 
int refreshMillis = 30;

GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;

int max_fila = 7;
int max_columna = 5;

float ladrilloXneg = -9.5;
float ladrilloXpos = -7.5;
float ladrilloYpos = 9.0f;
float ladrilloYneg = 8.5f;

int specials[5];
//estructura de los bloques
struct Ladrillo{
	Ladrillo() : active(1),breakable(0),bonus(0),counter(0),bonusAct(0),isSpecial(0){}
	float xpos;
	float xneg;
	float ypos;
	float yneg;
	bool active;
	bool breakable;
	int bonus;
	int counter;
	bool bonusAct;
	bool isSpecial;
};
Ladrillo ladrillos[5][7];
//estrucutra de las particulas de explosion
struct firework{
	firework() : x(0),y(0),active(0),exploded(0){}
	float x;
	float y;
	float xs[10];
	float ys[10];
	bool active;
	bool exploded;
};

firework fireworks[5];

//para convertir a rbg
float rbg(float i){
	return i/255;
}

void initGL(){
	glClearColor(0.0,0.0,0.0,1.0);
}

//No usada
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
			if (i>150)
				glColor3f(rbg(128),rbg(128),0.0);
			double angle = 2* PI * (i) / 360;
			double x = cos(angle);
			double y = sin(angle);
			glVertex2d(x*ballRadius,y*ballRadius);
		}
	glEnd();

	/* //Linea guia de impacto
	glLineWidth(0.5f);
	glBegin(GL_LINE_STRIP);
			glVertex2d(0,0);
			float x = 0;
			float y = 0;
				x=xSpeed*1000;

				y=ySpeed*1000;
			glVertex2d(x,y);
	glEnd();
	glLineWidth(3.0f);*/
}

void dibujarBonus(float cx, float cy, int btype){
	switch (btype)
	{   //Dibujos diferentes para cada bonus.
	case(1) :  //Disminución de tamano de la barra
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
	case(2) : glColor3f(1.0f,1,0.0f); //Aumento de velocidad de la pelota
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
	case(3) :  //Amuento de tamaño de la barra
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

//Con degradado
void dibujarLadrillo(float ladrilloXneg, float ladrilloXpos, float ladrilloYpos, float ladrilloYneg, int isSpecial){
	glPointSize(1.0f);
	if (isSpecial==1)
		glColor3f(0.0f,1.0f,0.0f);
	else if (isSpecial==0)
		glColor3f(1.0f,0.0f,0.0f);
	glLineWidth(3.0f);
	glBegin(GL_LINE_LOOP);
		glVertex2f(ladrilloXneg,ladrilloYneg);
		glVertex2f(ladrilloXneg,ladrilloYpos);
	if (isSpecial==1)
		glColor3f(0,rbg(100),rbg(0));
	else if (isSpecial==0)
		glColor3f(rbg(139),0.0f,0.0f);
		glVertex2f(ladrilloXpos,ladrilloYpos);
		glVertex2f(ladrilloXpos,ladrilloYneg);

	glEnd();
}

//Con degrado
void dibujarLadrilloRoto(float ladrilloXneg, float ladrilloXpos, float ladrilloYpos, float ladrilloYneg){
	glPointSize(1.0f);
		glColor3f(0,1,rbg(0));
		glLineWidth(3.0f);
		float ancho = (ladrilloXpos+ladrilloXneg)/2;
		float largo = (ladrilloYneg+ladrilloYpos)/2;

		glBegin(GL_LINE_LOOP);
			glVertex2f(ladrilloXneg,ladrilloYneg);
			glVertex2f(ladrilloXneg,ladrilloYpos);
			glColor3f(0,rbg(150),rbg(0));

			glVertex2f(ancho-0.15,ladrilloYpos);
			glVertex2f(ancho-0.40,largo);
			glVertex2f(ancho-0.15,ladrilloYneg);
		glEnd();
		glBegin(GL_LINE_LOOP);
			glColor3f(0,rbg(100),rbg(0));
			glVertex2f(ancho+0.35,ladrilloYneg);
			glVertex2f(ancho+0.10,largo);
			glVertex2f(ancho+0.35,ladrilloYpos);
			glVertex2f(ladrilloXpos,ladrilloYpos);
			glVertex2f(ladrilloXpos,ladrilloYneg);
		glEnd();
}

//Con degradado
void dibujarParedes(){
glColor3f(1.0f,0.0,1.0f);

	glTranslated(0,1,0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(-11,-9.5);
		glVertex2f(-12,-9.5);
		glVertex2f(-12,8.5);
glColor3f(rbg(75),rbg(0),rbg(130));
		glVertex2f(12,8.5);
		glVertex2f(12,-9.5);
		glVertex2f(11,-9.5);
		glVertex2f(11,7.5);
glColor3f(rbg(186),rbg(85),rbg(211));
		glVertex2f(-11,7.5);
		glVertex2f(-11,-9.5);


	glEnd();
}


//Procedimiento que inicializa los bloques, y los las explosiones
void initBlocks(){

float ladrilloXn = -9.5;
float ladrilloXp = -7.5;
float ladrilloYp = 7.0f;
float ladrilloYn = 6.3f;
int counter = 0;
time_t t;
std::srand((unsigned) time(&t)); //Posiciones aleatorias de las particulas
float aux2[10]={0.2,0.15,0.2,0.1,0.3,0.34,0.1,0.05,0.08,0.25};

	for (int j = 0; j < max_columna; j++){
		for (int i = 0; i < max_fila; i++){
			ladrillos[j][i].xpos = ladrilloXp;
			ladrillos[j][i].xneg = ladrilloXn;
			ladrillos[j][i].ypos = ladrilloYp;
			ladrillos[j][i].yneg = ladrilloYn;
			ladrillos[j][i].active = 1;

			//Para los firework
			for (int k=0; k<5; k++){
				if (j * 7 + i == specials[k]){
					ladrillos[j][i].isSpecial=true;
					std::random_shuffle(aux2, aux2 + 10);
					fireworks[k].x=ladrilloXn+0.5f;
					fireworks[k].y=ladrilloYn+0.5f;

					for (int w=0; w<10; w++){
						fireworks[k].xs[w]=aux2[w];
					}
					std::random_shuffle(aux2, aux2 + 10);
					for (int w=0; w<10; w++){
						fireworks[k].ys[w]=aux2[w];
					}
					break;
				} 
			}

			ladrilloXn += 2.8f;
			ladrilloXp += 2.8f;
		}

		ladrilloXn = -9.5;
		ladrilloXp = -7.50f;
		ladrilloYp -= 1.5f;
		ladrilloYn -= 1.5f;
	}

}

//define aleatoreamente los bloques especiales
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

//define aleatoreamente los bloques bonus equitativamente
void setBonus(){
	//Generando a los bloques bonus y sus valores.
	int maxBonuses=3;//Cambiar 3 por la cantidad tipos de bonus a tener.
    int aux[35] = {0};
	int aux2[2] = {0}; 
	for (int i =0; i<35; i++)
		aux[i]=i;

	std::random_shuffle(aux, aux + 35);
	int col, row;
	int bonC=1;
    for (int i=0; i<6; i++){
		col = aux[i] / 5;
		row = aux[i] - col * 5;

		int myBonus = bonC++;
		if (bonC>maxBonuses){
			bonC=1;
		}
		ladrillos[row][col].bonus=myBonus;
		
	}
}

//Verifica si el jugador ha ganado
void checkVictory(){
	bool myVict = true;
	for (int j=0; j<max_columna; j++){
		for (int i=0; i<max_fila; i++){
			myVict = myVict && !(ladrillos[j][i].active);
			if (!(myVict))
				break;
		}
		if (!(myVict))
			break;
	}
	if (myVict)
		victory=true;
}

//Dibuja las particulas de explosion
void dibujarFire(firework f){
	glColor3f(0,rbg(150),rbg(0));


	for (int j=0; j<10; j++){
		if (f.xs[j]!=0 && f.ys[j]!=0){
			if (j>=5)
				glColor3f(0,1,0);
			glBegin(GL_POLYGON);
				for(int i =19; i <= 360; i++){
					double angle = 2* PI * (i) / 360;
					double x = cos(angle);
					double y = sin(angle);
					glVertex2d(f.x+f.xs[j]+x*0.1,f.y+f.ys[j]+y*0.1);
				}
			glEnd();
			glLineWidth(0.5f);
		}
	}
}

//Dibuja los ladrillos segun sea el caso
void dibujarLadrillos(){

	//Dibujar Bonus
	for (int j=0; j<max_columna; j++){
		for (int i=0; i<max_fila; i++){
			if (ladrillos[j][i].bonusAct){
				float mX = ladrillos[j][i].xneg+1;
				float mY = ladrillos[j][i].yneg;
				dibujarBonus(mX,mY,ladrillos[j][i].bonus);
				ladrillos[j][i].yneg-=0.06f;
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

				for (int k=0; k<5; k++){
					if (j * 7 + i == specials[k]){
						isSpecial=true;
						break;
					} 
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
			else {  //Fireworks
				for (int k=0; k<5; k++){
					if (j * 7 + i == specials[k]){

						//fireworks
						if (!(fireworks[k].exploded)){
							dibujarFire(fireworks[k]);

							for (int w=0; w<10; w++){
								if (abs(fireworks[k].xs[w]+fireworks[k].x)>13) {
									fireworks[k].xs[w]=0;
								}
								if (abs(fireworks[k].ys[w]+fireworks[k].y)>13) {
									fireworks[k].ys[w]=0;
								}

								fireworks[k].xs[w]+=fireworks[k].xs[w]/50;
								fireworks[k].ys[w]+=fireworks[k].ys[w]/50;
							}
							bool isOver=false;
							for (int w=0; w<10; w++){
								isOver=isOver && fireworks[k].xs[w]==0 && fireworks[k].ys[w] == 0;
							}
							if (isOver){
								fireworks[k].exploded=true;
								break;
							}
						}
					} 
				}
			}


		}

	}

	if (defeatedB){  //Pantalla de derrota
		if (!(deS)){
			deS=true;
			TCHAR defS[MAX_PATH];
			strcpy(defS,path);  
			strcat(defS, "\\death.wav");
			PlaySound(defS, NULL, SND_LOOP|SND_ASYNC| SND_NOSTOP );
		}
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		fCounter+=1;
		float xn = -10.5;
		float xp = 10.5;
		float yp = 7.5f;
		float yn = -3.0f;
		glColor4f(1,1,1,0.5);
		glBegin(GL_POLYGON);
			glVertex3d(xn,yp,0.8);
			glVertex3d(xp,yp,0.8);
			glVertex3d(xp,yn,0.8);
			glVertex3d(xn,yn,0.8);
		glEnd();
		glColor4f(rbg(139),0,0,1);

		glBegin(GL_LINE_LOOP);
			glVertex2d(-9,6.5);
			glVertex2d(-9,1.5);
			glVertex2d(-9,3.5);
			glVertex2d(-7,6.5);
			glVertex2d(-9,3.5);
			glVertex2d(-7,1.5);
			glVertex2d(-9,3.5);
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex2d(-5,6.5);
			glVertex2d(-3,6.5);
			glVertex2d(-3,1.5);
			glVertex2d(-5,1.5);
		glEnd();
		if (fCounter>=2000)
			exit(0);
		
	}
	if (victory){   //Pantalla de victoria
		if (!(viS)){
			viS=true;
			TCHAR viSS[MAX_PATH];
			strcpy(viSS,path);  
			strcat(viSS, "\\win.wav");
			PlaySound(viSS, NULL, SND_LOOP|SND_ASYNC| SND_NOSTOP );
		}
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		fCounter+=1;
		float xn = -10.5;
		float xp = 10.5;
		float yp = 7.5f;
		float yn = -3.0f;
		glColor4f(1,1,1,0.8);
		glBegin(GL_POLYGON);
			glVertex3d(xn,yp,0.8);
			glVertex3d(xp,yp,0.8);
			glVertex3d(xp,yn,0.8);
			glVertex3d(xn,yn,0.8);
		glEnd();
		glColor4f(0,1,0,1);

		glBegin(GL_LINE_LOOP);
			glVertex2d(-9,6.5);
			glVertex2d(-6,6.5);
			glVertex2d(-6,1.5);
			glVertex2d(-9,1.5);
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex2d(-5,6.5);
			glVertex2d(-5,1.5);
			glVertex2d(-5,3.5);
			glVertex2d(-3,6.5);
			glVertex2d(-5,3.5);
			glVertex2d(-3,1.5);
			glVertex2d(-5,3.5);
		glEnd();
		if (fCounter>=2000)
			exit(0);
		
	}

}

//Verifica si la barra atrapa a un bonus cayendo.
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
					break;
				case(2):
					xSpeed+= xSpeed * 40 / 100;
					ySpeed+= ySpeed * 40 / 100;
					break;
				case(3):
					tam+= tam * 15 / 100;
					break;
				default:
					break;
				}
			}
		}
	}

}

//Verifica si hubo algun impacto con algun lateral de un bloque o sus esquinas.
void chocarLadrillos(){
	for (int j = 0; j < max_columna; j++){
		for (int i = 0; i < max_fila; i++){
			if (ladrillos[j][i].active){
				bool hasHit = false;

				if (ladrillos[j][i].xpos <= ballX-ballRadius &&
					ladrillos[j][i].xpos+2*ballRadius >= ballX &&
					ladrillos[j][i].ypos+2*ballRadius >= ballY &&
					ladrillos[j][i].ypos <= ballY-ballRadius){

					hasHit=true;
					printf("ESQUINA 1\n");

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

					if (ladrillos[j][i].counter <= 1){
						hasHit=true;
					}
					if ((ladrillos[j][i].breakable == 0) || ((ladrillos[j][i].breakable == 1) && (ladrillos[j][i].counter == 1))){
						ladrillos[j][i].active = false;
					}
					ladrillos[j][i].counter = ladrillos[j][i].counter +1; 
				}

				else if (ladrillos[j][i].xneg >= ballX+ballRadius &&
						 ladrillos[j][i].xneg-2*ballRadius <= ballX &&
						 ladrillos[j][i].ypos+2*ballRadius >= ballY &&
						 ladrillos[j][i].ypos <= ballY-ballRadius){
					hasHit=true;
					printf("ESQUINA 2\n");
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
					else {		printf("AAAAAAAAAAAAAAAA"); }

					if (ladrillos[j][i].counter <= 1){
						hasHit=true;
					}
					if ((ladrillos[j][i].breakable == 0) || ((ladrillos[j][i].breakable == 1) && (ladrillos[j][i].counter == 1))){
						ladrillos[j][i].active = false;
					}
					ladrillos[j][i].counter = ladrillos[j][i].counter +1; 

				}
				else if (ladrillos[j][i].xpos <= ballX-2*ballRadius &&
					ladrillos[j][i].xpos+2*ballRadius >= ballX &&
					ladrillos[j][i].yneg-2*ballRadius <= ballY &&
					ladrillos[j][i].yneg >= ballY+ballRadius){

					hasHit=true;
					printf("ESQUINA 4\n");

					if (xSpeed < 0 && ySpeed < 0){
						xSpeed=-xSpeed;
					}
					else if (xSpeed < 0 && ySpeed > 0){
						ySpeed=-ySpeed;
						xSpeed=-xSpeed;
					}
					else if (xSpeed > 0 && ySpeed > 0){  
						ySpeed=-ySpeed;
					}
					else {;} //> < no tiene sentido.

					if (ladrillos[j][i].counter <= 1){
						hasHit=true;
					}
					if ((ladrillos[j][i].breakable == 0) || ((ladrillos[j][i].breakable == 1) && (ladrillos[j][i].counter == 1))){
						ladrillos[j][i].active = false;
					}
					ladrillos[j][i].counter = ladrillos[j][i].counter +1; 
				}

				else if (ladrillos[j][i].xneg >= ballX+ballRadius &&
						 ladrillos[j][i].xneg-2*ballRadius <= ballX &&
						 ladrillos[j][i].yneg-2*ballRadius <= ballY &&
						 ladrillos[j][i].yneg >= ballY+ballRadius){
					hasHit=true;
					printf("ESQUINA 3\n");
					if (xSpeed < 0 && ySpeed > 0){
						ySpeed=-ySpeed;
					}
					else if (xSpeed > 0 && ySpeed > 0){
						ySpeed=-ySpeed;
						xSpeed=-xSpeed;
					}
					else if (xSpeed > 0 && ySpeed < 0){  
						xSpeed=-xSpeed;
					}
					else {		printf("AAAAAAAAAAAAAAAA"); } // < < no tiene sentido

					if (ladrillos[j][i].counter <= 1){
						hasHit=true;
					}
					if ((ladrillos[j][i].breakable == 0) || ((ladrillos[j][i].breakable == 1) && (ladrillos[j][i].counter == 1))){
						ladrillos[j][i].active = false;
					}
					ladrillos[j][i].counter = ladrillos[j][i].counter +1; 
				//FIN ESQUINAS
				}

				else if ((ladrillos[j][i].xneg < ballX && ladrillos[j][i].xpos > ballX && ladrillos[j][i].yneg-ballRadius <= ballY && ladrillos[j][i].yneg >= ballY) ){
					printf("CasoL 1\n");
					ySpeed = -ySpeed;
					if (ladrillos[j][i].counter <= 1){
						hasHit=true;
					}
					if ((ladrillos[j][i].breakable == 0) || ((ladrillos[j][i].breakable == 1) && (ladrillos[j][i].counter == 1))){
						ladrillos[j][i].active = false;
					}
					ladrillos[j][i].counter = ladrillos[j][i].counter +1; 
				}
				else if ((ladrillos[j][i].xneg < ballX && ladrillos[j][i].xpos > ballX && ladrillos[j][i].ypos <= ballY && ladrillos[j][i].ypos+ballRadius >= ballY) ){
					printf("CasoL 2\n");
					ySpeed = -ySpeed;
					if (ladrillos[j][i].counter <= 1){
						hasHit=true;
					}
					if ((ladrillos[j][i].breakable == 0) || ((ladrillos[j][i].breakable == 1) && (ladrillos[j][i].counter == 1))){
						ladrillos[j][i].active = false;
					}
					ladrillos[j][i].counter = ladrillos[j][i].counter +1; 


				}
				else if ((ladrillos[j][i].yneg < ballY && ladrillos[j][i].ypos > ballY && ladrillos[j][i].xpos <= ballX && ladrillos[j][i].xpos+ballRadius >= ballX) ){
					printf("CasoL 3\n");
					xSpeed = -xSpeed;
					if (ladrillos[j][i].counter <= 1){
						hasHit=true;
					}
					if ((ladrillos[j][i].breakable == 0) || ((ladrillos[j][i].breakable == 1) && (ladrillos[j][i].counter == 1))){
						ladrillos[j][i].active = false;

					}
					ladrillos[j][i].counter = ladrillos[j][i].counter +1; 

				}
				else if ((ladrillos[j][i].yneg < ballY && ladrillos[j][i].ypos > ballY && ladrillos[j][i].xneg-ballRadius <= ballX && ladrillos[j][i].xneg >= ballX) ){
					printf("CasoL 4\n");
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
					strcat(h1, "\\hit1.wav"); //Impacto con bloque
					PlaySound(h1, NULL, SND_ASYNC );
				}
				// verificamos si la pelota choco contra ladrillo bonus
				if (!(ladrillos[j][i].bonusAct) && !(ladrillos[j][i].active) && ladrillos[j][i].bonus > 0  && (ladrillos[j][i].counter == 1 || (ladrillos[j][i].breakable == 1 && ladrillos[j][i].counter == 2))){
					ladrillos[j][i].bonusAct=true;
				}
			}
		}
	}



}

//Chequea impacto con laterales y esquinas de la barra
void barHit(){
	//Impacto contra la barra.

	GLfloat barY = -9.0;
	GLfloat barraneg = cBarra-tam/2;
	GLfloat barrapos = cBarra+tam/2;

	bool hasHit2=false;
	//Esquinas
	if (barrapos <= ballX-ballRadius && barrapos+2*ballRadius >= ballX && barY+2*ballRadius >= ballY && barY <= ballY-ballRadius){
		hasHit2=true;
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
	}else if (barraneg <= ballX && barrapos >= ballX  && barY<=ballY-ballRadius && ballY<barY+2*ballRadius && ySpeed<0 ){
			ySpeed = -ySpeed;
			hasHit2=true;
	}
	else if ( barraneg >= ballX+ballRadius && barraneg-2*ballRadius <= ballX   && barY-2*ballRadius<=ballY && ballY<=barY+2*ballRadius && xSpeed>0 ){
		xSpeed = -xSpeed;
		hasHit2=true;
		//printf("BOOM.");
	}else if ( barrapos < ballX-ballRadius && barrapos+2*ballRadius > ballX   && barY-2*ballRadius<=ballY && ballY<=barY+2*ballRadius && xSpeed<0 ){
		xSpeed = -xSpeed;
		hasHit2=true;
	}


	if (hasHit2){
		TCHAR h2[MAX_PATH];
		strcpy(h2,path);  
a		PlaySound(h2, NULL, SND_ASYNC );
	}
}


//Render principal
void render(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	checkVictory();
	glPushMatrix();
		dibujarParedes(); //Bordes de la pantalla
	glPopMatrix();

	glPushMatrix();
	 dibujarLadrillos(); //Dibuja los ladrillos y hace ciertas comprobaciones al respecto (como bonus y similares).
	glPopMatrix();
	
	//Dibujar barra
	glPushMatrix();
		//ejesCoordenada(1.0);   //Descomentar para obtener eje.
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
	else if (ballY < ballYMin){    //En caso de perder, realiza las accioens adecuadas.
		ballY = -9.0f+ballRadius;
		ballX = cBarra;
		isPause = true;
		xSpeed=0;
		ySpeed=0;
		if (!(victory))
			defeatedB=true;
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
	ballXMin=-10.5;
	ballXMax=10.5;
	ballYMax=7.5;
	ballYMin = -10.5;
}

void MoverBarraDerecha(){
	if (cBarra+tam/2 <= ballXMax){
	cBarra += 0.5;
	}
	if (isPause)
		ballX=cBarra;
}

void MoverBarraIzquierda(){
	if (cBarra-tam/2 >= ballXMin){
	cBarra -= 0.5;
	}
	if (isPause)
		ballX=cBarra;
}

//Activamos teclas para seleccionar, rotar y soltar la pelota (de ser posible).
void keyboard(unsigned char key, int x, int y)
	{
		if (!(defeatedB) && !(victory)){
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
	case 32: 
			xSpeed = 0.25f;
			ySpeed = 0.15f;
			isPause=false;
			break;
	default:
	break;
	}
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
	//Inicializadores
	setSpecials(); //Bloques especiales.
	initBlocks();  //Coordenadas de bloques y explosiones.
	setBonus();    //Bloques con bonus.
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


	//Tema de inicio
	GetCurrentDirectory(MAX_PATH,path);  
	TCHAR pathm[MAX_PATH];
	strcpy(pathm,path);  
	strcat(pathm, "\\mainsound.wav");
	PlaySound(pathm, NULL, SND_LOOP|SND_ASYNC );
	glutMainLoop();
	return 0;
}