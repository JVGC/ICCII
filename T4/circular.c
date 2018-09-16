#include <stdio.h>
#include <stdlib.h>
#include "circular.h"


circular_t* cria_circular(){

	circular_t* l = (circular_t*) malloc(sizeof(circular_t)); // ALOCA A STRUCT LISTA
	// STARTA OS PONTEIROS HEAD E SENTI COMO NULOS
	l->head = NULL;
	l->senti = NULL;
	// RETORNA A LISTA
	return l;
}

void insere_circular(circular_t* l, elem x){
	no_t* p = (no_t*) malloc(sizeof(no_t)); // ALOCA O NOVO NO
	// PONTEIROS AUXILIARES PARA AJUDAR NA INSERCAO
	no_t* atual; 
	no_t* ant;
	
	if(l->head == NULL){ // QUER DIZER QUE NAO HA ELEMENTOS NA LISTA	
		// ENTAO SENTI E HEAD APONTAM PARA O NOVO NO
		l->head = p; 
		l->senti = p;
		// E O PONTEIRO PARA O PROXIMO DESSE NO APONTA PARA ELE MESMO JA QUE EH O UNICO ELEMENTO NA LISTA
		p->prox = p;
	}else{ // JA HA ALGUM ELEMENTO NA LISTA
		// SE OS t0 DOS PROCESSOS FOREM IGUAIS(COMO ELES JA VEM ORDENADOS POR t0->r->code)
		// ENTAO QUER DIZER QUE O PROCESSO ENTRANDO JA ESTA ORDENADO EM RELACAO AO DA CAUDA
		// OU AINDA SE OS t0 FOREM DIFERENTES, SE O PROCESSO ENTRANDO TIVER PRIORIDADE MENOR QUE O ULTIMO DA LISTA
		// ENTAO ELE TEM QUE SER INSERIDO DEPOIS
		if(l->senti->info.t0 == x.t0 || x.r < l->senti->info.r){ // INSERINDO NO 'FINAL' DA LISTA	
			p->prox = l->head; // APONTA O PONTEIRO DO PROX PARA HEAD(QUE EH O PROXIMO DA SENTI)
			l->senti->prox = p; // O PROXIMO DA CAUDA RECEBE O NOVO NO
			l->senti = p; // E CAUDA AGORA EH O NOVO NO
		} // SE NAO FOR PARA SER INSERIDO NA CAUDA, ENTÃO COMPARA AS PRIORIDADES DO NOVO NO E DA CABEÇA
		  // SE O NOVO NO TIVER MAIOR PRIORIDADE QUE O NO CABECA, ENTAO ELE EH A NOVA CABECA DA LISTA
		else if(x.r > l->head->info.r){ // INSERINDO NO 'INICIO'
			p->prox = l->head; //APONTA O PONTEIRO DO PROX PARA HEAD
			l->senti->prox = p; // O PROXIMO DA CAUDA RECEBE O NOVO NO
			l->head = p; // E CABECA AGORA EH O NOVO NO 
		}else{ // INSERINDO NO MEIO DA LISTA
			ant = l->head; // AUXILIAR QUE COMECA NA CABECA DA LISTA
			atual = l->head->prox; // O SEGUINTE DO DA CABECA
			// EXECUTA O LOOP ENQUANTO NAO FOR ENCONTRADO AONDE DEVE SER POSTO O NOVO NO
			// E ENQUANTO NAO TIVER DADO A VOLTA COMPLETA NA LISTA
			do{
				// SE A PRIORIDADE FOR IGUAL
				if(x.r == atual->info.r){	
					// AGORA VERIFICO O CODIGO DOS PROCESSOS
					// SE O CODIGO A SER INSERIDO FOR MENOR QUE O ATUAL
					// E CASO SEJA TEM QUE SER INSERIDO NA FRENTE DO ATUAL
					// (ORDEM ASCENDENTE DE CODIGO)
					if(x.code < atual->info.code){
						ant->prox =  p; // O PROXIMO DO ANTERIOR APONTA PARA P
						p->prox= atual; // E O PROXIMO DE P APONTA PARA O ATUAL
						break; // SAI DO WHILE
					}
				}
				// SE AS PRIORIDADES FOREM DIFERENTES
				// VERIFICO SE O NOVO NO TEM MAIOR PRIORIDADE QUE O ATUAL
				// E CASO TENHA TEM QUE SER INSERIDO NA FRENTE DO ATUAL
				// (ORDEM DECRESCENTE DE PRIORIDADE)
				else if (x.r > atual->info.r){ 
					ant->prox =  p;// O PROXIMO DO ANTERIOR APONTA PARA P
					p->prox= atual;// E O PROXIMO DE P APONTA PARA O ATUAL
					break; // SAI DO WHILE
				}
				// ANDA COM OS PONTEIROS PELA LISTA
				ant = atual; 
				atual= atual->prox;
			}while(atual != l->head);
		}
	}

	p->info = x; // PASSO O PROCESSO PARA P

}

void remover_circular(circular_t* l, int x){	
	// x EH O CODIGO DO PROCESSO A SER REMOVIDO
	no_t *ant, *atual; // PONTEIROS AUXILIARES PARA AJUDAR NA INSERCAO
	
	if(l->senti == NULL) // SE A CAUDA FOR NULA, NAO HA ELEMENTOS NA LISTA
		return ;
	
	ant = l->senti; // ANT RECEBE A SENTINELA



	atual = l->senti->prox; // E ATUAL RECEBE O PROXIMO
	
	// RODA ENQUANTO O ATUAL NAO DER A VOLTA COMPLETA PELA LISTA
	do{
		// SE OS CODIGOS FOREM IGUAIS, ACHEI O NO A SER REMOVIDO
		if(x == atual->info.code){
			// FACO QUE O PROXIMO DO NO ANTERIOR AO SER REMOVIDO, APONTE PARA O PROXIMO DO ATUAL
			ant->prox = atual->prox; 
			l->senti = ant; // E A SENTINELA APONTA PARA O ANTERIOR
			if(atual == l->senti) // SE O ATUAL FOR A SENTINELA
				l->senti = atual->prox; // A SENTINELA APONTA PARA O PROXIMO DO ATUAL
			if(atual == l->head) // SE ATUAL FOR A CABEÇA
				l->head = atual->prox; // A NOVA CABEÇA EH O PROXIMO
			if(atual == ant){ // HA SOMENTE UM ELEMENTO NA LISTA, PORTANTO A LISTA FICARA VAZIA
				l->senti = NULL;
				l->head = NULL;
			}
			free(atual); // LIBERA O NO
			return ;
		}
		// ANDA COM OS PONTEIROS PELA LISTA
		ant = atual; 
		atual = atual->prox;
	} while (atual != l->senti->prox);
	
}


void libera_circular(circular_t* l){
	// REMOVO TODOS OS ELEMENTOS RESTANTES NA LISTA
	while(l->head != NULL){	
		remover_circular(l, l->head->info.code);
	}	
	// E DOU FREE NA STRUCT
	free(l);

}