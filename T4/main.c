#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "Bubble.h"
#include "circular.h"


int main(){
	// ALOCO UM VETOR DE PROCESSOS COM TAMANHO 20
	process_t* processos = (process_t*) malloc(sizeof(process_t) * 20);
	
	char c;
	c= getchar(); // LEIO O PRIMEIRO CHARACTER DO ARQUIVO
	int i = 0;
	int j;

	// SE FOR 'f'
	if(c == 'f'){ // UTILIZAR FIFO	
		// LE OS PROCESSOS ENQUANTO NÃO FOR FIM DE ARQUIVO
		while(!feof(stdin)){
			// COM O FIFO NAO HA NECESSIDADE DE SE LER A PRIORIDADE DE CADA PROCESSO
			scanf("%d", &processos[i].code);
			scanf("%d", &processos[i].t0);
			scanf("%d", &processos[i].tf);
			i++;
		}	
		 int tam = i; // NUMERO DE PROCESSOS LIDOS
		
		// ORDENO O VETOR DE PROCESSOS
		Bubble_Sort(processos, tam, FIFO);
		
		i=0; // RESETO i (i VAI REPRESENTAR A POSICAO DO PRÓXIMO PROCESSO A SER PROCESSADO)
		j=0; // TEMPO PERCORRIDO
		
		// RODANDO OS PROCESSOS
		while(i< tam){
				//RODO CADA PROCESSO ATE O FINAL E PASSO PARA O PROXIMO
				while(processos[i].tf > 0){
				 	processos[i].tf--;
				 	j++; // INCREMENTO O TEMPO
				 }
				 printf("%d %d\n",processos[i].code, j);
			i++;
		}
	}else{ // SE NAO FOR PARA UTILIZAR FIFO, TEREI QUE USAR O ROUND ROBIN
		circular_t* circular = cria_circular(); // CRIA UM LISTA CIRCULAR
		
		// CRIO UMA STRING AUXILIAR PARA LER O CODIGO DO PRIMEIRO PROCESSO
		// JA QUE EU JA LI O PRIMEIRO CHARACTER COM %c, E ESSE CHARACTER PERDIDO FAZ PARTE
		// DO CODIGO DO PROCESSO
		
		char aux[7]; 
		aux[0] = c; // COLOCO O CHARACTER LIDO ANTERIORMENTE NA PRIMEIRA POSICAO DA STRING
		c= getchar(); // LEIO O PROXIMO CHARACTER
		i= 1; // POSICAO A SER INSERIDA NA STRING
		
		while(c != 32){ // ENQUANTO O CHAR LIDO NAO FOR UM SPACE
			aux[i] = c; // COLOCO O CHARACTER NA STRING
			i++; // INCREMENTO A POSICAO NA STRING
			c= getchar(); // LEIO O PROXIMO CHAR
			
		}

		aux[i] = '\0'; // NO FIM, COLOCO UM '\0' PARA INDICAR O FINAL DA STRING
		
		// TRANSFORMO A STRING EM INT, COLOCANDO NO CODIGO DO PRIMEIRO PROCESSO
		processos[0].code = atoi(aux); 
		
		// LEIO O RESTO DA INFORMAÇÕES DO PRIMEIRO PROCESSO
		scanf("%d", &processos[0].t0); 
		scanf("%d", &processos[0].tf);
		scanf("%d", &processos[0].r);
		i =1;
		
		// LEIO OS OUTROS PROCESSOS ENQUANTO NAO HOUVER FIM DE ARQUIVO
		while(!feof(stdin)){
			scanf("%d", &processos[i].code);
			scanf("%d", &processos[i].t0);
			scanf("%d", &processos[i].tf);
			scanf("%d", &processos[i].r);
			i++;
		}	


		int tam = i; // NUMERO DE PROCESSOS LIDOS
		i=0; // posicao no vetor de processos
		j=1; // tempo(COMECA EM 1 PORQUE EH O TEMPO MINIMO PARA UM PROCESSO SER RECONHECIDO)

		// ORDENO O VETOR DE PROCESSOS DE ACORDO COM O t0 DELES
		// PROCESSOS COM PRIORIDADE 0 SAO OS PRIMEIROS A SEREM RODADOS
		// E AS OUTRAS PRIORIDADES NAO PODEM SER RODADAS ENQUANTO
		// OS DE PRIORIDADE 0 INSERIDOS NAO TERMINAREM
		Bubble_Sort(processos, tam, RR);	
		
		// SE HOUVEREM PROCESSOS COM PRIORIDADE 0	
		if(processos[i].r == 0){
			// CRIO UMA LISTA CIRCULAR ESPECIFICA PARA ELES
			circular_t* real_time = cria_circular();
			while(processos[i].r == 0){ // ENQUANTO HOUVEREM PROCESSO COM PRIORIDADE 0
				// INSIRO ELES NA LISTA
				insere_circular(real_time, processos[i]);
				i++;
			}
			
			// E RODO UM ROUND ROBIN COM TODOS ELES
			// PASSANDO COMO ARGUMENTO:
			// A LISTA, O TEMPO PERCORRIDO, O VETOR DE PROCESSOS, E A POSICAO DO VETOR JA PERCORRIDA
			Round_Robin(real_time, &j, processos, &i);
			
			// LIBERO A MEMORIA ALOCADA NA LISTA
			libera_circular(real_time);
		}
		// AGORA INDEPENDENTE DE TER HAVIDO OU NAO PROCESSOS COM PRIORIDADE 0
		// PASSO PARA A LISTA TODOS OS PROCESSOS COM TEMPO MENOR QUE O ATUAL(j)
		while(processos[i].t0 <= j && i < tam){
	 		// INSIRO NA LISTA, E CONTINUO A PERCORRER O VETOR DE PROCESSOS
	 		insere_circular(circular, processos[i]);
		 	i++;
		}
		
		// AGORA RODO UM ROUND ROBIN COM OS ELEMENTOS JA INSERIDOS E COM OS QUE AINDA RESTAM
		// O ROUND ROBIN TERMINA DE INSERIR OS QUE FALTAM, CONFORME PASSA O TEMPO DE EXECUCAO
		// PASSANDO COMO ARGUMENTO:
		// A LISTA, O TEMPO PERCORRIDO, O VETOR DE PROCESSOS, E A POSICAO DO VETOR JA PERCORRIDA
		Round_Robin(circular, &j, processos, &i);		

		// LIBERO A MEMORIA ALOCADA NA LISTA
		libera_circular(circular);
			
		
	}
	// LIBERO O VETOR DE PROCESSOS
	free(processos);


	return 0;
	
}