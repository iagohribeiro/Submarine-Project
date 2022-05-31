/*	Iagoh Ribeiro Lima - 2014100678
		Computacao Grafica 2018/2
*/

#ifndef _TC2_
#define _TC2_

int quantCirc = 0;

typedef struct Janela
{
	int altura, largura;
	float corR, corG, corB;
	char* titulo;

}janelaProg;

typedef struct Circulo
{
	int id;
	int raio;
	bool desenhado;
	bool move;
	bool submerso;
	float corR, corG, corB;
	char fill[50];
	float centroX;
	float centroY;
	float coordenadaX[360];
	float coordenadaY[360];
}circuloProg;

typedef struct keys
{
    bool Keyboard[256];
} Keys;

janelaProg janela= {};
Keys teclas = {};
circuloProg circulo[400] = {};

#endif
