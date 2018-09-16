#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "terminal.h"
#include "bubble.h"


int main(){
	// DUAS VARIAVES QUE SERAO UTILIZADA PARA VERIFICACAO DE AUDITORIA
	int aux_int = 0, flag =0;
	
	double A; // CONSTANTE PARA A FUNCAO DE HASHING
	
	terminal_t* t1 = cria_tabela(100); // TERMINAL 1
	terminal_t* t2 = cria_tabela(103); // TERMINAL 2
	terminal_t* t3 = cria_tabela(59); // TERMINAL 3
	terminal_t* t4 = cria_tabela(97); // TERMINAL 4
	terminal_t* total = cria_tabela(53); // TERMINAL TOTAL
	terminal_t* a1 = cria_tabela(100); // AUDITORIA 1
	terminal_t* a2 = cria_tabela(100); // AUDITORIA 2
	terminal_t* a3 = cria_tabela(100); // AUDITORIA 3
	terminal_t* a4 = cria_tabela(100); // AUDITORIA 4
	terminal_t* e1 = cria_tabela(97); // TERMINAL 1 DE ERRO 
	terminal_t* e2 = cria_tabela(97); // TERMINAL 2 DE ERRO
	terminal_t* e3 = cria_tabela(97); // TERMINAL 3 DE ERRO
	terminal_t* e4 = cria_tabela(97); // TERMINAL 4 DE ERRO

	char aux1[100], aux2[100]; // STRING PARA A LEITURA

	// STRUCTS AUXILIARES PARA FAZER A LEITURA
	no_t* banco = (no_t*) calloc(1, sizeof(no_t));
	auditoria* a = (auditoria*) calloc(1, sizeof(auditoria));
	// LE A PRIMEIRA LINHA E ENTRA NUM DO/WHILE
	fgets(aux1, 100, stdin);

	do{
		//TRASFORMA AUX1 EM UM NO, UTILIZANDO SSCANF PARA A LEITURA
		sscanf(aux1,"%d %d", &banco->terminal, &aux_int);
		// SETA O BANCO DE DESTINO PARA 0, PARA EVITAR RESQUISCIOS
		banco->destiny = 0;
		// SE O AUX_INT FOR 0
		if(aux_int == 0){ // EH UMA AUDITORIA;
			flag = 1; // SETO A FLAG EM 1, E SAIO DO LOOP
			break;
		}
		// RESETO AUX_INT EM 0
		aux_int = 0;
		// EH UMA OPERACAO NORMAL ENTAO
		// LEIO OS 3 PRIMEIROS NUMEROS
		sscanf(aux1,"%d %d %c", &banco->terminal, &banco->banco, &banco->op);
		// SE A OPERACAO FOR DE TRANSFERENCIA, DEPOSITO OU SAQUE
		// AINDA HA MAIS COISAS PARA LER

		// SE FOR TRANSFERENCIA
		if(banco->op == 'T'){
			// CONTO O NUMERO DE ESPACOS NA STRING
			int nSpaces = 0;
			for (int i = 0; i < strlen(aux1); i++) {
				if (aux1[i] == ' ') 
					nSpaces++;
			}
			// SE FOREM 4 => TRANSFERENCIA ENTRE BANCOS DIFERENTES
			if(nSpaces == 4)
				sscanf(aux1,"%d %d %c %d %lf", &banco->terminal, &banco->banco, &banco->op, &banco->destiny, &banco->Tvalor_pos);
			else{ // TRANSFERENCIA ENTRE MESMO BANCO
				sscanf(aux1,"%d %d %c %lf", &banco->terminal, &banco->banco, &banco->op, &banco->Tvalor_pos);
			}
			
		// LEIO PARA CASO DE SER SAQUE OU DEPOSITO
		// O VALOR LIDO PARA ESSAS DUAS OPERACOES
		// SAO GUARDADAS NA MESMA VARIAVEL, POIS ISSO NAO FAZ DIFERENCA
		}else if(banco->op == 'D' || banco->op == 'S'){
			sscanf(aux1,"%d %d %c %lf", &banco->terminal, &banco->banco, &banco->op, &banco->Svalor);
		}

		// LEIO A PROXIMA LINHA
		// SE NAO HOUVER MAIS NADA A SER LIDO
		// SAI DO LOOP
		if(!fgets(aux2, 100, stdin))
			break;

		// SE A PROXIMA LINHA FOR ERRO
		// TENHO QUE INSERIR A LINHA ANTERIOR NA HASH DE ERRO
		// HA UM \r NA LINHA DE BAIXO, POR ISSO 
		// ESSE STRCMP RETORNA -13 CASO HAJA ERRO
		if(strcmp("ERRO", aux2) == -13){
			// COLOCA AUX1 NA HASH DE ERRO
			// AUMENTANDO OS ERROS NA HASH TOTAL
			total->err.total++;
			if(banco->op == 'S')
				total->err.saq++;
			else if(banco->op == 'D')
				total->err.dep++;
			else if(banco->op == 'C')
				total->err.con++;
			else if(banco->op == 'T')
				total->err.trans++;

			if(banco->terminal == 1){ //TERMINAL 1 DE ERRO
				insere_erro(e1, banco);
				e1->err.total++;
			}else if(banco->terminal == 2){ //TERMINAL 2 DE ERRO
				insere_erro(e2, banco);
				e2->err.total++;
			}else if(banco->terminal == 3){ //TERMINAL 3 DE ERRO
				insere_erro(e3, banco);
				e3->err.total++;
			}else{ //TERMINAL 4 DE ERRO
				insere_erro(e4, banco);
				e4->err.total++;
			}

			// LEIO A PROXIMA LINHA COM AUX1
			fgets(aux1, 100, stdin);
				

		}else{ // SE NAO FOR ERRO
			// AUMENTANDO O NUMERO DE OPERACOES NO TOTAL
			total->nOP++;
			// INSIRO A OPERACAO NA HASH TOTAL
			A = 0.841;
			insere2(total, banco, A);
			// E DEPOIS INSIRO A OPERACAO NA HASH NORMAL
			// E NA AUDITORIA
				if(banco->terminal == 1){ //TERMINAL 1
					A = 0.476;
					t1->nOP++;
					insere2(t1, banco, A);
					insere_auditoria(a1, banco);
				}else if(banco->terminal == 2){ //TERMINAL 2
					A = 0.356;
					t2->nOP++;
					insere2(t2, banco,A);
					insere_auditoria(a2, banco);
				}else if(banco->terminal == 3){ //TERMINAL 3
					A = 0.675;
					t3->nOP++;
					insere2(t3, banco, A);
					insere_auditoria(a3, banco);
				}else if(banco->terminal == 4){ //TERMINAL 4
					A = 0.843;
					t4->nOP++;
					insere2(t4, banco, A);
					insere_auditoria(a4, banco);
				}
			
			// RESETO ESSAS VARIAVEIS PARA NAO HAVER RESQUISCIOS

			banco->Tvalor_pos = 0;
			banco->terminal = 0;
			// COPIO AUX2 PARA AUX1
			strcpy(aux1, aux2);
			
		}
		
	}while(1);
	
	// SE AO SAIR DO WHILE A FLAG ESTIVER EM 0
	// EH PORQUE NAO HOUVER PEDIDO DE AUDITORIA
	// PORTANTO HA AINDA MAIS UMA OPERACAO QUE JA FOI LIDA
	// MAS AINDA NAO FOI COMPUTADA

	// COLOCO ELA NA HASH DA MESMA FORMA ACIMA
	if(!flag){	
		sscanf(aux1,"%d %d %c", &banco->terminal, &banco->banco, &banco->op);
		banco->destiny = 0;
		if(banco->op == 'T'){
			int nSpaces = 0;
			for (int i = 0; i < strlen(aux1); i++) {
				if (aux1[i] == ' ') 
					nSpaces++;
			}
			if(nSpaces == 4)
				sscanf(aux1,"%d %d %c %d %lf", &banco->terminal, &banco->banco, &banco->op, &banco->destiny, &banco->Tvalor_pos);
			else{
				sscanf(aux1,"%d %d %c %lf", &banco->terminal, &banco->banco, &banco->op, &banco->Tvalor_pos);
			}
				
		
		}else if(banco->op == 'D' || banco->op == 'S'){
			sscanf(aux1,"%d %d %c %lf", &banco->terminal, &banco->banco, &banco->op, &banco->Svalor);
		}
		total->nOP++;
		A = 0.841;
		insere2(total, banco, A);
		if(banco->terminal == 1){ //TERMINAL 1
			A = 0.476;
			t1->nOP++;
			insere2(t1, banco, A);
			insere_auditoria(a1, banco);
		}else if(banco->terminal == 2){ //TERMINAL 2
			A = 0.356;
			t2->nOP++;
			insere2(t2, banco, A);
			insere_auditoria(a2, banco);
		}else if(banco->terminal == 3){ //TERMINAL 3
			A = 0.675;
			t3->nOP++;
			insere2(t3, banco, A);
			insere_auditoria(a3, banco);
		}else if(banco->terminal == 4){ //TERMINAL 4
			A = 0.843;
			t4->nOP++;
			insere2(t4, banco, A);
			insere_auditoria(a4, banco);
		}
				
		banco->Tvalor_pos = 0;
		banco->terminal = 0;
		strcpy(aux1, aux2);
	}

	// ORDENO CADA UM DOS TERMINAIS USANDO BUBBLE SORT
	bubble_sort(t1);
	bubble_sort(t2);
	bubble_sort(t3);
	bubble_sort(t4);
	bubble_sort(total);

	// IMPRIMO OS RELATORIOS
	printf("===TERMINAL 1===\n");
	imprimir_relatorio(t1);
	imprimir_erro(e1);
	printf("===TERMINAL 2===\n");
	imprimir_relatorio(t2);
	imprimir_erro(e2);
	printf("===TERMINAL 3===\n");
	imprimir_relatorio(t3);
	imprimir_erro(e3);
	printf("===TERMINAL 4===\n");
	imprimir_relatorio(t4);
	imprimir_erro(e4);
	printf("===TOTAL===\n");
	imprimir_relatorio(total);
	imprimir_erro(total);
	// SE HOUVER AUDITORIA
	// IMPRIMO A AUDITORIA
	if(flag){
		printf("===AUDITORIA===\n");
		while(1){
			sscanf(aux1, "%d %c %d", &a->terminal, &a->op, &a->n);
			if(a->terminal == 1)
				imprimir_auditoria(a1, a);
			else if(a->terminal == 2)
				imprimir_auditoria(a2, a);
			else if(a->terminal == 3)
				imprimir_auditoria(a3, a);
			else if(a->terminal == 4)
				imprimir_auditoria(a4, a);
			if(feof(stdin))
				break;
			fgets(aux1, 100, stdin);
		}
	}

	// LIBERA TODAS AS MEMORIAS ALOCADAS

	libera_tabela(t1);
	libera_tabela(t2);
	libera_tabela(t3);
	libera_tabela(t4);

	libera_tabela(a1);
	libera_tabela(a2);
	libera_tabela(a3);
	libera_tabela(a4);

	libera_tabela(e1);
	libera_tabela(e2);
	libera_tabela(e3);
	libera_tabela(e4);

	libera_tabela(total);

	free(banco);
	free(a);


	return 0;
}