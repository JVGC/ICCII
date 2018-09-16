#include <stdio.h>
#include <stdlib.h>
#include "labirinto.h"
#include "pilha.h"
#include "DLC.h"
#include <math.h>

// CRIANDO UM VETOR DE PONTOS
Ponto* createPoints(int NP){
	int i;
	Ponto* p = (Ponto*) malloc(sizeof(Ponto)*NP);

	for(i = 0; i < NP; i++){
		scanf("%f", &p[i].x);
		scanf("%f", &p[i].y);
	}	

	return p;

}

// CRIANDO O VETOR DE CAMARAS
Camara* createCamaras(int NC){
	int i;
	Camara* c = (Camara*)malloc(sizeof(Camara)*NC);

	for(i = 0; i < NC; i++){
		scanf("%d", &c[i].v);
		scanf("%d", &c[i].o);
	}	

	return c;
}

// CRIANDO O VETOR DE SEGMENTOS
Segmento* createSegments(int NS, int** adj){
	int i;
	Segmento* s = (Segmento*) malloc(sizeof(Segmento)*100);

	for(i = 0; i < NS; i++){	
		// LE AS EXTREMIDADES DOS SEGMENTOS
		scanf("%d", &s[i].b); 
		scanf("%d", &s[i].e);
		// PREENCHE A MATRIZ DE ADJACENCIA COM AS CAMARAS LIGADAS
		// SE UMA DAS EXTREMIDADES DO SEGMENTO FOR MENOR QUE 0, ENTÃO O SEGMENTO ESTÁ FECHADO DESDE O COMEÇO DO LABIRINTO
		if(s[i].b < 0 ){	
			adj[-s[i].b - 1][s[i].e - 1] = CLOSE_FORBIDDEN;
			adj[s[i].e - 1][-s[i].b - 1] = CLOSE_FORBIDDEN;
		}
		else if(s[i].e < 0){
			adj[-s[i].e - 1][s[i].b - 1] = CLOSE_FORBIDDEN;
			adj[s[i].b-1][-s[i].e-1] = CLOSE_FORBIDDEN;
		}
		else{
			adj[s[i].b - 1][s[i].e - 1] = OPEN;
			adj[s[i].e - 1][s[i].b - 1] = OPEN;
		}
	}

	return s;
}

// CRIA UM VETOR DE CAMINHOS (RESPOSTA)
Caminho* createAnswer(){
	int i;
	Caminho* c = (Caminho* ) malloc(sizeof(Caminho) * 30);
	for(i=0;i < 20; i++){	
		c[i].vet = (elem* ) malloc(sizeof(elem) * 100);
		c[i].score=0;
	}
	return c;
}

void Print_Answer(Caminho* Resposta, int cont){
	int i, j, tam;
	for(i = 0; i < cont; i++){
		 tam = Resposta[i].tam;  
		 printf("%d ", tam);
		for(j = 0; j < tam; j++)
			printf("%d ", Resposta[i].vet[j]);
		printf("%0.f ",  Resposta[i].geo);
		printf("%0.f", Resposta[i].score);
		printf("\n");
	}
}


// LIBERA O VETOR DE PONTOS
void destroyPoints(Ponto* p){
	free(p);
}

// LIBERA O VETOR DE CAMARAS
void destroyCamaras(Camara* c){
	free(c);
}

// LIBERA O VETOR DE SEGMENTOS
void destroySegments(Segmento* s){
	free(s);
}

// CALCULANDO A SOMA GEODESICA DO CAMINHO
void Geodesic_Sum(Ponto* Points, int NP, Caminho* Resposta, int cont){
	int i;
	float aux;
	int tam = Resposta[cont].tam;
	
	for(i = 1; i < tam; i++){
		aux = pow((Points[Resposta[cont].vet[i]-1].x - Points[Resposta[cont].vet[i-1]-1].x),2.0);
		aux += pow((Points[Resposta[cont].vet[i]-1].y - Points[Resposta[cont].vet[i-1]-1].y),2.0);
		aux = (float)sqrt(aux);
		Resposta[cont].geo += aux;
	}
	
}

void abre_segmento(int** adj, int NP, int posicao, int* vis)
{
	int j;
	// VAI ABRIR OS SEGMENTOS SE EU MESMO FECHEI (OU SEJA VALE -1), E SE ESSA POSICAO NÃO FOI VISITADA AINDA

	for(j = 0; j < NP; j++){	
		if(adj[posicao-1][j] == CLOSE && (vis[j+1] == NOT_VISITED)){	
			adj[posicao-1][j] = OPEN;
			adj[j][posicao-1] = OPEN;
		}
	}


}


void push_Caminho(pilha_t* p, Caminho* c, int cont){
	int i;
	// COLOCA O CAMINHO PERCORRIDO NO VETOR RESPOSTA

	//  i COMEÇA EM 1, POIS A PILHA É INICIALIZADA EM 1
	for(i = 1; i <= (p->topo); i++){	
		c[cont].vet[i-1] = p->vet[i];
		
	}
	c[cont].tam = i-1;


}

int Path_Without_Treasure(int** adj, int NP, Camara* camaras, int NC, Caminho* Resposta, int Cs, int Ct){	
	
	pilha_t* caminho_percorrido = cria_pilha(); // CRIANDO A PILHA QUE IRÁ ARMAZENAR O CAMINHO PERCORRIDO
	
	int cont = 0; // NÚMERO DE CAMINHOS ENCONTRADOS
	int i, j;

	// VETOR DE VISITADO. VOU MARCAR SEMPRE QUE O PONTO FOR VISITADO, E SEMPRE QUE EU VOLTAR, A POSICAO DESSE PONTO NO VETOR VOLTA A 0
	int* vis = (int* ) malloc(sizeof(int)*NP+1);
	for(i = 0; i <= NP; i++)
		vis[i] = NOT_VISITED;
	vis[Cs] = VISITED; // MARCO A POSICAO INICIAL COMO VISITADA
	
	int posicao; // DEMARCA A POSICAO QUE O "PERSONAGEM" ESTA NO LABIRINTO
	posicao = Cs; // POSICAO ATUAL EH A CAMARA INICIAL
	

	push(caminho_percorrido, posicao); // COLOCA A CAMARA INICIAL NA PILHA 

	// ESSE DO/WHILE VAI FUNCIONAR ENQUANTO AS POSSIBILIDADES DE CAMINHOS NÃO FOREM SOMENTE A POSICAO INICIAL
	do{
		// PERCORRO MINHA MATRIZ DE ADJACENCIA, NA LINHA = POSICAO -1 (JÁ QUE j COMECA EM 0)
		for(j = 0; j < NP; j++){	

			// SE O SEGMENTO EXISTIR E ESTIVER ABERTO, E A POSICAO DESSE PROXIMO PONTO NAO HOUVER SIDO VISITADA NEM ESSE PONTO FOR O TESOURO

			if((adj[posicao-1][j] == OPEN) && (vis[j+1] == NOT_VISITED) && (j+1 != Ct)) {	
			
				// ENTÃO 
				push(caminho_percorrido, j+1); // EMPILHA A NOVA POSICAO
				vis[j+1] = VISITED; // MARCA A POSICAO NO VETOR DE VISITADO
				posicao = j+1; // ATUALIZA A POSICAO EM QUE ESTOU
				j = -1; // REINICIALIZA O FOR
			}
 		
		}
		// SE EU SAI DO PRIMEIRO FOR, QUER DIZER QUE CHEGUEI EM UMA POSICAO QUE NAO TEM CAMINHO A SE SEGUIR
		// ENTÃO PERCORRO AS CAMARAS PARA VERIFICAR SE EXISTE UMA CAMARA NAQUELA POSICAO, OU SE EH SOMENTE UM PONTO
		for(j = 0; j <NC; j++){
			if(camaras[j].v == posicao){	
				// VERIFICA SE A CAMARA TEM SAIDA
				if(camaras[j].o != 0){
					// COLOCO O CAMINHO PERCORRIDO NA RESPOSTA  E SAIO DESSE FOR
					push_Caminho(caminho_percorrido, Resposta, cont);
					cont++;
					
				}

				break;	
				
			}			
		}

		// AGORA QUE CHEGUEI NUMA POSICAO SEM SAIDA, ABRO OS SEGMENTO RELACIONADOS A ELA 
		abre_segmento(adj,NP, posicao, vis); 

		//FECHA OS SEGMENTO ENTRE A POSICAO ATUAL E A ANTERIOR A NÃO SER QUE ESSE SEGMENTO JÁ ESTIVESSE FECHADO DESDE O COMEÇO
		if((adj[caminho_percorrido->vet[caminho_percorrido->topo-1]-1][posicao-1]) == OPEN){
		 	 adj[caminho_percorrido->vet[caminho_percorrido->topo-1]-1][posicao-1] = CLOSE;
			 adj[posicao-1][caminho_percorrido->vet[caminho_percorrido->topo-1]-1] = CLOSE;
		} 

		 vis[posicao] = NOT_VISITED; // TIRA A POSICAO DO VETOR DE VISITADOS
		
		pop(caminho_percorrido); // DESEMPILHA A PILHA
		posicao = caminho_percorrido->vet[caminho_percorrido->topo]; // E ATUALIZA A POSICAO ATUAL COM O VALOR DO TOPO

		

	}while((posicao != Cs));
	
	push_Caminho(caminho_percorrido, Resposta, cont); // COLOCA A POSICAO INICIAL TAMBÉM COMO UMA SAIDA POSSIVEL
	cont++;
	abre_segmento(adj,NP, posicao, vis); // ABRE OS SEGMENTOS RELACIONADOS A POSICAO INICIAL

	free(vis); // LIBERA O VETOR DE VISITADOS

	libera_pilha(caminho_percorrido); // E LIBERA A MEMORIA DA PILHA

	return cont; // RETORNA O NUMERO DE CAMINHOS ENCONTRADOS
}





