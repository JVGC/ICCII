#include <stdio.h>
#include <stdlib.h>
#include "labirinto.h"
#include "pilha.h"
#include "DLC.h"

// ALOCA A MEMÓRIA PARA O TESOURO E LE A SUA POSICAO E A PONTUACAO
Tesouro* createTreasure(){
	int i;

	Tesouro* t = (Tesouro*) malloc(sizeof(Tesouro));

	scanf("%d", &t->v);
	scanf("%d", &t->T);

	scanf("%d", &t->NA); // LE O NUMERO DE ACOES DO TESOURO

	t->Actions = (Acao* ) malloc(sizeof(Acao) * t->NA); // CRIA UM VETOR DE ACOES

	// LE AS EXTREMIDADES DOS SEGMENTOS QUE SERÃO LIBERADOS OU FECHADOS DEVIDO AS ACOES
	for(i = 0; i < t->NA; i++){
		scanf("%d", &t->Actions[i].b);
		scanf("%d", &t->Actions[i].e);
	}
	
	return t;

}

// LIBERA O VETOR DE ACOES E O TESOURO
void destroyTreasure(Tesouro* t){
	free(t->Actions);
	free(t);
}

// ATIVA AS ACOES GUARDANDO OS VALORES ANTERIORES NUM VETOR AUXILIAR
void ativa_acoes(int** adj, int NP, Tesouro* treasure, int* aux){	
	int i;

	// PARA CADA ACAO, VERIFICA QUANDO UMA DAS DUAS EXTREMIDADES É < 0, E FECHA O SEGMENTO NA MATRIZ DE ADJACENCIA
	// SE AS DUAS EXTREMIDADES FOREM > 0, ENTAO O SEGMENTO É ABERTO
	for(i = 0; i < treasure->NA; i++){	
		if(treasure->Actions[i].b < 0 ){	
			aux[i] = adj[-treasure->Actions[i].b - 1][treasure->Actions[i].e - 1];
						
			adj[-treasure->Actions[i].b - 1][treasure->Actions[i].e - 1] = CLOSE_FORBIDDEN;
			adj[treasure->Actions[i].e - 1][-treasure->Actions[i].b - 1] = CLOSE_FORBIDDEN;
		}
		else if(treasure->Actions[i].e < 0){
			aux[i] = adj[treasure->Actions[i].b - 1][-treasure->Actions[i].e - 1];
			
			
			adj[-treasure->Actions[i].e - 1][treasure->Actions[i].b - 1] = CLOSE_FORBIDDEN;
			adj[treasure->Actions[i].b - 1][-treasure->Actions[i].e-1] = CLOSE_FORBIDDEN;
		}
		else{
			aux[i] = adj[treasure->Actions[i].b - 1][treasure->Actions[i].e - 1];
			
			adj[treasure->Actions[i].b - 1][treasure->Actions[i].e - 1] = OPEN;
			adj[treasure->Actions[i].e - 1][treasure->Actions[i].b - 1] = OPEN;
		}	
	}		
}

// FUNCAO USADA PARA DESATIVAR AS ACOES, COM O VALOR GUARDADO NAQUELE VETOR AUXILIAR
void desativa_acoes(int** adj, int NP, Tesouro* treasure, int* aux){
	int i;

	for(i = 0; i < treasure->NA; i++){
		if(treasure->Actions[i].b < 0){	
			adj[-treasure->Actions[i].b - 1][treasure->Actions[i].e - 1] = aux[i];
			adj[treasure->Actions[i].e - 1][-treasure->Actions[i].b - 1] = aux[i];
		}
		else if(treasure->Actions[i].e < 0){
			adj[-treasure->Actions[i].e - 1][treasure->Actions[i].b - 1] = aux[i];
			adj[treasure->Actions[i].b - 1][-treasure->Actions[i].e-1] = aux[i];
		}
		else{
			adj[treasure->Actions[i].b - 1][treasure->Actions[i].e - 1] = aux[i];
			adj[treasure->Actions[i].e - 1][treasure->Actions[i].b - 1] = aux[i];
		}	
	}

}

// ASSIM QUE EU VOU VOLTAR PARA O INICIO DO MEU LABIRINTO, VERIFICO SE HÁ OUTRO SEGMENTO QUE EU POSSA SEGUIR
// SERVE PARA O DO/WHILE USADO NO ALGORITMO NÃO FINALIZAR SOMENTE QUANDO A POSICAO FOR IGUAL A INICIAL
int verifica_Portais(int** adj, int NP, int* vis2, int posicao)
{
	int j;

	for(j = 0; j < NP; j++){	
		if((adj[posicao-1][j] == OPEN) &&  (vis2[j+1] == NOT_VISITED)){	
			return FOUND;
		}

	}
	
	return NOT_FOUND;
}



int Search_Treasure(int** adj, int NP, Camara* camaras, int NC, int Cs, Caminho* Resposta, int cont, Tesouro* treasure){
	int i, j;

	pilha_t* caminho_percorrido = cria_pilha(); // CRIANDO A PILHA QUE IRÁ ARMAZENAR O CAMINHO PERCORRIDO
	
	int posicao; // DEMARCA A POSICAO QUE O "PERSONAGEM" ESTA NO LABIRINTO
	posicao = Cs; // POSICAO INICIAL EH A CAMARA INICIAL
	push(caminho_percorrido, posicao); // COLOCA A CAMARA INICIAL NA PILHA 	
		


	// VETOR DE VISITADO. VOU MARCAR SEMPRE QUE O PONTO FOR VISITADO, E SEMPRE QUE EU VOLTAR, A POSICAO DESSE PONTO NO VETOR VOLTA A 0
	int* vis = (int* ) malloc(sizeof(int)*NP+1);
	for(i = 0; i <= NP; i++)
		vis[i] = NOT_VISITED;

	vis[Cs] = VISITED; // MARCO A POSICAO INICIAL COMO VISITADA

	int* aux = (int* ) malloc(sizeof(int) * treasure->NA); // VETOR AUXILIAR QUE IRÁ GUARDAR AS MODIFICAÇÕES NOS SEGMENTOS QUANDO AS AÇÕES SÃO ATIVADAS

	// ESSE DO/WHILE VAI FUNCIONAR ENQUANTO AS POSSIBILIDADES DE CAMINHOS NÃO FOREM SOMENTE A POSICAO INICIAL
	do{
		// PERCORRO MINHA MATRIZ DE ADJACENCIA, NA LINHA = POSICAO -1 (JÁ QUE j COMECA EM 0)
		for(j = 0; j < NP; j++){	
			// SE O SEGMENTO EXISTIR E ESTIVER ABERTO, E A POSICAO DESSE PROXIMO PONTO HOUVER SIDO VISITADA
			if((adj[posicao-1][j] == OPEN) && (vis[j+1] == NOT_VISITED)) {	

				push(caminho_percorrido, j+1); // EMPILHA A NOVA POSICAO
				vis[j+1] = VISITED; // MARCA A POSICAO NO VETOR DE VISITADO
				posicao = j+1; // ATUALIZA A POSICAO EM QUE ESTOU
				j = -1; // REINICIALIZA O VETOR
					
			
				// A CADA VEZ QUE ANDAR NO LABIRINTO, VERIFICA SE A NOVA CAMARA EH A CAMARA DO TESOURO
				if(posicao == treasure->v){
					// SE FOR, VERIFICA SE EH SAIDA
					if(camaras[treasure->v-1].o != 0){	
							// COLOCA ESSE CAMINHO NA RESPOSTA
							Resposta[cont].score = treasure->T;
							push_Caminho(caminho_percorrido,Resposta, cont);
							cont++;
						}
						// SE HOUVER ACOES A SEREM FEITAS
						if(treasure->NA > 0)
							ativa_acoes(adj, NP, treasure, aux); // UTILIZA AS ACOES
			 			
			 			// CHAMA UMA NOVA FUNCAO PARA A ACHAR AS POSSIVEIS SAIDAS A PARTIR DA CAMARA DO TESOURO
			 			cont = Search_Exit(adj, NP, camaras, NC, Resposta, cont, caminho_percorrido, treasure);
			 		
			 			// QUANDO VOLTAR DA FUNCAO, DESATIVA AS ACOES FEITAS ANTERIORMENTE CASO HOUVER
			 			// E VAI PROCURAR OUTROS CAMINHOS POSSIVEIS QUE LEVEM AO TESOURO

			 			if(treasure->NA > 0)
			 				desativa_acoes(adj, NP, treasure, aux); // DESATIVA AS ACOES
			 			break;
				}

			}
			

		}

		// AGORA QUE CHEGUEI NUMA POSICAO SEM SAIDA, ABRO OS SEGMENTO RELACIONADOS A ELA
		abre_segmento(adj,NP, posicao, vis); 

		//FECHA OS SEGMENTO ENTRE A POSICAO ATUAL E A ANTERIOR A NÃO SER QUE ESSE SEGMENTO JÁ ESTIVESSE FECHADO DESDE O COMEÇO
		if((adj[caminho_percorrido->vet[caminho_percorrido->topo-1]-1][posicao-1]) == OPEN){
		 	 adj[caminho_percorrido->vet[caminho_percorrido->topo-1]-1][posicao-1] = CLOSE;
			 adj[posicao-1][caminho_percorrido->vet[caminho_percorrido->topo-1]-1] = CLOSE;
		} 

		 vis[posicao] = NOT_VISITED; 
		 // TIRA A POSICAO DO VETOR DE VISITADOS
		
		pop(caminho_percorrido); // DESEMPILHA A PILHA
		posicao = caminho_percorrido->vet[caminho_percorrido->topo]; // E ATUALIZA A POSICAO ATUAL COM O VALOR DO TOPO

		

	}while((posicao != Cs));
	
	
	libera_pilha(caminho_percorrido); // LIBERA A PILHA
	
	free(aux); // LIBERA O VETOR AUXILIAR
	free(vis); // LIBERA O VETOR DE VISITADOS

	return cont; // RETORNO O NUMERO DE CAMINHOS ENCONTRADOS
}









int Search_Exit(int** adj, int NP, Camara* camaras, int NC, Caminho* Resposta, int cont, pilha_t* caminho_percorrido, Tesouro* treasure)
{	
	int i, j;

	int posicao; // DEMARCA A POSICAO QUE O "PERSONAGEM" ESTA NO LABIRINTO
	
	// CRIO UMA NOVAS MATRIZ DE ADJACENCIA IDÊNTICA A PRIMEIRA(JÁ COM AS ACOES ATIVADAS), PARA QUE AS MUDANÇAS FEITAS NESSA FUNÇÃO NÃO AFETEM 
	// A MATRIZ INICIAL

	int** adj2 = (int**) malloc(sizeof(int*)*NP);
	for(i = 0; i < NP; i++)
		adj2[i] = (int*) malloc(sizeof(int)*NP);

	for(i=0;i<NP; i++)
		for (j = 0; j < NP; j++)
		{
		 	adj2[i][j] = adj[i][j]; 
		}	 

	// CRIA UM NOVO VETOR DE VISITADO, JÁ QUE ASSIM QUE SE CHEGA NO TESOURO, PODE-SE PASSAR MAIS UMA VEZ POR UMA CAMARA JÁ VISITADA
	int* vis2 = (int* ) malloc(sizeof(int)*NP+1);
	for(i = 0; i <= NP; i++)
		vis2[i] = NOT_VISITED;

	int result = FOUND; // VARIAVEL AUXILIAR
	int Ct = treasure->v;
	posicao = Ct; // POSICAO INICIAL EH A CAMARA INICIAL

	vis2[Ct] = VISITED; // MARCO A POSICAO INICIAL COMO VISITADA
 
	

	// ESSE DO/WHILE VAI FUNCIONAR ENQUANTO AS POSSIBILIDADES DE CAMINHOS NÃO FOREM SOMENTE A POSICAO INICIAL
	do 
	{
		for(j = 0; j < NP; j++)
		{	
			// PERCORRO MINHA MATRIZ DE ADJACENCIA, NA LINHA = POSICAO -1 (JÁ QUE j COMECA EM 0)
			if((adj2[posicao-1][j] == OPEN) &&  (vis2[j+1] == NOT_VISITED)) 
			{	
					
				push(caminho_percorrido, j+1); // EMPILHA A NOVA POSICAO
				vis2[j+1] = VISITED; // MARCA A POSICAO NO VETOR DE VISITADOS
				
				posicao = j+1; // ATUALIZA A POSICAO EM QUE ESTOU
				j = -1; // REINICIALIZA O FOR
				
				// ESSA ABERTURA SERVE PARA ABRIR OS SEGMENTOS QUE AINDA NÃO FORAM VISITADOS EM VOLTA DA POSICAO QUE ESTOU
				// PARA EVITAR O CASO DE EU TER FECHADO ALGUM CAMINHO POSSIVEL ENQUANTO PROCURAVA O TESOURO, E AGORA ESSE CAMINHO PODE
				// SER POSSIVEL
				abre_segmento(adj2,NP, posicao, vis2);
					
	
			}

		}
		// SE EU SAI DO PRIMEIRO FOR, QUER DIZER QUE CHEGUEI EM UMA POSICAO QUE NAO TEM CAMINHO A SE SEGUIR
		// ENTÃO PERCORRO AS CAMARAS PARA VERIFICAR SE EXISTE UMA CAMARA NAQUELA POSICAO, OU SE EH SOMENTE UM PONTO
		for(j = 0; j < NC; j++)
		{
			if(camaras[j].v == posicao)
			{
				// VERIFICA SE A CAMARA TEM SAIDA
				if(camaras[j].o != 0)
				{
					// COLOCO O CAMINHO PERCORRIDO NA RESPOSTA  E SAIO DESSE FOR
					Resposta[cont].score = treasure->T;
					push_Caminho(caminho_percorrido, Resposta, cont);
					cont++;
					
				}
				
				break;
				
			}			
		}

		// AGORA QUE CHEGUEI NUMA POSICAO SEM SAIDA, ABRO OS SEGMENTO RELACIONADOS A ELA
		abre_segmento(adj2,NP, posicao, vis2); 

		//FECHA OS SEGMENTO ENTRE A POSICAO ATUAL E A ANTERIOR A NÃO SER QUE ESSE SEGMENTO JÁ ESTIVESSE FECHADO DESDE O COMEÇO
		if((adj2[caminho_percorrido->vet[caminho_percorrido->topo-1]-1][posicao-1]) == OPEN) 
		 {
		 	//printf("OLA\n");	
			adj2[caminho_percorrido->vet[caminho_percorrido->topo-1]-1][posicao-1] = CLOSE;
			adj2[posicao-1][caminho_percorrido->vet[caminho_percorrido->topo-1]-1] = CLOSE;
		} 
		
		 	vis2[posicao] = NOT_VISITED; // TIRA A POSICAO DO VETOR DE VISITADOS
		
		pop(caminho_percorrido); // DESEMPILHA A PILHA
		posicao = caminho_percorrido->vet[caminho_percorrido->topo]; // E ATUALIZA A POSICAO ATUAL COM O VALOR DO TOPO
		
		// VAI VERIFICAR SE VOLTOU PARA A CAMARA DO TESOURO, E SE AINDA HÁ OUTROS CAMINHOS POR ALI QUE AINDA NÃO FORAM PERCORRIDOS 
		if(posicao == Ct)
			result = verifica_Portais(adj2, NP, vis2, posicao);


		
		

	}while(result != NOT_FOUND); // SAI DO FOR QUANDO NÃO HOUVER MAIS OPCOES DE SAIDA

	// LIBERANDO VETOR DE VISITADOS
	free(vis2);

	// LIBERANDO A MATRIZ DE ADJACENCIA
	for(i = 0; i < NP; i++)
		free(adj2[i]);
	free(adj2);

	
	return cont; // RETORNA O NUMERO DE CAMINHOS ENCONTRADOS

}	


int Search_Exit_Special_Case(int** adj, int NP, Camara* camaras, int NC, int Cs, Caminho* Resposta, Tesouro* treasure)
{
	pilha_t* caminho_percorrido = cria_pilha(); // CRIANDO A PILHA QUE IRÁ ARMAZENAR O CAMINHO PERCORRIDO

	int i, j;

	int cont =0; // NUMERO DE CAMINHOS ENCONTRADOS

	int posicao; // DEMARCA A POSICAO QUE O "PERSONAGEM" ESTA NO LABIRINTO
	

	// VETOR DE VISITADO. VOU MARCAR SEMPRE QUE O PONTO FOR VISITADO, E SEMPRE QUE EU VOLTAR, A POSICAO DESSE PONTO NO VETOR VOLTA A 0
	int* vis = (int* ) malloc(sizeof(int)*NP+1);
	for(i = 0; i <= NP; i++)
		vis[i] = NOT_VISITED;

	posicao = Cs; // POSICAO INICIAL EH A CAMARA INICIAL
		// MARCO A POSICAO INICIAL COMO VISITADA
	vis[Cs] = VISITED;

	push(caminho_percorrido, posicao); // COLOCA A CAMARA INICIAL NA PILHA 

	// ESSE DO/WHILE VAI FUNCIONAR ENQUANTO AS POSSIBILIDADES DE CAMINHOS NÃO FOREM SOMENTE A POSICAO INICIAL
	do 
	{
		// PERCORRO MINHA MATRIZ DE ADJACENCIA, NA LINHDA = POSICAO -1 (JÁ QUE j COMECA EM 0)
		for(j = 0; j < NP; j++)
		{	
			// SE O SEGMENTO EXISTIR E ESTIVER ABERTO, E A POSICAO DESSE PROXIMO PONTO HOUVER SIDO VISITADA NEM ESSE PONTO FOR O TESOURO
			if((adj[posicao-1][j] == OPEN) && (vis[j+1] == NOT_VISITED)) 
			{	
					
					push(caminho_percorrido, j+1); // EMPILHA A NOVA POSICAO
					vis[j+1] = VISITED; // MARCA A POSICAO NO VETOR DE VISITADOS
					posicao = j+1; // ATUALIZA A POSICAO EM QUE ESTOU
					j = -1; // REINICIALIZA O FOR
					
	
			}

		}
		// SE EU SAI DO PRIMEIRO FOR, QUER DIZER QUE CHEGUEI EM UMA POSICAO QUE NAO TEM CAMINHO A SE SEGUIR
		// ENTÃO PERCORRO AS CAMARAS PARA VERIFICAR SE EXISTE UMA CAMARA NAQUELA POSICAO, OU SE EH SOMENTE UM PONTO
		for(j = 0; j <NC; j++)
		{
			if(camaras[j].v == posicao)
			{
				// VERIFICA SE A CAMARA TEM SAIDA
				if(camaras[j].o != 0)
				{
					// COLOCO O CAMINHO PERCORRIDO NA RESPOSTA E SAIO DESSE FOR
					Resposta[cont].score = treasure->T;
					push_Caminho(caminho_percorrido, Resposta, cont);
					cont++;
					
				}
				break;
				
			}			
		}

		// AGORA QUE CHEGUEI NUMA POSICAO SEM SAIDA, ABRO OS SEGMENTO RELACIONADOS A ELA 
		abre_segmento(adj,NP, posicao, vis); 

		//FECHA OS SEGMENTO ENTRE A POSICAO ATUAL E A ANTERIOR A NÃO SER QUE ESSE SEGMENTO JÁ ESTIVESSE FECHADO DESDE O COMEÇO
		if((adj[caminho_percorrido->vet[caminho_percorrido->topo-1]-1][posicao-1]) == OPEN) 
		{
		 	 adj[caminho_percorrido->vet[caminho_percorrido->topo-1]-1][posicao-1] = CLOSE;
			 adj[posicao-1][caminho_percorrido->vet[caminho_percorrido->topo-1]-1] = CLOSE;
		} 
		 vis[posicao] = NOT_VISITED; 
		 // TIRA A POSICAO DO VETOR DE VISITADOS
		
		pop(caminho_percorrido); // DESEMPILHA A PILHA
		posicao = caminho_percorrido->vet[caminho_percorrido->topo]; // E ATUALIZA A POSICAO ATUAL COM O VALOR DO TOPO


	}while((posicao != Cs));

	


	Resposta[cont].score = treasure->T;
	push_Caminho(caminho_percorrido, Resposta, cont);  // COLOCA A POSICAO INICIAL TAMBÉM COMO UMA SAIDA POSSIVEL
	cont++;

	libera_pilha(caminho_percorrido);
	free(vis);

	return cont; // RETORNA O NUMERO DE CAMINHOS ENCONTRADOS
}