#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "terminal.h"

terminal_t* cria_tabela(int tam){	

	// ALOCA A TABELA E CASO SEJA BEM SUCEDIDO
	terminal_t* t = (terminal_t*) malloc(sizeof(terminal_t));
	if(t != NULL){
		t->tam_table = tam; 
		//ALOCA O VETOR COM O TAMANHO DADO
		t->table = (no_t*) calloc(tam, sizeof(no_t));
		t->nOP = 0; // NUMERO DE OPERACOES EH 0
	}
	return t;
}

void libera_tabela(terminal_t* t){
	free(t->table);
	free(t);
}

void insere2(terminal_t* t, no_t* x, float A){
	// CALCULA A POSICAO UTILZANDO O METODO DA MULTIPLICAO
	int pos = (int) (((A * x->banco) - floor(A * x->banco)) * t->tam_table);
	int i = pos;
	int j;
	int flag;
	// SE A OPERACAO FOR CONSULTA, NAO HA NADA PARA FAZER NA TABELA
	if(x->op == 'C')
		return;
	// TRATANDO AS COLISOES
	// SE ACABAR SAINDO DA TABELA, ELE VOLTA PARA A POSICAO 0
	while(t->table[i].banco != 0 && t->table[i].banco != x->banco){
		i++;
		if(i > t->tam_table)
			i =0;

	}	
	// SE A POSICAO ESTIVER VAZIA, INSERE O NOVO NÓ NELA
	if(t->table[i].banco == 0){ // posicao vazia
		t->table[i].banco = x->banco;
		flag = 1;
	}
	// SE A OPERACAO FOR SAQUE
	if(x->op == 'S'){
		t->table[i].Svalor += x->Svalor;
	}else if(x->op == 'D'){ // SE A OPERACAO FOR DEPOSITO
		t->table[i].Dvalor +=x->Svalor;
	}else if(x->op == 'T'){ // SE A OPERACAO FOR TRANSFERENCIA
		if(x->destiny == 0){ // TRANSFERENCIA ENTRE MESMO BANCO
			// SE O BANCO HOUVER SIDO CRIADO NESSA CHAMADA DA FUNCAO
			// ENTAO RETIRA O BANCO DA HASH
			if(flag == 1)
				t->table[i].banco = 0;
			return ;
		}else{ // TRANSFERENCIAS ENTRE BANCOS DIFERENTES
			// CALCULA A POSICAO DO BANCO DESTINO
			j = (int) (((A * x->destiny) - floor(A * x->destiny)) * t->tam_table);
			// PERCORRE ATE ACHAR ELE OU ACHAR UMA POSICAO VAZIA
			while(t->table[j].banco != 0 && t->table[j].banco != x->destiny){
				j++;
				if(j > t->tam_table)
					j = 0;
			}
			// SE A POSICAO ESTIVER VAZIA, PREENCHE ELA
			if(t->table[j].banco == 0){ // posicao vazia
				t->table[j].banco = x->destiny;
			}
			// INSERE OS VALORES DE TRANSFERENCIA EM CADA UM DOS BANCOS
			t->table[i].Tvalor_neg += x->Tvalor_pos;
			t->table[j].Tvalor_pos += x->Tvalor_pos;
		}
	}

}

void insere_auditoria(terminal_t* t, no_t* x){
	int pos;
	// A INSERCAO DA AUDITORIA DIVIDIU O VETOR EM 4 PARTES
	// ONDE CADA UMA DAS PARTES FICARÁ CADA UMA DAS 4 OPERACOES

	// SE FOR  SAQUE, FICARA NA PRIMEIRA PARTE DO VETOR
	if(x->op == 'S'){
		pos = 0;

	}// SE FOR  DEPOSITO, FICARA NA SEGUNDA PARTE DO VETOR
	if(x->op == 'D'){
		pos = (int)(t->tam_table/4);
	} // SE FOR  CONSULTA, FICARA NA TERCEIRA PARTE DO VETOR
	if(x->op == 'C'){
		pos =(int)(t->tam_table/4)*2;
	} // SE FOR  TRANSFERENCIA, FICARA NA QUARTA PARTE DO VETOR
	if(x->op == 'T'){
		pos =(int)(t->tam_table/4)*3;
	}

	int i = pos;
	int j;

	// COM A POSICAO CALCULADA, TRATA AS COLISOES
	while(t->table[i].banco != 0){
		i++;
	}
	// SE A OPERACAO FOR SAQUE OU DEPOSITO
	// INSERE A OPERACAO NA TABELA
	if(x->op == 'S' || x->op == 'D'){	
		t->table[i].banco = x->banco;
		t->table[i].op = x->op;
		t->table[i].Svalor = x->Svalor;
	}
	else if(x->op == 'T'){ // FAZ O MESMO COM TRANSFERENCIA
		t->table[i].banco = x->banco;
		t->table[i].op = x->op;
		t->table[i].Svalor = x->Tvalor_pos;
		// SE FOR TRANSFERENCIA ENTRE BANCOS DIFERENTES
		if(x->destiny != 0)
			t->table[i].destiny = x->destiny;
		else // SE FOR ENTRE MESMO BANCO, O DESTINO EH IGUAL A ORIGEM
			t->table[i].destiny = x->banco;
	}else if(x->op == 'C') // SE FOR CONSULTA
		t->table[i].op = x->op;

}

void insere_erro(terminal_t* t, no_t* x){
	int pos;
	// A INSERCAO DA ERRO, ACONTECE DA MESMA FORMA QUE NA AUDITORIA
	// ONDE CADA UMA DAS PARTES FICARÁ CADA UMA DAS 4 OPERACOES

	// SE FOR  SAQUE, FICARA NA PRIMEIRA PARTE DO VETOR
	if(x->op == 'S'){
		pos = 0;

	}// SE FOR  DEPOSITO, FICARA NA SEGUNDA PARTE DO VETOR
	if(x->op == 'D'){
		pos = (int)(t->tam_table/4);
	} // SE FOR  CONSULTA, FICARA NA TERCEIRA PARTE DO VETOR
	if(x->op == 'C'){
		pos =(int)(t->tam_table/4)*2;
	} // SE FOR  TRANSFERENCIA, FICARA NA QUARTA PARTE DO VETOR
	if(x->op == 'T'){
		pos =(int)(t->tam_table/4)*3;
	}
	int i = pos;
	int j;

	// COM A POSICAO CALCULADA, TRATA AS COLISOES
	while(t->table[i].banco != 0){
		i++;
	}
	// SE FOR SAQUE, INSERE A OPERACAO E SOMA O CONTADOR DE ERROS DE SAQUE
	if(x->op == 'S'){	
		t->err.saq++;
		t->table[i].banco = x->banco;
		t->table[i].op = x->op;
		t->table[i].Svalor = x->Svalor;
	} // O MESMO PARA TRANSFERENCIAS
	else if(x->op == 'T'){
		t->err.trans++;
		t->table[i].banco = x->banco;
		t->table[i].op = 'T';
		t->table[i].Svalor = x->Tvalor_pos;
		if(x->destiny != 0)
			t->table[i].destiny = x->destiny;
		else
			t->table[i].destiny = x->banco;
	}else if(x->op == 'D'){ // E O MESMO PARA DEPOSITO
		t->err.dep++;
		t->table[i].banco = x->banco;
		t->table[i].op = x->op;
		t->table[i].Svalor = x->Svalor;
	}else if(x->op == 'C') // E O MESMO PARA CONSULTA
		t->err.con++;

}

// IMPRIMINDO RELATORIO NORMAL
void imprimir_relatorio(terminal_t* t){
	int i;
	// PERCORRE A TABELA INTEIRA 
	// PRINTANDO CADA NÓ INSERIDO(BANCO != 0)
	for(i = 0; i < t->tam_table; i++){
		if(t->table[i].banco != 0){
			printf("Banco %d: Moeda +%.2f -%.2f Transferencia +%.2f -%.2f\n", t->table[i].banco, t->table[i].Dvalor,t->table[i].Svalor, t->table[i].Tvalor_pos, t->table[i].Tvalor_neg);
		}
	}
	// PRINTA O LUCRO FINAL
	printf("Lucro obtido: %.2f\n", t->nOP*3.0);
}

// IMPRIME A AUDITORIA
void imprimir_auditoria(terminal_t* t, auditoria* a){
	int cont = 0, pos;

	if(a->op == 'S'){
		printf("===SAQUE TERMINAL %d===\n",a->terminal);
		printf("Mostrando primeiros %d resultados\n", a->n);
		pos = 0;
		// IMPRIME A QUANTIDADE DE OPERACOES PEDIDAS
		while(cont < a->n ){	
			// COMO AS OPERACOES ESTAO EM SEQUENCIA
			// SE A POSICAO ESTIVER VAZIA
			// OU A OPERACAO FOR DIFERENTE DA PEDIDA
			// DEVE SE PARAR A IMPRESSAO

			// SE O CONTADOR FOR 0, SEM RESULTADOS
			if(t->table[pos].banco == 0 || t->table[pos].op != 'S'){
				if(cont == 0)
					printf("Sem resultados\n");
				return ; 
			}
			// IMPRIME OS NOS, AUMENTANDO O CONTADOR, E SEGUINDO PARA A PROXIMA POSICAO	
			printf("%d- Banco %d %.2f\n", cont+1, t->table[pos].banco,t->table[pos].Svalor);
			cont++;
			pos++;
		}

	}else if(a->op == 'D'){ // MESMA COISA PARA DEPOSITO
		printf("===DEPOSITO TERMINAL %d===\n",a->terminal);
		printf("Mostrando primeiros %d resultados\n", a->n);
		pos = (int)(t->tam_table/4);
		while(cont < a->n ){
			if(t->table[pos].banco == 0 || t->table[pos].op != 'D'){
				if(cont == 0)
					printf("Sem resultados\n");
				return ; 
			}
			printf("%d- Banco %d %.2f\n", cont+1, t->table[pos].banco,t->table[pos].Svalor);
			cont++;
			pos++;
		}

	}else if(a->op == 'C'){ // E PARA CONSULTA
		printf("===CONSULTA TERMINAL %d===\n",a->terminal);
		printf("Mostrando primeiros %d resultados\n", a->n);
		pos =(int)(t->tam_table/4)*2;
		while(cont < a->n ){
			if(t->table[pos].banco == 0 || t->table[pos].op != 'C'){
				if(cont == 0)
					printf("Sem resultados\n");
				return ; 
			}
			printf("%d- Banco %d\n ", cont+1, t->table[pos].banco);
			cont++;
			pos++;
		}

	}else if(a->op == 'T' ){ // ASSIM COMO PARA TRANSFERENCIA
		printf("===TRANSFERENCIA TERMINAL %d===\n",a->terminal);
		printf("Mostrando primeiros %d resultados\n", a->n);
		pos =(int)(t->tam_table/4)*3;
		while(cont < a->n ){
			if(t->table[pos].banco == 0 || t->table[pos].op != 'T'){
				if(cont == 0)
					printf("Sem resultados\n");
				return ; 
			}
			printf("%d- Banco origem %d Banco destino %d %.2f\n", cont+1, t->table[pos].banco,t->table[pos].destiny, t->table[pos].Svalor);
			cont++;
			pos++;
		}

	}
}


void imprimir_erro(terminal_t* t){
	// IMPRIME OS ERROS
	// SE NAO HOUVER ERROS, RETORNA
	if(t->err.total == 0)
		return;
	if(t->err.saq >0)
		printf("Erros de saque: %d\n",  t->err.saq);
	if(t->err.dep >0)
		printf("Erros de deposito: %d\n",  t->err.dep);
	if(t->err.con >0)
		printf("Erros de consulta: %d\n",  t->err.con);
	if(t->err.trans >0)
		printf("Erros de transferencia: %d\n",  t->err.trans);
	
	printf("Total de erros: %d\n",t->err.total);

}