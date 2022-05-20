#ifndef _TC1_
#define _TC1_

#define GL_SILENCE_DEPRECATION

int quantCirc = 0;
typedef struct Janela
{
	int altura, largura;
	float corR, corG, corB;
	char* titulo;

}janelaProg;

typedef struct Circulo
{
	int raio;
	bool desenhado;
	bool move;
	float corR, corG, corB;
	float centroX;
	float centroY;
	float coordenadaX[360];
	float coordenadaY[360];
}circuloProg;

janelaProg janela= {};
circuloProg circulo[400] = {};

#endif
