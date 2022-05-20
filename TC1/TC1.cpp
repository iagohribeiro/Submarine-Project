#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "tinystr.h"
#include "tinyxml.h"
#include "TC1.h"

using namespace std;
int estadoMouse=0; // armazena se o mouse foi clicado com o botao esquerdo
int movimentoMouse =0; // Armazena o indice do vetor do circulo que podera ser movimentado
bool podeMover = false; // diz para o process Motion se ele pode movimentar o circulo
int xMouse; // armazena a posição x do mouse
int yMouse; // armazena a posição y do mouse

int openFileXML (char *NameFile)
{

	TiXmlDocument doc (NameFile);
	doc.LoadFile();
	
	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);
	const char* m_name;
	int largura, altura, raio;
	float corR, corG, corB;
	char* title;
	
	pElem=hDoc.FirstChildElement().Element();
	//Obter uma raiz valida
	if (pElem)
	{
		m_name = pElem->Value();
		hRoot = TiXmlHandle(pElem);
	}
	
	pElem = hRoot.FirstChild().Element();
	
	for(; pElem; pElem=pElem->NextSiblingElement())
	{
		if(strcmp (pElem->Value(), "janela") == 0)
		{
			pElem->QueryIntAttribute("largura", &janela.largura);
			pElem->QueryIntAttribute("altura", &janela.altura);

			TiXmlElement* subChild = hRoot.FirstChild().FirstChild().Element();
			
			for(; subChild; subChild = subChild->NextSiblingElement())
			{
				if(strcmp (subChild->Value(), "fundo") == 0)
				{
					subChild->QueryFloatAttribute("corR", &janela.corR);
					subChild->QueryFloatAttribute("corG", &janela.corG);
					subChild->QueryFloatAttribute("corB", &janela.corB);
				}
				else if(strcmp (subChild->Value(), "titulo") == 0)
				{
					janela.titulo = strdup(subChild->GetText());
				}
			}
		}
		else if(strcmp (pElem->Value(), "circulo") == 0)
		{
			pElem->QueryIntAttribute("raio", &circulo[0].raio);
			pElem->QueryFloatAttribute("corR", &circulo[0].corR);
			pElem->QueryFloatAttribute("corG", &circulo[0].corG);
			pElem->QueryFloatAttribute("corB", &circulo[0].corB);
		}
				
	}
	
}

void init (void)
{
  glClearColor (janela.corR, janela.corG, janela.corB, 0.0);
  glClear (GL_COLOR_BUFFER_BIT);

}

void DesenhaCirculo ()
{
    // Como tem-se vetor de circulos com um vetor de vertice para cada circulo, plota tudo de uma vez
    for (int j = 0; j<quantCirc; j++)
    {
    	if (circulo[j].desenhado)
    	{
    		glBegin(GL_POLYGON);
    		for (int i = 0; i < 360; i++)
    		{
        		glVertex2f(circulo[j].coordenadaX[i], circulo[j].coordenadaY[i]);
    		}
    		glEnd();
    	}
    	
    }
    
}

//Funcao responsavel para calcular os vertices de cada circulo
void calculaCirculo (float rx, float ry)
{

	circulo[quantCirc].desenhado = true;
	rx = rx / janela.largura;
	ry = (janela.altura - ry) / janela.altura;

	GLfloat theta = 0.0;
    for (int i = 0; i < 360; i++)   {
        GLfloat x = (GLfloat (circulo[0].raio)) * cosf(theta) / GLfloat (janela.largura);
        GLfloat y = (GLfloat (circulo[0].raio)) * sinf(theta) / GLfloat (janela.altura);
        circulo[quantCirc].coordenadaX[i] = x + rx;
        circulo[quantCirc].coordenadaY[i] = y + ry;

        theta = 2.0f * M_PI * GLfloat(i+1) / GLfloat(360); 
    }
    quantCirc++; //calculo a quantidade de circulos que foram adicionados na minha tela
}

//Funcao responsavel por recalcular o vertice do circulo que sera movimentado
void calculaCirculoMov (float rx, float ry)
{
	circulo[movimentoMouse].desenhado = true;

	GLfloat theta = 0.0;
    for (int i = 0; i < 360; i++)   {
        GLfloat x = (GLfloat (circulo[0].raio)) * cosf(theta) / GLfloat (janela.largura);
        GLfloat y = (GLfloat (circulo[0].raio)) * sinf(theta) / GLfloat (janela.altura);
        circulo[movimentoMouse].coordenadaX[i] = x + rx;
        circulo[movimentoMouse].coordenadaY[i] = y + ry;
        theta = 2.0f * M_PI * GLfloat(i+1) / GLfloat(360); 
    }
}

//Verifica se o novo ponto clicado na tela esta fora do circulo e pode ser desenhado
bool foraCirculo (float x, float y)
{

	x = (x/ janela.largura);
	y = (janela.altura - y) / janela.largura;

	bool estaFora = false;
	int i = 0;

	while (i < quantCirc)
	{
		float vx = (x - float(circulo[i].centroX)) * float(janela.largura);
		float vy = (y - float(circulo[i].centroY)) * float(janela.altura);
		float distancia = sqrt (pow(vx,2)+pow(vy, 2));
		
		//se a distancia dividido por dois for menor que o raio, eh porque a partir do ponto clicado, consigo desenhar outro circulo
		if ((distancia/2 <= circulo[0].raio) && float(circulo[i].desenhado))
		{
			estaFora = true;
		}
		i++;
	} 
	return estaFora;
}

//Verifica se ultrapassou o limite de um circulo desenhado
void apagaCirculo (float x, float y)
{

	int i = 0;

	while (i < quantCirc)
	{
		float vx = (x - float(circulo[i].centroX)) * float(janela.largura);
		float vy = (y - float(circulo[i].centroY)) * float(janela.altura);
		float distancia = sqrt (pow(vx,2)+pow(vy, 2));
		
		//se a distancia dividido por dois for menor que o raio, eh porque a partir do ponto clicado, consigo desenhar outro circulo
		if (distancia/2 < circulo[0].raio)
		{
			circulo[i].desenhado = false;
		}
		i++;
	} 
}

//testea se foi clicado dentro de um circulo com o botao direito
bool testaCirculo (float x, float y)
{

	x = (x/ janela.largura);
	y = (janela.altura - y) / janela.largura;

	bool estaDentro = false;
	int i = 0;

	while (i < quantCirc)
	{
		float vx = (x - float(circulo[i].centroX)) * float(janela.largura);
		float vy = (y - float(circulo[i].centroY)) * float(janela.altura);
		float distancia = sqrt (pow(vx,2)+pow(vy, 2));
		
		if (distancia <= circulo[0].raio)
		{
			estaDentro = true;
			movimentoMouse = i;
		}
		i++;
	} 
	return estaDentro;
}

void ProcessMotion(int x, int y)
{
	if (podeMover)
	{
		float xs = (float(x)/ float(janela.largura));
		float ys = (float(janela.altura) - float(y)) / float(janela.largura);

		circulo[movimentoMouse].centroX = xs;
		circulo[movimentoMouse].centroY = ys;

		xMouse = (xMouse/ janela.largura);
		yMouse = (janela.altura - yMouse) / janela.largura;

    	apagaCirculo(xs,ys);
    	calculaCirculoMov(xs, ys);
	}


    glutPostRedisplay();

}

void mouse (int button, int state, int x, int y)
{	
	switch(button)
    {
        case GLUT_LEFT_BUTTON:
        {
            
            if (state == GLUT_DOWN)
            {
            	estadoMouse = 1;
				xMouse = x;
				yMouse = y;
				circulo[quantCirc].centroX = float(xMouse) / float(janela.largura);
				circulo[quantCirc].centroY = float(janela.altura - yMouse) / float(janela.largura);
            }
            
            else if (state == GLUT_UP)
            {
                estadoMouse = 0;
            }
        } break;

        case GLUT_RIGHT_BUTTON:
        {
            if (state == GLUT_DOWN && testaCirculo(x, y))
            {
            	podeMover = true;
            	estadoMouse = 0;
            	xMouse = x-circulo[movimentoMouse].centroX;
            	yMouse = y-circulo[movimentoMouse].centroY;
            }
            else if (state == GLUT_UP)
            {
            	podeMover = false;
            	estadoMouse = 0;
            }
            

        } break;
     }

    glutPostRedisplay();
}

//Recalcula o tamanho da tela para ficar proporcional
void Reshape(int Width, int Height)
{
    glViewport(0, 0, (GLsizei) Width, (GLsizei) Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1.0, 0, 1.0, -1.0, 1.0);    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    janela.largura = Width;
    janela.altura = Height;
}

void display (void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(circulo[0].corR, circulo[0].corG, circulo[0].corB);
	if (estadoMouse && (quantCirc == 0))
	{
		circulo[quantCirc].centroX = float(xMouse) / float(janela.largura);
		circulo[quantCirc].centroY = float(janela.altura - yMouse) / float(janela.largura);

		calculaCirculo (xMouse,yMouse);
	}
	else if (estadoMouse && !foraCirculo(xMouse, yMouse))
	{
		calculaCirculo (xMouse,yMouse);

	}
	DesenhaCirculo();
	glFlush ();
}

int main (int argc, char **argv){

	openFileXML (argv[1]);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (janela.largura, janela.altura);
	glutCreateWindow (janela.titulo);
	glutReshapeFunc(Reshape);
	glutDisplayFunc (display);
	glutMouseFunc (mouse);
	glutMotionFunc(ProcessMotion);

	init();
	
	glutMainLoop();

	return 0;
}
