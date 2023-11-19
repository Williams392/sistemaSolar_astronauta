#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <iostream>

// Agrengando las nuevas librerias:
#include "texturas/RgbImage.h"
#include <vector>

using namespace std;

//float camaraX = -10; original
//float camaraY = 40; original
//float camaraZ = 85; original
float camaraX = -15;
float camaraY = 40;
float camaraZ = 80;

float radio = 66.72 * 2;;
float angulo = 2.23 * 2;;
float centroY = 10;

struct Estrella {
	float x, y, z;
};
std::vector<Estrella> estrellas; // me sale mal :(

// ----------------------------------------------------------------------------------

GLuint texturas[15]; // un arreglo de 15 imagenes.
GLUquadric* quad;

// 1. CargarTexturasDesdeArchivo:
void loadTexturesFromFile(const char* filename, int index) { // filename -> Nombre del archivo
	RgbImage theTextMap(filename);
	glGenTextures(1, &texturas[index]);
	glBindTexture(GL_TEXTURE_2D, texturas[index]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, theTextMap.GetNumCols(),
		theTextMap.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE, theTextMap.ImageData());
}

// 2: cargarImagenes al metodo -> main
void cargarImagenes() {

	loadTexturesFromFile("texturas/fondo_1.bmp", 0);
	loadTexturesFromFile("texturas/moon.bmp", 1);

	loadTexturesFromFile("texturas/tierra.bmp", 2);
	loadTexturesFromFile("texturas/marte.bmp", 3);
	loadTexturesFromFile("texturas/sol.bmp", 4);
	loadTexturesFromFile("texturas/saturno.bmp", 5);

	// espacio:
	loadTexturesFromFile("texturas/fondo/back.bmp", 6);
	loadTexturesFromFile("texturas/fondo/bottom.bmp", 7);
	loadTexturesFromFile("texturas/fondo/front.bmp", 8);
	loadTexturesFromFile("texturas/fondo/left.bmp", 9);
	loadTexturesFromFile("texturas/fondo/right.bmp", 10);
	loadTexturesFromFile("texturas/fondo/top.bmp", 11);

	// Aliens:
	loadTexturesFromFile("Texturas/piedra.bmp", 12);

}

// ----------------------------------------------------------------------------------

// Ampliando vision de 80 a 40:
void iniciarVentana(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, (float)w / (float)h, 1, 500); // 60  - 200 original
}
void inicializarLuces() {

	GLfloat luz_ambiente[] = { 0.2,0.2,0.2,1 };
	GLfloat luz_difusa[] = { 0.2,0.2,0.2,1 };
	GLfloat luz_especular[] = { 0.1,0.1,0.1,1 };

	float reflejo[] = { 0,0,0,1 };
	float posicionLuz[] = { 0,70,0,1 };
	int sombra = 128;

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luz_ambiente);
	glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMaterialfv(GL_FRONT, GL_SPECULAR, reflejo);
	glMateriali(GL_FRONT, GL_SHININESS, sombra);
	glLightfv(GL_LIGHT0, GL_POSITION, posicionLuz);

	//Luces Piso
	GLfloat light_ambient1[] = { 0.1,0.1,0.1,1 };
	GLfloat light_diffuse1[] = { 0.1,0.1,0.1,1 };
	GLfloat light_specular1[] = { 0.1,0.1,0.1,1 };

	float posicionLuz1[] = { -50,0,0,1 };
	float posicionLuz2[] = { 0,0,50,1 };
	float posicionLuz3[] = { 50,0,0,1 };
	float posicionLuz4[] = { 0,0,-50,1 };

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT1, GL_POSITION, posicionLuz1);

	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT2, GL_POSITION, posicionLuz2);

	glEnable(GL_LIGHT3);
	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT3, GL_POSITION, posicionLuz3);

	glEnable(GL_LIGHT4);
	glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT3, GL_POSITION, posicionLuz4);

}



/*--------------------------------------------------------------
# Estructura - Project:
--------------------------------------------------------------*/

/*void ejes() {
	glPushMatrix();
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(50, 0, 0);

	glColor3ub(0, 255, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 50, 0);

	glColor3ub(0, 0, 255);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 50);
	glEnd();
	glPopMatrix();
}*/
/*void piso() { // Modificando Piso con imagen:

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[1]);

	glColor3ub(255, 255, 255); // IMPORTANTE: Siempre trabajar con el color blanco en Texturas.
	glPushMatrix();
	    glTranslated(0, -0.1, 0);
		glBegin(GL_POLYGON);
		glTexCoord2f(0, 1); glVertex3d(-90, 0, -90);
		glTexCoord2f(1, 1); glVertex3d(90, 0, -90);
		glTexCoord2f(1, 0); glVertex3d(90, 0, 90);
		glTexCoord2f(0, 0); glVertex3d(-90, 0, 90);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
} */

void piso_luna() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[1]);
	glColor3ub(255, 255, 255);

	glPushMatrix();
	    glTranslatef(0, -120, 45); // x, y, z

		quad = gluNewQuadric();
		gluQuadricTexture(quad, 1);
		gluSphere(quad, 120, 50, 50); // perfecto
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
void paredes() {
	glEnable(GL_TEXTURE_2D);
	glTranslatef(0, -100, 0);

	// Pared frontal
	glBindTexture(GL_TEXTURE_2D, texturas[8]); // Textura del frente
	glColor3ub(255, 255, 255);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3d(-200, 0, -200);
	glTexCoord2f(1, 0); glVertex3d(200, 0, -200);
	glTexCoord2f(1, 1); glVertex3d(200, 200, -200);
	glTexCoord2f(0, 1); glVertex3d(-200, 200, -200);
	glEnd();

	// Pared trasera
	glBindTexture(GL_TEXTURE_2D, texturas[6]); // Textura de atrás
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3d(-200, 0, 200);
	glTexCoord2f(1, 0); glVertex3d(200, 0, 200);
	glTexCoord2f(1, 1); glVertex3d(200, 200, 200);
	glTexCoord2f(0, 1); glVertex3d(-200, 200, 200);
	glEnd();

	// Pared izquierda
	glBindTexture(GL_TEXTURE_2D, texturas[9]); // Textura de la izquierda
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3d(-200, 0, -200);
	glTexCoord2f(1, 0); glVertex3d(-200, 0, 200);
	glTexCoord2f(1, 1); glVertex3d(-200, 200, 200);
	glTexCoord2f(0, 1); glVertex3d(-200, 200, -200);
	glEnd();

	// Pared derecha
	glBindTexture(GL_TEXTURE_2D, texturas[10]); // Textura de la derecha
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3d(200, 0, -200);
	glTexCoord2f(1, 0); glVertex3d(200, 0, 200);
	glTexCoord2f(1, 1); glVertex3d(200, 200, 200);
	glTexCoord2f(0, 1); glVertex3d(200, 200, -200);
	glEnd();

	// Pared superior
	glBindTexture(GL_TEXTURE_2D, texturas[11]); // Textura de arriba
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3d(-200, 200, -200);
	glTexCoord2f(1, 0); glVertex3d(200, 200, -200);
	glTexCoord2f(1, 1); glVertex3d(200, 200, 200);
	glTexCoord2f(0, 1); glVertex3d(-200, 200, 200);
	glEnd();

	// Pared inferior
	glBindTexture(GL_TEXTURE_2D, texturas[7]); // Textura de abajo
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3d(-200, 0, -200);
	glTexCoord2f(1, 0); glVertex3d(200, 0, -200);
	glTexCoord2f(1, 1); glVertex3d(200, 0, 200);
	glTexCoord2f(0, 1); glVertex3d(-200, 0, 200);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0); // Desactivar la textura
	glDisable(GL_TEXTURE_2D);
} 


// Cielo:
/*void esfera_universo() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[6]);
	glColor3ub(255, 255, 255);

	glPushMatrix();
	    glTranslatef(0, 0, 0); // x, y, z

	    quad = gluNewQuadric();
	    gluQuadricTexture(quad, 1);
	    gluSphere(quad, 140, 50, 50); // perfecto
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}  */

/*--------------------------------------------------------------
# Planetas:
--------------------------------------------------------------*/
float angleTierra = 0.0;
float angleMarte = 0.0;
float angleSaturno = 0.0;
float factorDistanciaTierra = 1.0;  // Factor de distancia para la Tierra
float factorDistanciaMarte = 1.8;   // Factor de distancia para Marte
float factorDistanciaSaturno = 2.5; // Factor de distancia para Saturno

void sol() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[4]);
	glColor3ub(255, 255, 255);

	glPushMatrix();
	    glTranslatef(-60, 40, -50.0); // Posición del sol
		glRotatef(angleTierra, 0.0, 1.0, 0.0); // Rotación del sol
		gluSphere(quad, 20, 50, 50);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}
void planetaTierra() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[2]);
	glColor3ub(255, 255, 255);

	glPushMatrix();
	    glTranslatef(-60 + 50 * factorDistanciaTierra * cos(angleTierra * 0.0175), 40, -50.0 + 50 * factorDistanciaTierra * sin(angleTierra * 0.0175));
	
	    glRotatef(angleTierra, 0.0, 1.0, 0.0); // Rotación propia de la Tierra
	    gluSphere(quad, 5, 50, 50);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}
void marte() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[3]);
	glColor3ub(255, 255, 255);

	glPushMatrix();
	    glTranslatef(-60 + 35 * factorDistanciaMarte * cos(angleMarte * 0.0175), 40, -50.0 + 35 * factorDistanciaMarte * sin(angleMarte * 0.0175));

		glRotatef(angleMarte, 0.0, 1.0, 0.0); // Rotación propia de Marte
		gluSphere(quad, 3.5, 50, 50);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}
void planetaSaturno() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[5]);
	glColor3ub(255, 255, 255);

	glPushMatrix();

	    glTranslatef(-60 + 30 * factorDistanciaSaturno * cos(angleSaturno * 0.0175), 40, -50.0 + 30 * factorDistanciaSaturno * sin(angleSaturno * 0.0175));

	    glRotatef(angleSaturno, 0.0, 1.0, 0.0); // Rotación propia de Saturno
	    gluSphere(quad, 8, 50, 50);

	    // anillo 1:
	    glPushMatrix();
	        glScaled(1, 0.5, 1);
			glRotatef(-115, -1, 0.0, 0.0);
		    glColor3f(0.5, 0.5, 0.5);
		    glutSolidTorus(0.8, 12, 50, 50);
     	glPopMatrix();
	    // anillo 2:
	    glPushMatrix();
	        glScaled(1.20, 0.5, 1.20);
			glRotatef(-115, -1, 0.0, 0.0);
			glColor3f(255, 0.5, 0.5);
			glutSolidTorus(0.8, 12, 50, 50);
	    glPopMatrix();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
void planetas() {
	piso_luna();
	sol();
	planetaTierra();
	marte();
	planetaSaturno();
}


/*--------------------------------------------------------------
# Astronauta:
--------------------------------------------------------------*/
void bota(double posX) {
	glPushMatrix();
	    glTranslated(posX, 0.2, 0);
		glScaled(1, 0.9, 1.5);
		glutSolidSphere(1, 50, 50);
	glPopMatrix();
	glPushMatrix();
	     glTranslated(posX, 0.2, -0.46);
		 glScaled(0.9, 1, 1);
		 glRotated(-90, 1, 0, 0);
		 gluCylinder(gluNewQuadric(), 1, 1, 0.8, 50, 50);
	glPopMatrix();
}
void pierna(double posX) {
	glPushMatrix();
	    glTranslated(posX, 1, -0.46);
		glRotated(-90, 1, 0, 0);
		gluCylinder(gluNewQuadric(), 1.04, 1.04, 5, 50, 50);
	glPopMatrix();
}
void cuerpo() {
	glPushMatrix();
	glTranslated(0, 7, -0.46);
	glScaled(1.1, 1, 0.7);
	glRotated(-90, 1, 0, 0);
	glutSolidSphere(2, 50, 50);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 6, -0.46);
	glScaled(1, 1, 0.6);
	glRotated(-90, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 2.3, 2.6, 4, 50, 50);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 9.6, -0.46);
	glScaled(1.15, 1, 0.7);
	glRotated(-90, 1, 0, 0);
	glutSolidSphere(2.3, 50, 50);
	glPopMatrix();
}
void casco() {
	glPushMatrix();
	glTranslated(0, 13.6, 0.24);
	glScaled(1, 1, 0.5);
	glRotated(10, 1, 0, 0);
	glutSolidTorus(1.06, 1.08, 50, 50);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 13.6, -0.6);
	glutSolidSphere(2.2, 50, 50);
	glPopMatrix();
	glColor3ub(0, 0, 0);
	glPushMatrix();
	glTranslated(0, 13.58, -0.3);
	glutSolidSphere(2.04, 50, 50);
	glPopMatrix();
}
void brazo(double posX, int rot, int romHombro) {
	glPushMatrix();
	glTranslated(posX, 10.6, 0);
	glRotated(romHombro, 0, 0, 1);
	glScaled(1, 1.5, 1);
	glutSolidSphere(0.6, 50, 50);
	glPopMatrix();
	glPushMatrix();
	glTranslated(posX, 10.6, 0);
	glRotated(rot, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.7, 0.68, 2.7, 50, 50);
	glPopMatrix();
}
void antebrazo(double posX) {
	glPushMatrix();
	glTranslated(posX, 8.2, 0);
	glRotated(90, 1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.68, 0.65, 2.5, 50, 50);
	glPopMatrix();
	glPushMatrix();
	glTranslated(posX, 5.8, 0);
	glScaled(1, 1, 1);
	glutSolidSphere(0.5, 50, 50);
	glPopMatrix();
}
void astronauta() {
	//glTranslated(0, 0, 9);
	glPushMatrix();
	    glTranslated(-5, 0, 45);

	    glColor3ub(255, 255, 255);
		bota(1.2);
		bota(-1.2);
		pierna(1.2);
		pierna(-1.2);
	    cuerpo();
		brazo(-2, -20, -60);
		brazo(2, 20, 60);
		antebrazo(3);
		antebrazo(-3);
		casco();
	glPopMatrix();
}

/*--------------------------------------------------------------
# Aliens:
--------------------------------------------------------------*/
float moverAl = 0;
float velocidad = 0.5;
int ladoX = 1;
float posA = 0;

void craneoalien() {
	glPushMatrix();
	glPushMatrix();
		glColor3ub(118, 172, 26);
		glTranslated(0, 12, 0);
		glScaled(1.5, 2, 1);
		glRotated(90, 1, 0, 0);
		glutSolidSphere(2, 20, 20);
	glPopMatrix();
}
void cuerpoalien() {
	glPushMatrix();
	glPushMatrix();
	glColor3ub(118, 172, 26);
	glTranslated(0, 6, 0);
	glScaled(1, 8, 1);
	glRotated(90, 1, 0, 0);
	glutSolidSphere(0.8, 20, 20);
	glPopMatrix();
}
void brazoalien() {
	glPushMatrix();
	glPushMatrix();
	glColor3ub(118, 172, 26);
	glRotated(45, 0, 0, 1);
	glTranslated(2, 6, 0);
	glScaled(1, 4.5, 1);
	glutSolidSphere(0.7, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(118, 172, 26);
	glRotated(-45, 0, 0, 1);
	glTranslated(-2, 6, 0);
	glScaled(1, 4.5, 1);
	glutSolidSphere(0.7, 20, 20);
	glPopMatrix();

	glPopMatrix();
}
void ojoalien() {
	glPushMatrix();
	//glTranslated(0, moverAl, 0);
	glTranslated(0, posA, 0);
	glPushMatrix();
	glColor3ub(21, 46, 16);
	glRotated(1.05, 1, 0, 0);
	glTranslated(1.35, 12.15, 1.85);
	glScaled(1.5, 2.6, 1);
	glutSolidSphere(0.5, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(21, 46, 16);
	glRotated(1.05, 1, 0, 0);
	glTranslated(-1.35, 12.15, 1.85);
	glScaled(1.5, 2.6, 1);
	glutSolidSphere(0.5, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(191, 29, 0);
	glRotated(-5, 1, 0, 0);
	glTranslated(0, 10, 2.5);
	glScaled(1.5, 2.6, 1);
	glutSolidSphere(0.5, 20, 20);
	glPopMatrix();

	glPopMatrix();
}
void antenas() {
	glPushMatrix();
	//glTranslated(0, moverAl, 0);
	glTranslated(0, posA, 0);
	glPushMatrix();
	glColor3ub(118, 172, 26);
	glRotated(40, 0, 0, 1);
	glTranslated(9.8, 14, 0);
	glScaled(0.45, 4.5, 0.45);
	glutSolidSphere(0.7, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3ub(118, 172, 26);
	glRotated(-40, 0, 0, 1);
	glTranslated(-9.8, 14, 0);
	glScaled(0.45, 4.5, 0.45);
	glutSolidSphere(0.7, 20, 20);
	glPopMatrix();


	glPopMatrix();
}
void piedra() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturas[12]);
	glColor3ub(255, 255, 255);

	glPushMatrix();
	glPushMatrix();
	//glTranslated(0, 0.75, 0);
	glTranslated(15, 0.75, 30);
	glScaled(2.85, 1.55, 1.4);
	glPushMatrix();
	quad = gluNewQuadric();
	gluQuadricTexture(quad, 1);
	gluSphere(quad, 5, 50, 50);
	glPopMatrix();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}
void alien() {
	//glPushMatrix();
	//glTranslated(0, moverAl, 0);
	if (posA > 8) {
		ladoX = -1;
	}
	if (posA < -18) {
		ladoX = 1;
	}
	posA += velocidad * ladoX;

	glColor3ub(255, 0, 0);
	glPushMatrix();
	glTranslated(0, posA, 0);
	cuerpoalien();
	craneoalien();
	   brazoalien();
	   ojoalien();
	   antenas();
	glPopMatrix();
}

void Alien() {

	glPushMatrix();
	   glTranslated(15, 0.75, 30);
	   glRotated(-10, 0, 0, 1);
	   alien();
	   piedra();
	glPopMatrix();
}


// -------------------------------------------------------------

void dibujar() {

	inicializarLuces();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(radio * cos(angulo), camaraY, radio * sin(angulo), 0, centroY, 0, 0, 1, 0);
	//glClearColor(255 / 255.0, 255 / 255.0, 208 / 255.0, 1); // amarillos palido:
	//glClearColor(0.2, 0.2, 0.15, 1); // Color de fondo oscuro y pálido
	glClearColor(255, 255, 255, 1);
	glPushMatrix();

	    //piso();

	    // -------------
	    // Project:
	    // -------------

		//ejes();
	    planetas();
	    astronauta();
	   
		Alien();

		paredes(); // siempre ultimo

	glPopMatrix();
	glutSwapBuffers();

}

void timer(int t) {
	angleTierra += 1.0; // Ajusta la velocidad de rotación de la Tierra
	angleMarte += 0.7;  // Ajusta la velocidad de rotación de Marte
	angleSaturno += 0.3; // Ajusta la velocidad de rotación de Saturno


	glutPostRedisplay();
	glutTimerFunc(20, timer, 0);
}

void teclado(int tecla, int x, int y) {

	//cout << "Angulo: " << angulo << " Camara Y: " << camaraY << " Radio: "
		//<< radio << " CentroY: " << centroY << "\n";

	switch (tecla) {
	case 100:
		angulo += 0.05;
		break;
	case 102:
		angulo -= 0.05;
		break;
	case 101:
		camaraY += 1;
		break;
	case 103:
		camaraY -= 1;
		break;
	case 105:
		radio += 2;
		break;
	case 104:
		radio -= 2;
		break;
	case 106:
		centroY += 2;
		break;
	case 107:
		centroY -= 2;
		break;
	}

}
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1009, 711);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Project - Astraonauta - T4");
	cargarImagenes();
	glutReshapeFunc(iniciarVentana);
	glutDisplayFunc(dibujar);
	glutSpecialFunc(teclado);
	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	return 0;
}
