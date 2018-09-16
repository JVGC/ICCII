#include <stdio.h>
#include <stdlib.h>
#include "Bubble.h"
#include "labirinto.h"

void Bubble_Sort(Caminho* Resposta, int n){	
	int i, j;

	// ORDENANDO OS CAMINHOS DE ACORDO COM AS REGRAS DE ORDENAÇÃO DO TRABALHO
	//1- MAIOR PONTUAÇÃO
	//2 - MENOR DISTANCIA GEODESICA
	//3 - MENOR NUMERO DE PONTOS PERCORRIDOS
	//4 - MENOR ORDEM DE INDICES. EXEMPLO: (5 3 2) ENTRA ANTES DE (5 4 1), POIS 5=5, E 3 É MENOR QUE 4
	
	for(i = 1; i < n; i++)
		for(j = 0; j < n-i; j++){	
			if(Resposta[j].score < Resposta[j+1].score)
				swap(Resposta, j, j+1);
			else if(Resposta[j].score == Resposta[j+1].score){
				if(Resposta[j].geo > Resposta[j+1].geo)
					swap(Resposta, j, j+1);
				else if(Resposta[j].geo == Resposta[j+1].geo){
					if(Resposta[j].tam > Resposta[j+1].tam)
						swap(Resposta, j, j+1);
					else if(Resposta[j].tam == Resposta[j+1].tam)
						Verifica_Ordem(Resposta, j, j+1);
				}
			}
		}
			


}

// VERIFICANDO A CONDICAO 4 DA ORDENACAO
void Verifica_Ordem(Caminho* Resposta, int p1, int p2){
	int i;

	for (i = 0; i < Resposta[p1].tam; i++){

		if(Resposta[p1].vet[i] > Resposta[p2].vet[i]){	
			swap(Resposta, p1, p2);
			return ;
		}else if(Resposta[p1].vet[i] < Resposta[p2].vet[i])
			return ;
	}

}
// TROCANDO OS CAMINHOS DE POSICOES
void swap(Caminho* c, int p1, int p2){
	int i;

	Caminho aux = c[p1];
	c[p1] = c[p2];
	c[p2] = aux;

	


}
