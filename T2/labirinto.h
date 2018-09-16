#ifndef _LABIRINTO_H_
#define _LABIRINTO_H_

#include "pilha.h"

#define CLOSE -1 // INDICA QUE O SEGMENTO ESTA FECHADO, MAS QUE FOI O ALGORITMO QUE FECHOU
#define OPEN 1 // INDO QUE O SEGMENTO ESTA ABERTO
#define CLOSE_FORBIDDEN -2 // INDICA QUE O SEGMENTO ESTA FECHADO, MAS QUE NÃO PODE SER ABERTO, OU SEJA, FOI FECHADO PELAS ACOES OU DESDE O COMEÇO

#define NOT_VISITED 0 // INDICA QUE O PONTO AINDA NAO FOI VISITADO
#define VISITED 1 // INDICA QUE O PONTO JA FOI VISITADO

#define NOT_FOUND -1
#define FOUND 1

typedef struct _caminho
{
	elem* vet; // VETOR DOS PONTOS DO CAMINHO
	float score; // PONTUACAO FINAL DO CAMINHO
	int tam;
	float geo;
}Caminho;

typedef struct _camara
{
	int v; //INDICE QUE INDICA A POSICAO DA CAMARA
	int o; // SE 0 NÃO HA SAIDA DO LABIRINTO. SE DIFERENTE DE 0, HA SAIDA

}Camara;

typedef struct _segmento
{
	int b, e; // EXTREMIDADES DO SEGMENTO
}Segmento;



typedef struct _ponto
{
	float x, y; // coordenadas do ponto

}Ponto;



Ponto* createPoints(int NP);
Camara* createCamaras(int NC);
Segmento* createSegments(int NS, int** adj);
Caminho* createAnswer();

void Geodesic_Sum(Ponto* Points, int NP, Caminho* Resposta, int cont);
void Print_Answer(Caminho* Resposta, int cont);


void destroyPoints(Ponto* p);
void destroyCamaras(Camara* c);
void destroySegments(Segmento* s);



int verifica_pilha(pilha_t* p, elem x, int value);
void abre_segmento(int** adj, int NP, int posicao, int* vis);
void fecha_segmento(int** adj, int posicao, int posicaoANT, int NS);
void push_Caminho(pilha_t* p, Caminho* c, int cont);
int Path_Without_Treasure(int** adj, int NP, Camara* camaras, int NC, Caminho* Resposta, int Cs, int Ct);




#endif