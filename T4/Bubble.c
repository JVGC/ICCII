#include <stdio.h>
#include <stdlib.h>
#include "Bubble.h"
#include "process.h"

void Bubble_Sort(elem* vet, int n, int FLAG){	
	// RODANDO O BUBBLE SORT:
	// ORDEM DE ORDENACAO: 
	// 1- TEMPO DE RECONHECIMENTO(t0)
	// 2 (SE FOR FIFO) - CODIGO(ORDEM ASCENDENTE)
	// 2 (SE FOR ROUND ROBIN) - PRIORIDADE(PRIORIDADE 0 TEM MAIOR PRIORIDADE QUE OS OUTROS)
	// 3- CODIGO
	int i, j;
	for(i = 1; i < n; i++)
		for(j = 0; j < n-i; j++){	
			if(vet[j].t0 > vet[j+1].t0)
				swap(vet, j, j+1);
			else if(vet[j].t0 == vet[j+1].t0){
				if(FLAG == FIFO){	
					if(vet[j].code > vet[j+1].code)
						swap(vet, j, j+1);
				}else{
					if(vet[j+1].r == 0)
						swap(vet, j, j+1);
					else if(vet[j].r < vet[j+1].r)
						swap(vet, j, j+1);
					else if(vet[j].r == vet[j+1].r){
						if(vet[j].code > vet[j+1].code)
							swap(vet, j, j+1);
					}
				}
			}
		}
			


}
// TROCANDO OS ELEMENTOS DE POSICOES
void swap(elem* vet, int p1, int p2){

	elem aux = vet[p1];
	vet[p1] = vet[p2];
	vet[p2] = aux;

}
