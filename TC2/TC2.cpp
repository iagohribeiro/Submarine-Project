/*	Iagoh Ribeiro Lima - 2014100678
		Computacao Grafica 2018/2
*/

#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "tinystr.h"
#include "tinyxml.h"
#include "TC2.h"

using namespace std;
int estadoMouse=0; // armazena se o mouse foi clicado com o botao esquerdo
int movimentoMouse =0; // Armazena o indice do vetor do circulo que podera ser movimentado
bool podeMover = false; // diz para o process Motion se ele pode movimentar o circulo
int xMouse; // armazena a posição x do mouse
int yMouse; // armazena a posição y do mouse
char nome[256], tipo[32], caminho[256]; //Strings que vao receber o caminho do svg
char svgCaminho[544]; // String que vao receber o caminho do svg concatenado
int numCircl = 0; // Quantidade de circulos no arquivo SVG
int blackCircle[100]; // Armazena os indices de circulos pretos do vetor de circulos
int greenCircle[100]; // Armazena os indices de circulos verdes do vetor de circulos
int redCircle[100]; // Armazena os indices de circulos vermelhos do vetor de circulos
int blueCircle[100]; // Armazena os indices de circulos azuis do vetor de circulos
int quantBlue=0, quantGreen=0, quantRed=0, quantBlack=0;
bool atualiza = true;

void openFileSVG()
{
	TiXmlDocument doc (svgCaminho);
	
	doc.LoadFile();
	
	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);
	const char* m_name;

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
		
		if(strcmp (pElem->Value(), "circle") == 0)
		{
			pElem->QueryFloatAttribute("cx", &circulo[numCircl].centroX);
			pElem->QueryFloatAttribute("cy", &circulo[numCircl].centroY);
			pElem->QueryIntAttribute("r", &circulo[numCircl].raio);
			pElem->QueryIntAttribute("id", &circulo[numCircl].id);

			strncpy(circulo[numCircl].fill, pElem->Attribute("fill"), 50);

			if(strcmp (circulo[numCircl].fill, "blue") == 0)
			{
				circulo[numCircl].corR = 0.0f;
				circulo[numCircl].corG = 0.0f;
				circulo[numCircl].corB = 1.0f;
				blueCircle[quantBlue] = numCircl;
				quantBlue++;
			}
			else if (strcmp (circulo[numCircl].fill, "red") == 0)
			{
				circulo[numCircl].corR = 1.0f;
				circulo[numCircl].corG = 0.0f;
				circulo[numCircl].corB = 0.0f;
				redCircle[quantRed] = numCircl;
				quantRed++;	
			}
			else if (strcmp (circulo[numCircl].fill, "green") == 0)
			{
				circulo[numCircl].corR = 0.0f;
				circulo[numCircl].corG = 0.5f;
				circulo[numCircl].corB = 0.0f;
				greenCircle[quantGreen] = numCircl;
				quantGreen++;		
			}
			else if (strcmp (circulo[numCircl].fill, "black") == 0)
			{
				circulo[numCircl].corR = 0.0f;
				circulo[numCircl].corG = 0.0f;
				circulo[numCircl].corB = 0.0f;
				blackCircle[quantBlack] = numCircl;
				quantBlack++;		
			}

			numCircl++;
		}

	}
	janela.altura = 2*circulo[blueCircle[0]].raio;
	janela.largura = 2*circulo[blueCircle[0]].raio;
}
int openFileXML (char *NameFile)
{

	TiXmlDocument doc (NameFile);
	doc.LoadFile();
	
	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);
	const char* m_name;


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
		if(strcmp (pElem->Value(), "arquivoDaArena") == 0)
		{

			TiXmlElement* subChild = hRoot.FirstChild().FirstChild().Element();
			
			for(; subChild; subChild = subChild->NextSiblingElement())
			{
				if (strcmp (subChild->Value(), "nome") == 0)
				{
					strcpy(nome, subChild->GetText());			
				}
				else if (strcmp (subChild->Value(), "tipo") == 0)
				{
					strcpy(tipo, subChild->GetText());			
				}
				else if(strcmp (subChild->Value(), "caminho") == 0)
				{
					strcpy(caminho, subChild->GetText());
					printf("Caminho SVG: %s\n", caminho);
				}
			}
		}

	}

	strcat (svgCaminho, caminho);
	strcat (svgCaminho, nome);
	strcat (svgCaminho, ".");
	strcat (svgCaminho, tipo);

	openFileSVG();
}

void init (void)
{
  glClearColor (1.0, 1.0, 1.0, 0.0);
  glClear (GL_COLOR_BUFFER_BIT);

}

void DesenhaCirculo ()
{
    // Como tem-se vetor de circulos com um vetor de vertice para cada circulo, plota tudo de uma vez
    	
    for (int j = 0; j<quantCirc; j++)
    {
    	if (strcmp(circulo[j].fill, "red") != 0)
    	{

    		glColor3f(circulo[j].corR, circulo[j].corG, circulo[j].corB);
    		glBegin(GL_POLYGON);
    		for (int i = 0; i < 360; i++)
    		{
        		glVertex2f(circulo[j].coordenadaX[i], circulo[j].coordenadaY[i]);
    		}
    		glEnd();
    	}
    	
    }

    for (int j = 0; j<quantRed; j++)
    {

		glColor3f(circulo[redCircle[j]].corR, circulo[redCircle[j]].corG, circulo[redCircle[j]].corB);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++)
		{
    		glVertex2f(circulo[redCircle[j]].coordenadaX[i], circulo[redCircle[j]].coordenadaY[i]);
		}
		glEnd();
    	
    }
    
}

//Funcao responsavel para calcular os vertices de cada circulo
void calculaCirculo (float rx, float ry)
{

	circulo[quantCirc].desenhado = true;
	rx = rx / janela.largura;
	ry = (janela.altura - ry) / janela.altura;
	circulo[quantCirc].centroX = rx;
	circulo[quantCirc].centroY = ry;

	GLfloat theta = 0.0;
    for (int i = 0; i < 360; i++)   {
        GLfloat x = (GLfloat (circulo[quantCirc].raio)) * cosf(theta) / GLfloat (janela.largura);
        GLfloat y = (GLfloat (circulo[quantCirc].raio)) * sinf(theta) / GLfloat (janela.altura);
        circulo[quantCirc].coordenadaX[i] = x + rx;
        circulo[quantCirc].coordenadaY[i] = y + ry;

        theta = 2.0f * M_PI * GLfloat(i+1) / GLfloat(360); 
    }
    quantCirc++; //calculo a quantidade de circulos que foram adicionados na minha tela
}

//Funcao responsavel por recalcular o vertice do circulo que sera movimentado
void calculaCirculoMov (float rx, float ry)
{


	GLfloat theta = 0.0;
    for (int i = 0; i < 360; i++)   {
        GLfloat x = (GLfloat (circulo[greenCircle[0]].raio)) * cosf(theta) / GLfloat (janela.largura);
        GLfloat y = (GLfloat (circulo[greenCircle[0]].raio)) * sinf(theta) / GLfloat (janela.altura);
        circulo[greenCircle[0]].coordenadaX[i] = x + rx;
        circulo[greenCircle[0]].coordenadaY[i] = y + ry;
        theta = 2.0f * M_PI * GLfloat(i+1) / GLfloat(360); 
    }
}

//Verifica se encostou em uma ilha ou inimigo
void verificaInimigo (float x, float y)
{

	
	int i;
	if(!circulo[greenCircle[0]].submerso)
	{
		i=0;

		while (i < quantRed)
		{
			float vx = (x - float(circulo[redCircle[i]].centroX)) * float(janela.largura);
			float vy = (y - float(circulo[redCircle[i]].centroY)) * float(janela.altura);
			float distancia = sqrt (pow(vx,2)+pow(vy, 2));

		//se a distancia dividido por dois for menor que o raio, eh porque a partir do ponto clicado, consigo desenhar outro circulo
			if (distancia < circulo[redCircle[i]].raio + circulo[greenCircle[0]].raio)
			{
				atualiza = false;
			}
			i++;
		} 
	}

	i=0;

	while (i < quantBlack)
	{
		float vx = (x - float(circulo[blackCircle[i]].centroX)) * float(janela.largura);
		float vy = (y - float(circulo[blackCircle[i]].centroY)) * float(janela.altura);
		float distancia = sqrt (pow(vx,2)+pow(vy, 2));
		
		//se a distancia dividido por dois for menor que o raio, eh porque a partir do ponto clicado, consigo desenhar outro circulo
		if (distancia < circulo[blackCircle[i]].raio + circulo[greenCircle[0]].raio)
		{
			atualiza = false;
		}
		i++;
	} 
	
	i=0;
	
	while (i < quantBlue)
	{
		float vx = (x - float(circulo[blueCircle[i]].centroX)) * float(janela.largura);
		float vy = (y - float(circulo[blueCircle[i]].centroY)) * float(janela.altura);
		float distancia = sqrt (pow(vx,2)+pow(vy, 2));
		
		//se a distancia dividido por dois for menor que o raio, eh porque a partir do ponto clicado, consigo desenhar outro circulo
		if (distancia > circulo[blueCircle[i]].raio - circulo[greenCircle[0]].raio)
		{
			atualiza = false;
		}
		i++;
	} 
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


	int normalizacaoX = circulo[blueCircle[0]].centroX-circulo[quantCirc].raio;
	int normalizacaoY = circulo[blueCircle[0]].centroY-circulo[quantCirc].raio;
	
	while (quantCirc < numCircl){
		calculaCirculo (circulo[quantCirc].centroX-normalizacaoX,circulo[quantCirc].centroY-normalizacaoY);
	}

	DesenhaCirculo();
	glFlush ();
}
void verificaTeclas(int Value)
{

    if (teclas.Keyboard['a'])
    {
    	verificaInimigo (circulo[greenCircle[0]].centroX-0.0015, circulo[greenCircle[0]].centroY);
    	if (atualiza)
        	circulo[greenCircle[0]].centroX -= 0.0015;
        else
        	atualiza = true;
    }
    
    if (teclas.Keyboard['s'])
    {
    	verificaInimigo (circulo[greenCircle[0]].centroX, circulo[greenCircle[0]].centroY-0.0015);
    	if (atualiza)
        	circulo[greenCircle[0]].centroY -= 0.0015;
        else
        	atualiza = true;
    }
    
    if (teclas.Keyboard['w'])
    {
    	verificaInimigo (circulo[greenCircle[0]].centroX, circulo[greenCircle[0]].centroY+0.0015);
    	if (atualiza)
        	circulo[greenCircle[0]].centroY += 0.0015;
        else
        	atualiza = true;
    }
    
    if (teclas.Keyboard['d'])
    {
    	verificaInimigo (circulo[greenCircle[0]].centroX+0.0015, circulo[greenCircle[0]].centroY);
    	if (atualiza)
        	circulo[greenCircle[0]].centroX += 0.0015;
        else
        	atualiza = true;
    }
    if (teclas.Keyboard['u'])
    {
    	if(circulo[greenCircle[0]].submerso)
    	{
    		circulo[greenCircle[0]].submerso = false;
    		circulo[greenCircle[0]].raio = circulo[greenCircle[0]].raio*2;
        	circulo[greenCircle[0]].corR = 0.0f;
        	circulo[greenCircle[0]].corG = 0.5f;
        	circulo[greenCircle[0]].corB = 0.0f;
    	}
        else
        {
        	circulo[greenCircle[0]].submerso = true;
        	circulo[greenCircle[0]].raio = circulo[greenCircle[0]].raio/2;
        	circulo[greenCircle[0]].corR = 0.0f;
        	circulo[greenCircle[0]].corG = 1.0f;
        	circulo[greenCircle[0]].corB = 1.0f;
        }
    }
    
    calculaCirculoMov(circulo[greenCircle[0]].centroX,circulo[greenCircle[0]].centroY);
    glutPostRedisplay();
    glutTimerFunc(16, verificaTeclas, 0);

}

void KeyUp(unsigned char Key, int X, int Y)
{
    teclas.Keyboard[Key] = false;
}

void Keyboard(unsigned char Key, int X, int Y)
{
    teclas.Keyboard[Key] = true;
}

int main (int argc, char **argv){

	if ((argc > 1))
	{
		char caminho[500] = {};
		strncpy(caminho, argv[1], 500);
	    //strcat(caminho, "config.xml");

		printf("Caminho XML: %s\n", caminho);

		openFileXML (caminho);

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowSize (janela.largura, janela.altura);
		glutCreateWindow ("TC2 - Iagoh Ribeiro Lima");
		glutReshapeFunc(Reshape);
		glutDisplayFunc (display);
	    glutKeyboardUpFunc(KeyUp);
	    glutKeyboardFunc(Keyboard);
	    glutTimerFunc(16, verificaTeclas, 0);

		init();
		
		glutMainLoop();
	}
    else if (argc <= 1)
    {
        fprintf(stderr, "Caminho não encontrado. Tente Novamente.\n");
    }
    else
    {
        fprintf(stderr, "Caminho não encontrado. Tente Novamente.\n");
    }

   	return 0;
}
