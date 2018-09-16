#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexical.h"



// ALOCA A MEMORIA E INICIALIZA OS CONTADORES EM 0
parser_t* cria_parser(){
	parser_t* p = (parser_t*) malloc(sizeof(parser_t));
	p->nObjects =0;
	p->nStrings =0;
	p->nArrays =0;
	p->nNumbers =0;
	p->nPairs = 0;
	p->nFalses =0;
	p->nTrues = 0;
	p->nNulls =0;

	return p;
}

int OBJECT(parser_t* p, char* json, int* i){
	int j;
	int tam = strlen(json); // TAMANHO DO JSON
	char c; // CHAR AUXILIAR PARA FACILITAR NAS COMPARACOES DO IF
	

	for(j = *i+1 ; j < tam; j++){ // j RECEEBE i + 1, pois o valor de i, já foi considerado ao chamar a função	
		c = json[j]; 

		if(c == '{'){ // se encontrar outra abertura de objeto, 	
			if(OBJECT(p, json, &j) == -1) // CHAMA A FUNÇÃO NOVAMENTE PARA CONSIDERAR O OBJETO MAIS INTERNO PRIMEIRO
				return -1; // SE HOUVER ERRO NESSE OBJETO, RETORNA -1

		}
		if(c == '}'){ // ENCOTRANDO O FINAL DE UM OBJETO
			*i = j; // ATUALIZA A QUANTIDADE POSICOES PERCORRIDAS NO VETOR
			p->nObjects++; // AUMENTA O CONTADORES
			return 0; // RETORNA 0 PARA SUCESSO
		}
		if(c == '"'){ // ENCONTRANDO UMA STRING
			if(STRING(p,json, &j) == -1){
				return -1;	// SE HOUVE ERRO, RETORNA -1
			}else{
				while(c != ':' && j < tam){ // COMO A STRING ESTA DENTRO DE UM OBJETO, PODE HAVER UM PAIR
				 // ENTÃO, PROCURA POR UM ':' PARA PROCURAR O PAIR
					j++; // ATUALIZANDO SEMPRE O j
					c=json[j];

					if(c == '}'){ // SE HOUVER UM FIM DE OBJETO APÓS A STRING, DESFAZ O J SOMADO ANTERIORMENTE, E SAI DO WHILE
						j--; 
						break;
					}
					if(c == ':'){ // ENCONTRANDO O ':', CHAMA A FUNÇÃO PAIR
						
						if(PAIR(p, json, &j) == 0){
							break;
						}else
							return -1;
					}
					
				}
			}
		
		}
		 if(c == '['){ // ENCONTRANDO UM ARRAY
			if(ARRAY(p, json, &j) == -1){
				return -1;	// RETORNA ERRO SE HOUVER PROBLEMA NO ARRAY
			}
		}
		if(c == 'f') // SE ENCONTRAR UM 'f', QUER DIZER QUE EH POSÍVEL HAVER UM FALSE
			if(FALSO(p, json, &j) == -1) // SE NÃO HOUVE UM FALSE, ENTÃO O OBJETO ESTA ERRADO
				return -1;
		if(c == 't') // SE ENCONTRAR UM 't', QUER DIZER QUE EH POSÍVEL HAVER UM FALSE
			if(VERDADEIRO(p, json, &j) == -1) // SE NÃO HOUVE UM TRUE, ENTÃO O OBJETO ESTA ERRADO
				return -1;
		if(c == 'n') // SE ENCONTRAR UM 'n', QUER DIZER QUE EH POSÍVEL HAVER UM FALSE
			if(NULO(p, json, &j) == -1) // SE NÃO HOUVE UM NULL, ENTÃO O OBJETO ESTA ERRADO
				return -1;						
		if(c >= 48 && c <= 57) // SE HOUVER UM NUMERO, HÁ UM ERRO, POIS NÃO PODE HAVER NUMERO FORA DE UM ARRAY
			return -1;

	}

	return -1; // SE ACABAR O json E CHEGAR AQUI, QUER DIZER QUE NÃO FECHOU O OBJETO(RETORNA ERRO)
}

int STRING(parser_t* p, char* json, int* i){
	
	
	int j;
	int tam = strlen(json);
	char c; // CHAR AUXILIAR PARA FACILITAR NAS COMPARACOES DO IF
	for(j = *i+1; j < tam; j++){ // j RECEEBE i + 1, pois o valor de i, já foi considerado ao chamar a função
		c = json[j];
		if(c == '"'){
            if(json[j+1] == '"') // SE HOUVER UMA ASPAS APÓS A PRÓPRIA ASPAS, HÁ ERRO POIS NÃO PODE HAVER ASPAS DENTRO DE UMA STRING
                return -1;
			*i=j; // ATUALIZA A QUANTIDADE DE POSICOES PERCORRIDAS NA STRING
			p->nStrings++; // ATUALIZA O CONTADOR DE STRINGS
			return 0;
		}
		else if(c == 92){ // SE HOUVER UMA '/', CONFERE SE NÃO HÁ ALGUMA COISA QUE NÃO PODE HAVER APÓS A '\'
			// NÃO PODE HAVER APÓS A BARRA: b, f, n, r, t, u, \, "
			if((json[j+1] == 'b') || (json[j+1] == 'f') || (json[j+1] == 'n') || (json[j+1] == 'r') || (json[j+1] == 't') || (json[j+1] == 'u') || (json[j+1] == 92) || (json[j+1] == '"'))
				return -1;

		}	
		
	}	
	return -1; // SE ACABAR O json E CHEGAR AQUI, QUER DIZER QUE NÃO FECHOU A STRING(RETORNA ERRO)
}

int ARRAY(parser_t* p, char* json, int* i){

	int j;
	int tam = strlen(json);
 	char c; // CHAR AUXILIAR PARA FACILITAR NAS COMPARACOES DO IF

	for(j=*i+1; j< tam; j++){ // j RECEEBE i + 1, pois o valor de i, já foi considerado ao chamar a função
		c = json[j];

		if(c == '[') // CHAMA A FUNÇÃO NOVAMENTE PARA CONSIDERAR O ARRAY MAIS INTERNO PRIMEIRO
			if(ARRAY(p, json, &j) == -1) 
				return -1; // RETORNA ERRO CASO HAJA ERRO NO ARRAY MAIS INTERNO
		if(c == ']'){ // ENCONTRANDO O FINAL DE UM ARRAY
			*i=j; // ATUALIZA A QUANTIDADE DE POSICOES PERCORRIDAS NA STRING
			p->nArrays++; // ATUALIZA O CONTADOR DE ARRAYS
			return 0;
		}
		if(c == '"'){ // ENCONTRANDO UMA STRING, CHAMA A FUNÇÃO STRING E RETORNA ERRO CASO HAJA ERRO NA STRING
			if(STRING(p,json, &j) == -1){
				return -1;	
			}
		}
		if(c == '{'){ // ENCONTRANDO UM OBJETO, CHAMA A FUNÇÃO OBJETO E RETORNA ERRO CASO HAJA ERRO NO OBJETO
			if(OBJECT(p,json, &j) == -1){
				return -1;	
			}
		}
		if(c == 'f'){ //ENCONTRADO UM f, CHAMA A FUNÇÃO FALSE PARA VERIFICAR SE HA O FALSE	
			if(FALSO(p, json, &j) == -1)
				return -1;
		}	
		if(c == 't'){ //ENCONTRADO UM t, CHAMA A FUNÇÃO FALSE PARA VERIFICAR SE HA O FALSE	
			if(VERDADEIRO(p, json, &j) == -1)
				return -1;
		}
		if(c == 'n'){ //ENCONTRADO UM n, CHAMA A FUNÇÃO FALSE PARA VERIFICAR SE HA O FALSE
			if(NULO(p, json, &j) == -1)
				return -1;
		}
		if((c >=48 && c <=57) || c== '+' || c == '-'){ // SE HOUVER UM NUMERO OU UM SINAL, HÁ A POSSIBILIDADE HAVER UM NUMBER	
			if(NUMBER(p, json, &j)== -1)
				return -1;
		}		
		if(c == ':') // SE HOUVER UM ':'  DENTRO DE UM ARRAY, HÁ ERRO POIS NÃO PODE HAVER UM PAIR DENTRO DE UM ARRAY
			return -1;	

	}
	return -1; // SE ACABAR O json E CHEGAR AQUI, QUER DIZER QUE NÃO FECHOU O ARRAY(RETORNA ERRO)

}


int PAIR(parser_t* p, char* json, int* i){
	int j;
	int tam = strlen(json);
	char c; // CHAR AUXILIAR PARA FACILITAR NAS COMPARACOES DO IF


	for(j = *i+1 ; j < tam; j++){	
		c = json[j];

		if(c == '{'){ // SE HOUVER UMA ABERTURA DE UM OBJETO, CHAMA OBJETO PARA VER SE ACONTECEU O PAIR	
			if(OBJECT(p, json, &j) == -1)
				return -1;
			else{	
				*i = j; // ATUALIZA A QUANTIDADE DE POSICOES PERCORRIDAS NA STRING
				p->nPairs++; // ATUALIZA O CONTADOR DE PAIRS
				return 0;
			}
		}
		if(c == '"'){ // SE HOUVER UMA ABERTURA DE UMA STRING, CHAMA STRING PARA VER SE ACONTECEU O PAIR
			if(STRING(p,json, &j) == -1)
				return -1;	
			else{	
				*i = j; // ATUALIZA A QUANTIDADE DE POSICOES PERCORRIDAS NA STRING
				p->nPairs++; // ATUALIZA O CONTADOR DE PAIRS
				return 0;
			}	
		}
		 if(c == '['){
			if(ARRAY(p, json, &j) == -1){ // SE HOUVER UMA ABERTURA DE UM ARRAY, CHAMA ARRAY PARA VER SE ACONTECEU O PAIR
				return -1;	
			}
			else{	
				*i = j; // ATUALIZA A QUANTIDADE DE POSICOES PERCORRIDAS NA STRING
				p->nPairs++; // ATUALIZA O CONTADOR DE PAIRS
				return 0;
			}
		}
		if((c >=48 && c <=57) || c== '+' || c == '-'){ // SE HOUVER UM NUMERO OU UM SINAL(+,-), CHAMA NUMBER PARA VER SE ACONTECEU O PAIR	
			if(NUMBER(p, json, &j) == -1)
				return -1;
			else{
				*i = j; // ATUALIZA A QUANTIDADE DE POSICOES PERCORRIDAS NA STRING
				p->nPairs++; // ATUALIZA O CONTADOR DE PAIRS
				return 0;
			}
		}
		if(c == 'f'){ // SE HOUVER UM f, CHAMA FALSO PARA VER SE ACONTECEU O PAIR
			if(FALSO(p, json, &j) == -1)
				return -1;
			else{	
				*i = j; // ATUALIZA A QUANTIDADE DE POSICOES PERCORRIDAS NA STRING
				p->nPairs++; // ATUALIZA O CONTADOR DE PAIRS
				return 0;
			}
		}
		if(c == 't'){ // SE HOUVER UM t, CHAMA VERDADEIRO PARA VER SE ACONTECEU O PAIR
			if(VERDADEIRO(p, json, &j) == -1)
				return -1;
			else{	
				*i = j; // ATUALIZA A QUANTIDADE DE POSICOES PERCORRIDAS NA STRING
				p->nPairs++; // ATUALIZA O CONTADOR DE PAIRS
				return 0;
			}
		}
		if(c == 'n'){ // SE HOUVER UM n, CHAMA NULO PARA VER SE ACONTECEU O PAIR
			if(NULO(p, json, &j) == -1) 
				return -1;		
			else{	
				*i = j; // ATUALIZA A QUANTIDADE DE POSICOES PERCORRIDAS NA STRING
				p->nPairs++; // ATUALIZA O CONTADOR DE PAIRS
				return 0;
			}
		}
	}

	return -1;// SE ACABAR O json E CHEGAR AQUI, QUER DIZER QUE NÃO FECHOU O PAIR(RETORNA ERRO)
}

int FALSO(parser_t* p, char* json, int* i){

	int j = *i;
	
	if(json[j] == 'f' && json[j+1] == 'a' && json[j+2] == 'l' && json[j+3] == 's' && json[j+4] == 'e'){ // VERIFICA SE O QUE ESTA ESCRITO EH 'false'		
		if(json[j+5] == ',' || json[j+5] == '{' || json[j+5] == '}'){	
			*i = j+4; // ATUALIZA A QUANTIDADE DE POSICOES PERCORRIDAS NA STRING
			p->nFalses++; // ATUALIZA O CONTADOR DE FALSES
			return 0;
		}
		return -1;
	}

	return -1; // SE ACABAR O json E CHEGAR AQUI, QUER DIZER QUE NÃO HOUVE O false(RETORNA ERRO)

}

int VERDADEIRO(parser_t* p, char* json, int* i){

	int j = *i;
	
	if(json[j] == 't' && json[j+1] == 'r' && json[j+2] == 'u' && json[j+3] == 'e' ){ // VERIFICA SE O QUE ESTA ESCRITO EH 'true'		
		*i = j+3; // ATUALIZA A QUANTIDADE DE POSICOES PERCORRIDAS NA STRING
		p->nTrues++; // ATUALIZA O CONTADOR DE TRUES
		return 0;
	}					
	return -1; // SE ACABAR O json E CHEGAR AQUI, QUER DIZER QUE NÃO HOUVE O true(RETORNA ERRO)

}

int NULO(parser_t* p, char* json, int* i){

	int j = *i;
	if(json[j] == 'n' && json[j+1] == 'u' && json[j+2] == 'l' && json[j+3] == 'l'){ // VERIFICA SE O QUE ESTA ESCRITO EH 'null'		
		*i = j+3; // ATUALIZA A QUANTIDADE DE POSICOES PERCORRIDAS NA STRING
		p->nNulls++; // ATUALIZA O CONTADOR DE NULLS
		return 0;
	}			
						
	return -1; // SE ACABAR O json E CHEGAR AQUI, QUER DIZER QUE NÃO HOUVE  null(RETORNA ERRO)

}

int NUMBER(parser_t* p, char* json, int* i){
	int j;
	int tam = strlen(json);
	char c; // CHAR AUXILIAR PARA FACILITAR NAS COMPARACOES DO IF


	for(j = *i; j<tam; j++){
		c = json[j]; 
		if(c == '+' || c == '-'){ // SE FOR UM SINAL, VERIFICA SE O QUE ESTA APOS EH DIFERENTE DE UM NUMERO, UM '.' E UM EXPONENCIAL
			if((json[j+1]<= 47 || json[j+1]>=58) && json[j+1] != '.' && json[j+1] != 'e' && json[j+1] != 'E' )
				return -1; // SE FOR, HÁ ERRO
		}	
		if((c <= 47 || c>=58) && c != '.' && c != 'e' && c != 'E' && c != '+' && c != '-')
			break; // SE HOUVER ALGO DIFERENTE DE NUMERO, DE '.' OU DE UM EXPONENCIAL, SAI DO FOR, POIS O NUMERO TERMINOU
		if(c == 'E' || c == 'e'){ // SE FOR UM EXPONENCIAL, VERIFICA SE HÁ UM EXPONENCIAL APÓS ELE(DOIS EXPONENCIAIS SEGUIDOS)
			if(json[j+1] == 'E' || json[j+1] == 'e') // SE HOUVER, HÁ ERRO
				return -1;
		}

	}
	*i=j-1; // COMO ELE SAI DO FOR, SE ENCONTRAR ALGO DEPOIS DO NUMERO, ESTE ALGO TEM QUE SER CONSIDERADO EM OUTRA FUNÇÃO, POR ISSO O -1
	p->nNumbers++; // ATUALIZA O CONTADOR DE NUMEROS
	return 0; // RETORNA SUCESSO

}