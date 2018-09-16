#ifndef _CIRCULAR_H_
#define _CIRCULAR_H_

#include "process.h"

typedef process_t elem; // DEFINE A STRUCT PROCESS COMO O ELEMENTO A SER INSERIDO NO NÓ
typedef struct no no_t;

struct no{

	struct no* prox; // ponteiro para o próximo nó
	elem info; // contem um struct process
};


struct circular{

	no_t* head; // PONTEIRO PARA O NO INICIAL(CABEÇA DA LISTA CIRCULAR)
	no_t* senti; // PONTEIRO SENTINELA QUE GERALMENTE APONTA PARA O ULTIMO MEMBRO DA LISTA
	//  FUNCIONA COMO UMA SENTINELA(ELA APONTA PARA O ULTIMO MEMBRO DA LISTA)
	// MAS CONFORME VAO SE RETIRANDO OS MEMBROS DA LISTA
	// ELA PASSA A APONTAR PARA O PROXIMO DO NO REMOVIDO
	// PORTANTO ELA NAO NECESSARIAMENTE APONTA PARA O ULTIMO MEMBRO
};


typedef struct circular circular_t;

circular_t* cria_circular(); // FUNCAO QUE ALOCA A LISTA E INICIA OS PONTEIROS COMO NULOS
void insere_circular(circular_t* l, elem x); // INSERE UM NOVO ELEMENTO NA LISTA
void remover_circular(circular_t* l, int x); // REMOVE UM ELEMENTO DA LISTA
void libera_circular(circular_t* l); // LIBERA A LISTA E OS ELEMENTOS RESTANTES NELA DA MEMORIA







#endif