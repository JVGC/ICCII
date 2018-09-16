#include <stdio.h>
#include <stdlib.h>
#include "terminal.h"
#include "bubble.h"

// SWAP NOS NÓS
void swap(no_t* t, int p1, int p2){

	no_t aux = t[p1];
	t[p1] = t[p2];
	t[p2] = aux;

}
// ORDENA A TABELA HASH EM ORDEM CRESCENTE DE BANCO

void bubble_sort(terminal_t* t){
	int i, j;

	for(i = 1; i < t->tam_table; i++){
		for(j = 0; j < t->tam_table - i; j++){
			if(t->table[j].banco > t->table[j+1].banco){
				swap(t->table, j, j+1);
			}
		}
	}
}