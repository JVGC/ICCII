#ifndef _PILHA_H_
#define _PILHA_H_ 

typedef int elem;	

typedef struct pilha {
	elem* vet;
	int topo;

}pilha_t;

pilha_t* cria_pilha();
void libera_pilha(pilha_t* p);
void push(pilha_t* p, elem x);
void pop(pilha_t* p);



#endif