
#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "circular.h"

void Round_Robin(circular_t* l, int* i, process_t* p, int* pos){
	no_t* aux; // NO AUXILIAR
	int prior; // VARIAVEL QUE GUARDA A PRIORIDADE DO ULTIMO PROCESSO RODADO
	aux = l->senti; // AUX COMECA NO ULTIMO ELEMENTO DA LISTA
	
	// ENTRO NUM LOOP QUE RODA ENQUANTO A LISTA NAO ESTIVER VAZIA
	do{
		// AUX RECEBE O SEU PROXIMO (NA PRIMEIRA VEZ AUX APONTA PARA HEAD)
		aux = aux->prox;
		prior = aux->info.r; // PRIOR RECEBE A PRIORIDADE DO PROCESSO A SER RODADO
		aux->info.tf--; // DECREMENTO 1 NO tf(TEMPO NECESSARIO PARA EXECUCAO) DO PROCESSO ATUAL
		*i = *i +1; // AUMENTO O TEMPO
		
		// SE O PROCESSO JA FOI FINALIZADO
		if(aux->info.tf == 0){
			// IMPRIMO AS INFORMACOES E DEPOIS REMOVO O PROCESSO DA LISTA
			printf("%d %d\n",aux->info.code, *i-1);
			remover_circular(l, aux->info.code);
			
			// AUX PASSA A APONTA PARA O NO SENTINELA DA LISTA
			// AUX RECEBE A NOVA SENTINELA(QUE NO CASO EH O PROXIMO PROCESSO A SER EXECUTADO)
			aux = l->senti; 
		}
		// SE HOUVER ALGUM PROCESSO A SER INSERIDO
		if(p[*pos].t0 == *i){
			// SE ESSE PROCESSO TIVER PRIORIDADE 0
			if(p[*pos].r == 0){
				// CRIO UMA LISTA CIRCULAR ESPECIFICA PARA ELES
				circular_t* real_time = cria_circular();
				// PASSO TODOS OS PROCESSOS DE PRIORIDADE 0 E QUE TENHAM QUE ENTRAR NESSE TEMPO
				while(p[*pos].t0 == *i && p[*pos].r == 0){
					insere_circular(real_time, p[*pos]);
					*pos = *pos +1;
				}	
				
				// E RODO UM ROUND ROBIN COM TODOS ELES(CHAMADA RECURSIVA NESSE CASO)
				// PASSANDO COMO ARGUMENTO:
				// A LISTA, O TEMPO PERCORRIDO, O VETOR DE PROCESSOS, E A POSICAO DO VETOR JA PERCORRIDA
				Round_Robin(real_time, i, p, pos);
			} 
			
			// SE A PRIORIDADE NAO FOR 0, TENHO QUE VERIFICAR SE O NOVO PROCESSO
			// TEM MAIOR PRIORIDADE QUE O NO CABECA
			else if(l->head->info.r <= p[*pos].r){
				// SE ELE TIVER, ENTAO EU TENHO QUE RODÁ-LO ASSIM QUE ELE FOR INSERIDO NA LISTA
				aux = aux->prox; // O AUX RECEBE O PROXIMO
				
				// ANTES DE INSERIR O NOVO PROCESSO, TENHO QUE TERMINAR DE RODAR
				// TODOS OS PROCESSOS QUE TEM A MESMA PRIORIDADE QUE O RODADO ANTERIORMENTE
				while(prior == aux->info.r){
					aux->info.tf--; // DECREMENTO 1 NO tf(TEMPO NECESSARIO PARA EXECUCAO) DO PROCESSO ATUAL
					*i = *i +1; // AUMENTO O TEMPO
					
					// SE O PROCESSO JA FOI FINALIZADO
					if(aux->info.tf == 0){
						// IMPRIMO AS INFORMACOES E DEPOIS REMOVO O PROCESSO DA LISTA
						printf("%d %d\n",aux->info.code, *i-1);
						remover_circular(l, aux->info.code);
						// AUX PASSA A APONTA PARA O NO SENTINELA DA LISTA
						// AUX RECEBE A NOVA SENTINELA(QUE NO CASO EH O PROXIMO PROCESSO A SER EXECUTADO)
						aux = l->senti; 
					}

					aux = aux->prox; // O AUX RECEBE O PROXIMO
				}
				
				// INSIRO O NOVO ELEMENTO NA LISTA
				insere_circular(l, p[*pos]);
				
				// AGORA RODO O NOVO NO CABECA
				aux = l->head;
				aux->info.tf--; // DECREMENTO 1 NO tf(TEMPO NECESSARIO PARA EXECUCAO) DO PROCESSO ATUAL
				*i = *i +1; // AUMENTO O TEMPO
				
				// SE O PROCESSO JA FOI FINALIZADO
				if(aux->info.tf == 0){
					// IMPRIMO AS INFORMACOES E DEPOIS REMOVO O PROCESSO DA LISTA
					printf("%d %d\n",aux->info.code, *i-1);
					remover_circular(l, aux->info.code);
					
					// AUX PASSA A APONTA PARA O NO SENTINELA DA LISTA
					// AUX RECEBE A NOVA SENTINELA(QUE NO CASO EH O PROXIMO PROCESSO A SER EXECUTADO)
					aux = l->senti; 
				}
			}else{ 
				// SE A PRIORIDADE DO NOVO ELEMENTO NAO FOR MAIOR QUE DO CABECA,
				// ELE EH SIMPLESMENTE INSERIDO NA NOVA LISTA, E CONTINUA RODANDO NORMALMENTE
				insere_circular(l, p[*pos]);
			}
			*pos = *pos +1; // PASSA PARA O NOVO PROCESSO A SER INSERIDO
		}
			
	}while(l->head != NULL);

}