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
GLfloat ballCoords[4][2] = {0};
bool isPause = true;
TCHAR path[MAX_PATH];

int refreshMillis = 30;

//Projection clipping area
GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;


float rbg(float i){
	return i/255;
}


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
			if (i>150)
				glColor3f(rbg(128),rbg(128),rbg(128));
			double angle = 2* PI * (i) / 360;
			double x = cos(angle);
			double y = sin(angle);
			glVertex2d(x*ballRadius,y*ballRadius);
		}
	glEnd();

	/*
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

void defeated(){


	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(rbg(100),rbg(100),rbg(100),1);
	glBegin(GL_POLYGON);
		glVertex2f(-13,-13);
		glVertex2f(-13,13);
		glVertex2f(13,13);
		glVertex2f(13,-13);

	glEnd();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

}

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

int max_fila = 7;
int max_columna = 5;

float ladrilloXneg = -9.5;
float ladrilloXpos = -7.5;
float ladrilloYpos = 9.0f;
float ladrilloYneg = 8.5f;

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

void dibujarParedes(){
glColor3f(1.0f,0.0,1.0f);

	glTranslated(0,1,0);
	glBegin(GL_LINE_LOOP);
		//pared izquierda inferior
		glVertex2f(-11,-9.5);
		//glVertex2f(-12,-10);
		//pared izquierda externa
		glVertex2f(-12,-9.5);
		glVertex2f(-12,8.5);
		//pared arriba externa
		//glVertex2f(-12,8.5);
glColor3f(rbg(75),rbg(0),rbg(130));

		glVertex2f(12,8.5);
		//pared derecha externa
		//glVertex2f(12,8.5);
		glVertex2f(12,-9.5);
		//pared derecha inferior
		//glVertex2f(12,-10);
		glVertex2f(11,-9.5);
		//pared derecha interna
		//glVertex2f(11,-10);
		glVertex2f(11,7.5);
		//pared arriba interna
		//glVertex2f(11,7.5);
glColor3f(rbg(186),rbg(85),rbg(211));
		glVertex2f(-11,7.5);
		//pared izquierda interna
		//glVertex2f(-11,7.5);
		glVertex2f(-11,-9.5);


	glEnd();
}

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

void initBlocks(){

float ladrilloXn = -9.5;
float ladrilloXp = -7.5;
float ladrilloYp = 7.0f;
float ladrilloYn = 6.3f;
int counter = 0;
time_t t;
std::srand((unsigned) time(&t));
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
void dibujarFire(firework f){
	glColor3f(0,rbg(150),rbg(0));


	cout<<"dibujo esa cosa"<<endl;
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
								if (abs(fireworks[k].xs[w]+fireworks[k].x)>6) {
									fireworks[k].xs[w]=0;
								}
								if (abs(fireworks[k].ys[w]+fireworks[k].y)>6) {
									fireworks[k].ys[w]=0;
								}

								fireworks[k].xs[w]+=fireworks[k].xs[w]/50;
								fireworks[k].ys[w]+=fireworks[k].ys[w]/50;
							}
							bool isOver=false;
							for (int w=0; w<10; w++){
								isOver=isOver && fireworks[k].xs[w]==0 && fireworks[k].ys[w] == 0;
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
					cout<<"Menos tama�o"<<endl;
					break;
				case(2):
					xSpeed+= xSpeed * 40 / 100;
					ySpeed+= ySpeed * 40 / 100;
					cout<<"M�s velocidad"<<endl;
					break;
				case(3):
					tam+= tam * 15 / 100;
					cout<<"M�s Tama�o"<<endl;
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
		else {		printf("AAAAAAAAAAAAAAAA");
} //> < no tiene sentido.
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
		else {		printf("AAAAAAAAAAAAAAAA");
} //< > no tiene sentido.
	}else if (barraneg <= ballX && barrapos >= ballX  && barY<=ballY-ballRadius && ballY<barY+2*ballRadius && ySpeed<0 ){
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
		dibujarParedes();
	glPopMatrix();

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
		ballY = -9.0f+ballRadius;
		ballX = cBarra;
		isPause = true;
		xSpeed=0;
		ySpeed=0;
		defeated();
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
	case 32: 
			xSpeed = 0.25f;
			ySpeed = 0.15f;
			isPause=false;
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
	initBlocks();
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