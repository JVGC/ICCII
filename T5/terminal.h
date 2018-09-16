#ifndef _TERMINAL_H_
#define _TERMINAL_H_

// STRUCT PARA GUARDAR A AUDITORIA
typedef struct auditoria
{
	char op; // OPERACAO A SER AUDITADA
	int terminal; // TERMINAL A SER AUDITADO
	int n; // QUANTAS PRIMEIRAS OPERACOES
}auditoria;

// STRUCT GUARDADA NA TABELA HASH
typedef struct no{
	int terminal; // TERMINAL DO NO
	int banco; // BANCO DO NO
	char op; // OPERACAO DO NO
	double Tvalor_pos; // TRANSFERENCIAS RECEBIDAS
	double Tvalor_neg; // TRANSFERENCIAS FEITAS
	double Svalor; // VALOR SACADO
	double Dvalor; // VALOR DEPOSITADO
	int destiny; // CASO DE TRANSFERENCIA ENTRE BANCOS DIFERENTE
	// BANCO DE DESTINO

}no_t;

// STRUCT PARA GUARDAR A QUANTIDADE DE ERROS
typedef struct{
	int dep, saq, trans, con; // CONTADOR PARA CADA OPERACAO 
	int total; // CONTADOR TOTAL
}erro_t;

// TABELA HASH DO TERMINAL
typedef struct{
	no_t* table; // VETOR DE NOS
	int tam_table; // TAMANHO DA TABELA
	int nOP; // NUMERO DE OPERACOES DA TABELA
	erro_t err; // STRUCT DE ERRO DA TABELA
}terminal_t;


terminal_t* cria_tabela(int tam); // CRIA A TABELA HASH A PARTIR DE UM TAMANHO
void libera_tabela(terminal_t* t);
void insere2(terminal_t* t, no_t* x, float A); // INSERE UM NÓ NOS TERMINAIS NORMAIS E NO TERMINAL TOTAL
void insere_auditoria(terminal_t* t, no_t* x); // UTILIZADO PARA CRIAR A TABELA HASH DAS AUDITORIAS
void insere_erro(terminal_t* t, no_t* x); // INSERE ERROS EM UMA TABELA HASH
void imprimir_relatorio(terminal_t* t); // IMPRIME O RELATORIO NORMAL
void imprimir_auditoria(terminal_t* t, auditoria* a); // IMPRIME A AUDITORIA
void imprimir_erro(terminal_t* t); // IMPRIME O RELATORIO DE ERRO

#endif