#include <stdio.h>
#include <stdlib.h>
#include "labirinto.h"
#include "pilha.h"
#include "DLC.h"
#include "Bubble.h"

// EXPLICANDO O ALGORITMO:
// EU IREI PERCORRER AS CAMARAS ATRAVES DE UMA MATRIZ DE ADJACENCIA, MARCANDO OS PONTOS VISITADOS SEMPRE QUE EU PASSAR POR ELES
// IREI ANDAR NO LABIRINTO ATÉ ACHAR UM CAMINHO SEM SAIDA. ASSIM, VOU ABRIR TODOS SEGMENTOS LIGADOS A ELES.
// E DEPOIS IREI FECHAR O CAMINHO ENTRE ESSA POSICAO ATUAL E A ANTERIOR, E DEPOIS DESEMPILHAR A PILHA (VOLTANDO PARA O PONTO ANTERIOR)
// SEMPRE QUE ACHAR CAMINHO FECHADO, ABRE OS CAMINHOS, FECHA O SEGMENTO ENTRE O ATUAL E O ANTERIOR, E DESEMPILHA A PILHA.





int main(int argc, char const *argv[]){
	int i, j;
	int NP; // NUMERO DE PONTOS NO LABIRINTO
	scanf("%d", &NP);
	
	Ponto* points= createPoints(NP); // CRIA UM VETOR DE PONTOS E LE AS COORDENADAS (X,Y) DE CADA PONTO

	//ALOCANDO UM MATRIZ DE ADJACENTES E SETANDO CADA POSICAO EM 0

	int **adj = (int** ) malloc(sizeof(int*) * (NP));
	for(i = 0; i < NP; i++)
		adj[i] = (int* ) malloc(sizeof(int) * (NP));

	for(i = 0; i < NP; i++)
		for(j = 0; j < NP; j++)
			adj[i][j] = 0;

	
	int NC; // NUMERO DE CAMARAS NO LABIRINTO
	scanf("%d", &NC); 
	Camara* camaras = createCamaras(NC); // CRIA UM VETOR DE CAMARAS E LE A POSICÃO E SE HÁ PORTAL DE SAIDA PARA CADA CAMARA

	int NS; // NUMERO DE SEGMENTOS NO LABIRINTO
	scanf("%d", &NS);
	Segmento* segments = createSegments(NS, adj); // CRIA UM VETOR DE SEGMENTOS E LE A POSIÇÃO DAS DUAS EXTREMIDADES DE CADA SEGMENTO
	Tesouro* treasure = createTreasure(); // CRIA UMA CAMARA PARA O TESOURO E LE A POSICÃO DELA E A PONTUACAO DO TESOURO

	int Cs; // POSICAO INICIAL DO LABIRINTO
	int cont = 0; // CONTADOR DE CAMINHOS. ME AJUDARÁ A ALOCAR OS CAMINHOS NO VETOR RESPOSTA		
	scanf("%d", &Cs);
	Caminho* Resposta = createAnswer(); // CRIANDO UM VETOR DE CAMINHOS

	// SE A CAMARA INICIAL FOR IGUAL A DO TESOURO, SE TORNA UM CASO ESPECIAL
	if(Cs == treasure->v){	
		int* aux = (int* ) malloc(sizeof(int) * treasure->NA); // VETOR AUXILIAR QUE IRÁ GUARDAR AS MODIFICAÇÕES NOS SEGMENTOS QUANDO AS AÇÕES SÃO ATIVADAS
		ativa_acoes(adj, NP, treasure, aux); // ATIVO AS ACOES DA CAMARA DO TESOURO
		cont = Search_Exit_Special_Case(adj, NP, camaras, NC, Cs, Resposta, treasure); // PROCURA OS CAMINHOS POSSIVEIS
		
		free(aux); // LIBERA O VETOR AUXILIAR
	}else{
		// SE NÃO FOR, É UM CASO NORMAL

		// PROCURA POR CAMINHOS POSSÍVEIS QUE NÃO PASSEM PELA CAMARA DO TESOURO
		cont = Path_Without_Treasure(adj, NP, camaras, NC, Resposta, Cs, treasure->v); 
		// LOGO APOS, CHAMA OUTRA FUNCAO QUE IRA PROCURAR PELO CAMINHOS POSSIVEI QUE PASSEM PELO TESOURO
		
		cont = Search_Treasure(adj, NP, camaras, NC, Cs, Resposta, cont, treasure);

	}
	// CALCULANDO A SOMA GEODESICA
	for(i = 0; i < cont; i++)
		Geodesic_Sum(points, NP, Resposta, i);
	
	//CALCULANDO A PONTUACAO DO CAMINHO
	for(i = 0; i < cont; i++)
		Resposta[i].score = Resposta[i].score - Resposta[i].geo;
	
	// ORDENANDO OS CAMINHOS
	Bubble_Sort(Resposta, cont);

	// IMPRIMINDO AS RESPOSTAS

	printf("%d %d\n", treasure->v, treasure->T);
	Print_Answer(Resposta, cont);

	//LIBERANDO A MEMORIA ALOCADA
	destroyPoints(points);
	destroyCamaras(camaras);
	destroySegments(segments);
	destroyTreasure(treasure);
	
	for(i = 0; i < cont; i++)
		free(Resposta[i].vet);
	free(Resposta);
	
	for(i = 0; i < NP; i++)
		free(adj[i]);
	free(adj);
	
	return 0;
}

