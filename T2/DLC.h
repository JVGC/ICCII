#ifndef _DLC_H_
#define _DLC_H_


							

typedef struct _acao
{
	int b, e; // EXTREMIDADES DA ACAO

}Acao;

typedef struct _tesouro
{
	int v; // INDICE QUE INDICA A POSICAO DA CAMARA 
	Acao* Actions; // AS RESPECTIVAS ACOES QUE O TESOURO REALIZA QUANDO EH ENCONTRADO
	int NA; // NUMERO DE ACOES DO TESOURO
	int T;  // PONTUACAO DO TESOURO

}Tesouro;

 
Tesouro* createTreasure();

void destroyTreasure(Tesouro* t);
int verifica_Portais(int** adj, int NP, int* vis2, int posicao);
void ativa_acoes(int** adj, int NP, Tesouro* treasure, int* mat);
int Search_Treasure(int** adj, int NP, Camara* camaras, int NC, int Cs, Caminho* Resposta, int cont, Tesouro* treasure);
int Search_Exit(int** adj, int NP, Camara* camaras, int NC, Caminho* Resposta, int cont, pilha_t* caminho_percorrido, Tesouro* treasure);
int Search_Exit_Special_Case(int** adj, int NP, Camara* camaras, int NC, int Cs, Caminho* Resposta, Tesouro* treasure);


#endif