#ifndef _BUBBLE_H_
#define _BUBBLE_H_ 

#include "process.h"

typedef process_t elem; // O ELEMENTO EH UM VETOR DE PROCESSOS

// ORDENA OS ELEMENTOS DO VETOR
void Bubble_Sort(elem* vet, int n, int FLAG);
void swap(elem* vet, int p1, int p2); // TROCA OS ELEMENTOS DE POSICOES


#endif