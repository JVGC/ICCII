#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexical.h"


int main()
{
	int i;
	parser_t* p = cria_parser(); // CRIA A STRUTURA COM OS CONTADORES
	char json[200]; // STRING ESTÁTICA PARA GUARDAR O JSON LIDO
	char c; // CHAR AUXILIAR PARA FACILITAR NAS COMPARAÇÕES DE IFS
	int cont =0;// CONTADOR DE JSONS
	
	while(1){	
		fgets(json, 200, stdin); // LE O JSON
		int tam = strlen(json);
		for(i =0; i < tam; i++){
			c = json[i];
			if(c == '{'){ / SE ENCONTRAR UM COMEÇO DE OBJECT, CHAMA ELA
				if(OBJECT(p, json, &i) == -1){	
					printf("Error line %d\n", cont+1);
					break;
				}
			}else if(c == '"'){ // SE ENCONTRAR UM COMEÇO DE STRING, CHAMA ELA	
				if(STRING(p,json, &i) == -1){	
					printf("Error line %d\n", cont+1);
					break;
				}
			}else if(c == '['){ // SE ENCONTRAR UM COMEÇO DE ARRAY, CHAMA ELA
				if(ARRAY(p, json, &i) == -1){	
					printf("Error line %d\n", cont+1);
					break;
				}
			}
		}
		cont++; // AUMENTA O CONTADOR DE JSONS
	if(feof(stdin)) // NO FINAL DE ARQUIVO, SAI DO FOR
			break;
	}
	
	printf("Number of Objects: %d\n", p->nObjects);
	printf("Number of Arrays: %d\n", p->nArrays);
	printf("Number of Pairs: %d\n", p->nPairs);
	printf("Number of Strings: %d\n", p->nStrings);
	printf("Number of Numbers: %d\n", p->nNumbers);
	printf("Number of Trues: %d\n", p->nTrues);
	printf("Number of Falses: %d\n", p->nFalses);
	printf("Number of Nulls: %d\n", p->nNulls);
	
	free(p);
	
	return 0;
}

