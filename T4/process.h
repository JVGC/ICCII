#ifndef _PROCESS_H_
#define _PROCESS_H_

// FLAGS QUE AJUDAM NA ORDENACAO PARA SABER SE DEVE OU NAO SER LEVADA A PRIORIDADE EM CONSIDERACAO
#define FIFO 0
#define RR 1


struct process{
	int code, t0, tf, r; // CODIGO, TEMPO DE ENTRADA, TEMPO DE EXECUCAO E PRIORIDADE RESPECTIVAMENTE

};

typedef struct process process_t;

void Round_Robin(); // RODA O ALGORITMO ROUND ROBIN


#endif