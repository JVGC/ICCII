#ifndef _LEXICAL_H
#define _LEXICAL_H

typedef struct parser{
	int nObjects, nArrays, nPairs, nStrings, nNumbers, nTrues, nFalses, nNulls; // CONTADORES PARA CADA TIPO DO JSON

}parser_t;

parser_t* cria_parser(); // FUNCAO QUE CRIA A STRUTURA E INICIALIZA OS CONTADORES EM 0


int OBJECT(parser_t* p, char* json, int* i); // FUNCAO CHAMADA QUANDO ENCONTRA UM OBJETO
int STRING(parser_t* p, char* json, int* i); // FUNCAO CHAMADA QUANDO ENCONTRA UMA STRING
int ARRAY(parser_t* p, char* json, int* i); // FUNCAO CHAMADA QUANDO ENCONTRA UM ARRAY
int PAIR(parser_t* p, char* json, int* i); // FUNCAO CHAMADA QUANDO ENCONTRA UM PAIR
int VERDADEIRO(parser_t* p, char* json, int* i); // FUNCAO CHAMADA QUANDO ENCONTRA UM TRUE
int FALSO(parser_t* p, char* json, int* i); //FUNCAO CHAMADA QUANDO ENCONTRA UM FALSE
int NULO(parser_t* p, char* json, int* i); //FUNCAO CHAMADA QUANDO ENCONTRA UM NULL
int NUMBER(parser_t* p, char* json, int* i); //FUNCAO CHAMADA QUANDO ENCONTRA UM NUMBER










#endif