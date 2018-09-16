/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  TRABALHO 1 DE ICC 2 - 2017 - 2 SEMESTRE 
  UNIVERSIDADE DE SAO PAULO(USP) - CAMPUS SAO CARLOS

  NOME DO ALUNO: JOAO VICTOR GARCIA COELHO
PROFESSOR: RODRIGO MELLO

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/




#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define IS_HERE 1 // USADO PARA INDICAR QUE O OBJETO ESTA NO OCTANTE E/OU NA CAIXA DELIMITADORA
#define IS_NOT_HERE 0// USADO PARA INDICAR QUE O OBJETO NAO ESTA NO OCTANTE E/OU NA CAIXA DELIMITADORA

typedef struct _ponto{

	double x,y,z; // COORDENADAS DOŚ VÉRTICES

}Ponto;

typedef struct _triangulo{

	Ponto v1,v2,v3; // OS 3 VERTICES DO TRIANGULO

}Triangulo;

typedef struct _caixa{

	Ponto vertices[8]; // VETOR DE COORDENADAS DA CAIXA DELIMITADORA
	Ponto Centro; // CENTRO DA CAIXA
	int Level; // NIVEL MÁXIMO DA OCTREE

}Caixa;


Triangulo* CreateObjALVO(int quant); // FUNÇÃO PARA CRIAR O OBJETO ALVO A PARTIR DA QUANTIDADE DE TRIANGULOS QUE ESTE POSSUI
Caixa* CreateCaixaDemilitadora(); // FUNCAO PARA CRIAR A CAIXA DELIMITADORA
Ponto* CreateObjColisor(); // FUNCAO PARA CRIAR O OBJETO COLISOR

void DestroiCaixaDelimitadora(Caixa* c); // FUNCAO PARA DESALOCAR A CAIXA DELIMITADORA
void DestroiObjAlvo(Triangulo* t); // FUNCAO PARA DESALOCAR O OBJETO ALVO
void DestroiObjColisor(Ponto* p); // FUNCAO PARA DESALOCAR O OBJETO COLISOR




// FUNCAO PARA CALCULAR A CAIXA DELIMITADOR CASO ELA NÃO SEJA DADA
void CalculaCaixaDelimitadora(Caixa* c, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax); 

// FUNCAO PARA VERIFICAR SE O OBJETO COLISOR ESTA DENTRO DA CAIXA DELIMITADORA
int verificaPonto(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, Ponto* objColisor); 

// FUNCAO RECURSIVA QUE VERIFICARÁ E CHAMARÁ O OCTANTE EM QUE O OBJETO COLISOR ESTÁ ATÉ O NIVEL MÁXIMO DETERMINADO PELO USUARIO
int CalculaOctante(Caixa* c, Ponto* ObjColisor, Triangulo* ObjAlvo, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, int levels, int quant);

// FUNCAO PARA VERIFICAR SE ALGUM VERTICE DE ALGUM TRIANGULO DO OBJETO ALVO ESTÁ DENTRO DO OCTANTE, E CASO HAJA, HOUVE A COLISAO
int VerificaVertices(Triangulo* t, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax, int quant);

// FUNCAO PARA VERIFICAR SE ALGUM PONTO MEDIO DE ALGUMA ARESTA DE ALGUM TRIANGULO DO OBJETO ALVO ESTA DENTRO DO OCTANTE, E CASO HAJA, HOUVE COLISAO
int VerificaPontoMedio(Triangulo* t, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax, int quant);






int main(){	
	// DECLARAÇÃO DE VARIAVEIS:
	// FLAG SERA USADA PARA LER O PRIMEIRO NUMERO QUE INDICA SE A CAIXA DELIMITADOR DEVE OU NAO SER CALCULADA
	// quant INDICA O NUMERO DE TRIANGULOS QUE O OBJETO ALVO POSSUI
	// xmin, ymin, zmin, xmax, ymax e zmax SAO VARIAVES QUE SERAO CALCULADAS DE ACORDO COM OS VALORES MINIMOS E MAXIMOS DAS COORDENADAS DOS TRIANGULOS
	// NESSE PROGRAMA ELAS SERAO UTILIZADAS PARA ANDAR PELOS OCTANTES E TAMBEM PARA CALCULAR A CAIXA DELIMITADOR CASO FLAG=1

	int FLAG, quant,i;
	double xmin, ymin, zmin, xmax, ymax, zmax;
	
	// LENDO E ALOCANDO OS OBJETOS 
	
	scanf("%d", &FLAG);
	
	Caixa *CaixaInicial = CreateCaixaDemilitadora(); 
	
	scanf("%d", &CaixaInicial->Level); // LENDO O NÍVEL MÁXIMO DA OCTREE
	scanf("%d", &quant);
	
	Ponto *ObjColisor = CreateObjColisor();
	Triangulo *ObjAlvo = CreateObjALVO(quant);
	
	// ATRIBUINDO VALORES INICIAIS PARA AS VARIAVES DE MINIMO E MAXIMO
	xmin = ObjAlvo[0].v1.x;
	ymin = ObjAlvo[0].v1.y;
	zmin = ObjAlvo[0].v1.z;
	xmax = xmin;
	ymax = ymin;
	zmax = zmin; 
	
	
	//CALCULA O XMIN	
	for(i = 0 ; i < quant; i++){
		if(ObjAlvo[i].v1.x < xmin)
			xmin = ObjAlvo[i].v1.x;
		if(ObjAlvo[i].v2.x < xmin)
			xmin = ObjAlvo[i].v2.x;
		if(ObjAlvo[i].v3.x < xmin)
			xmin = ObjAlvo[i].v3.x;
	}
	//CALCULA XMAX
	for(i = 0 ; i < quant; i++){
		if(ObjAlvo[i].v1.x > xmax)
			xmax = ObjAlvo[i].v1.x;
		if(ObjAlvo[i].v2.x > xmax)
			xmax = ObjAlvo[i].v2.x;
		if(ObjAlvo[i].v3.x > xmax)
			xmax = ObjAlvo[i].v3.x;
	}

	//CALCULA O YMIN	
	for(i = 0 ; i < quant; i++){
		if(ObjAlvo[i].v1.y < ymin)
			ymin = ObjAlvo[i].v1.y;
		if(ObjAlvo[i].v2.y < ymin)
			ymin = ObjAlvo[i].v2.y;
		if(ObjAlvo[i].v3.y < ymin)
			ymin = ObjAlvo[i].v3.y;
	}

	//CALCULA YMAX
	for(i = 0 ; i < quant; i++){
		if(ObjAlvo[i].v1.y > ymax)
			ymax = ObjAlvo[i].v1.y;
		if(ObjAlvo[i].v2.y > ymax)
			ymax = ObjAlvo[i].v2.y;
		if(ObjAlvo[i].v3.y > ymax)
			ymax = ObjAlvo[i].v3.y;
	}

	//CALCULA O ZMIN	
	for(i = 0 ; i < quant; i++){
		if(ObjAlvo[i].v1.z < zmin)
			zmin = ObjAlvo[i].v1.z;
		if(ObjAlvo[i].v2.z < zmin)
			zmin = ObjAlvo[i].v2.z ;
		if(ObjAlvo[i].v3.z < zmin)
			zmin = ObjAlvo[i].v3.z;
	}

	//CALCULA O ZMAX
	for(i = 0 ; i < quant; i++){
		if(ObjAlvo[i].v1.z > zmax)
			zmax = ObjAlvo[i].v1.z;
		if(ObjAlvo[i].v2.z > zmax)
			zmax = ObjAlvo[i].v2.z;
		if(ObjAlvo[i].v3.z > zmax)
			zmax = ObjAlvo[i].v3.z;
	}	

	// SE A FLAG = 0, AS COORDENADAS DA CAIXA SAO LIDAS, CASO CONTRARIO ELAS SAO CALCULADAS DE ACORDO COM OS VALORES DE MINIMO E MAXIMO JA CALCULADOS

	
	if(!FLAG){
		for(i=0;i<8;i++){
			scanf("%lf", &CaixaInicial->vertices[i].x);
			scanf("%lf", &CaixaInicial->vertices[i].y);
			scanf("%lf", &CaixaInicial->vertices[i].z);
		}
	}else{
		CalculaCaixaDelimitadora(CaixaInicial, xmin, ymin, zmin, xmax, ymax, zmax);
	}
	

	int result = verificaPonto(xmin,xmax,ymin,ymax,zmin,zmax, ObjColisor); // VERIFICA SE O PONTO ESTÁ NA CAIXA INICIAL
	
	// SENÃO ESTIVER, JÁ  DESTROI A CAIXA, E OS OBJETOS, E ENCERRA O PROGRAMA COM SAIDA INDICANDO QUE NAO HOUVE COLISAO
	if(result == IS_NOT_HERE){
		DestroiCaixaDelimitadora(CaixaInicial); 

		DestroiObjAlvo(ObjAlvo);
		DestroiObjColisor(ObjColisor);
		
		printf("0");
		return 0;

	}

	// SE CHEGAR AQUI, QUER DIZER QUE O OBJETO COLISOR ESTA NA CAIXA

	// ASSIM, CHAMA A FUNCAO RECURSIVA PARA ACHAR O OCTANTE NO QUAL O PONTO ESTA INSERIDO
	// OS ARGUMENTOS DA FUNCAO SAO A CAIXA, OS DOIS OBJETOS E OS VALORES DE MINIMO E MAXIMO
	// 0 EH PARA INDICAR QUE O NIVEL INICIAL DA CAIXA DELIMITADORA EH ZERO

	result = CalculaOctante(CaixaInicial, ObjColisor, ObjAlvo, xmin, xmax, ymin, ymax, zmin, zmax, 0, quant);
	
	// DE ACORDO COM O RESULTADO DA FUNCAO, ENTÃO HOUVE OU NAO COLISAO
	if(result == IS_HERE)
		printf("1");
	else
		printf("0");

	// LIBERANDO AS MEMORIAS ALOCADAS
	DestroiCaixaDelimitadora(CaixaInicial); 
	DestroiObjAlvo(ObjAlvo);
	
	DestroiObjColisor(ObjColisor);


	return 0;

}


////////////////////////////////////////////////////FUNCOES/////////////////////////////////////////////////////////////////////////




Triangulo* CreateObjALVO(int quant){
	Triangulo *t = (Triangulo*) malloc(sizeof(Triangulo)*quant); // ALOCANDO UM VETOR DE TRIANGULOS
	int i;

	// LENDO OS 3 VÉRTICES DE CADA TRIANGULO
	for(i=0;i<quant;i++){
		scanf("%lf", &t[i].v1.x);
		scanf("%lf", &t[i].v1.y);
		scanf("%lf", &t[i].v1.z);

		scanf("%lf", &t[i].v2.x);
		scanf("%lf", &t[i].v2.y);
		scanf("%lf", &t[i].v2.z);

		scanf("%lf", &t[i].v3.x);
		scanf("%lf", &t[i].v3.y);
		scanf("%lf", &t[i].v3.z);
	}

	return t;
}


Caixa* CreateCaixaDemilitadora(){
	Caixa* c = (Caixa*) malloc(sizeof(Caixa)); // ALOCANDO A CAIXA DELIMITADORA DO OBJETO ALVO
	
	return c;
}

Ponto* CreateObjColisor(){
	// ALOCANDO O OBJETO COLISOR E LENDO SUAS COORDENADAS
	Ponto *p = (Ponto*) malloc(sizeof(Ponto));
	scanf("%lf", &p->x);
	scanf("%lf", &p->y);
	scanf("%lf", &p->z);

	return p;
}

// LIBERANDO AS MEMÓRIAS ALOCADAS PARA A CAIXA, E OS DOIS OBJETOS(ALVO E COLISOR)
void DestroiCaixaDelimitadora(Caixa* c){
	free(c);
}

void DestroiObjAlvo(Triangulo* t){
	free(t);
}

void DestroiObjColisor(Ponto* p){
	free(p);
}


void CalculaCaixaDelimitadora(Caixa* c, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax){	
	//FAZENDO A PERMUTACAO PARA DETERMINAR CADA VERTICE DA CAIXA DELIMITADORA

	c->vertices[0].x = xmin;
	c->vertices[0].y = ymin;
	c->vertices[0].z = zmin;

	c->vertices[1].x = xmin;
	c->vertices[1].y = ymin;
	c->vertices[1].z = zmax;

	c->vertices[2].x = xmin;
	c->vertices[2].y = ymax;
	c->vertices[2].x = zmin;

	c->vertices[3].x = xmin;
	c->vertices[3].y = ymax;
	c->vertices[3].z = zmax;

	c->vertices[4].x = xmax;
	c->vertices[4].y = ymin;
	c->vertices[4].z = zmin;

	c->vertices[5].x = xmax;
	c->vertices[5].y = ymin;
	c->vertices[5].z = zmax;

	c->vertices[6].x = xmax;
	c->vertices[6].y = ymax;
	c->vertices[6].z = zmin;

	c->vertices[7].x = xmax;
	c->vertices[7].y = ymax;
	c->vertices[7].z = zmax;
	
}

int verificaPonto(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, Ponto* objColisor) {
	//SE CADA COORDENADA(x,y,z) DO OBJETO COLISOR ESTIVER ENTRE AS COORDENADAS DE MENOR E MAIOR VALOR DA CAIXA DELIMITADORA, ENTÃO
	
	//O O OBJETO COLISOR ESTA NA CAIXA
	
	// POR EXEMPLO SE A COORDENADA x DO OBJETO COLISOR ESTIVER ENTRE XMIN E XMAX(QUE SÃO AS MENORES E MAIOR COORDENADAS DE X NA CAIXA), ENTÃO 
	
	// O OBJETO COLISOR ESTA NA CAIXA

	if((objColisor->x >= xmin && objColisor->x <= xmax) && (objColisor->y >= ymin && objColisor->y <= ymax) && (objColisor->z >= zmin && objColisor->z <= zmax))
		return IS_HERE;
	else 
		return IS_NOT_HERE;

}

int CalculaOctante(Caixa* c, Ponto* ObjColisor, Triangulo* ObjAlvo, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, int levels, int quant)
{	
	//CALCULA AS COORDENADAS DO PONTO CENTRAL DE CADA OCTANTE ACESSADO

	c->Centro.x = (xmin + xmax)/2.0;
	c->Centro.y = (ymin + ymax)/2.0;
	c->Centro.z = (zmin + zmax)/2.0;

	// CONDICAO DE PARADA DA FUNCAO RECURSIVA: SE O LEVEL EM QUE O OCTANTE ESTA É O MAXIMO DADO PELO USUARIO
	
	if(levels == c->Level){
		// VERIFICA SE HÁ ALGUM VERTICE DO OBJETO ALVO DENTRO DO OCTANTE
		int result = VerificaVertices(ObjAlvo, xmin, ymin, zmin, xmax, ymax, zmax, quant); 
		
		if(result == IS_NOT_HERE){ // SE NÃO HOUVER
			// VERIFICA SE HA ALGUM PONTO DE ALGUMA ARESTA DO OBJETO ALVO DENTRO DO OCTANTE
			return VerificaPontoMedio(ObjAlvo, xmin, ymin, zmin, xmax, ymax, zmax, quant);		
		}	
		return result;
	}
	
	// CHAMA O PROXIMO OCTANTE ONDE ESTÁ O OBJETO COLISOR DE ACORDO COM A RELAÇÃO 
	// ENTRE AS SUAS COORDENADAS E AS COORDENADAS DO CENTRO DO OCTANTE ATUAL 

	// A IDEIA EH ATUALIZAR SEMPRE O VALOR DE MIN E MAX DE CADA COORDENADA DE ACORDO COM A POSICAO DO OBJETO EM RELACAO AO CENTRO

	if((ObjColisor->x <= c->Centro.x) && (ObjColisor->y <= c->Centro.y) && (ObjColisor->z <= c->Centro.z))
		return	CalculaOctante(c, ObjColisor, ObjAlvo, xmin, c->Centro.x, ymin, c->Centro.y, zmin, c->Centro.z, levels+1, quant); 
													  //xmin  xmax        ymin   ymax        zmin  zmax	
	
	else if((ObjColisor->x <= c->Centro.x) && (ObjColisor->y <= c->Centro.y) && (ObjColisor->z >= c->Centro.z))
		return	CalculaOctante(c, ObjColisor, ObjAlvo, xmin, c->Centro.x, ymin, c->Centro.y, c->Centro.z, zmax, levels+1, quant); 
													// xmin  xmax          ymin ymax         zmin         zmax
	
	else if((ObjColisor->x <= c->Centro.x) && (ObjColisor->y >= c->Centro.y) && (ObjColisor->z <= c->Centro.z))
		return	CalculaOctante(c, ObjColisor, ObjAlvo, xmin, c->Centro.x, c->Centro.y, ymax, zmin, c->Centro.z, levels+1, quant); 
													// xmin     xmax       ymin        ymax  zmin     zmax
	
	else if((ObjColisor->x <= c->Centro.x) && (ObjColisor->y >= c->Centro.y) && (ObjColisor->z >= c->Centro.z))
		return	CalculaOctante(c, ObjColisor, ObjAlvo, xmin, c->Centro.x, c->Centro.y, ymax, c->Centro.z, zmax, levels+1, quant); 
													// xmin     xmax       ymin        ymax  zmin     zmax
	
	else if((ObjColisor->x >= c->Centro.x) && (ObjColisor->y <= c->Centro.y) && (ObjColisor->z <= c->Centro.z))
		return	CalculaOctante(c, ObjColisor, ObjAlvo, c->Centro.x, xmax, ymin, c->Centro.y, zmin, c->Centro.z, levels+1, quant); 
														// xmin     xmax  ymin     ymax       zmin     zmax
	
	else if((ObjColisor->x >= c->Centro.x) && (ObjColisor->y <= c->Centro.y) && (ObjColisor->z >= c->Centro.z))
		return	CalculaOctante(c, ObjColisor, ObjAlvo, c->Centro.x, xmax, ymin, c->Centro.y,c->Centro.z, zmax, levels+1, quant); 
														// xmin     xmax  ymin     ymax       zmin       zmax
	
	else if((ObjColisor->x >= c->Centro.x) && (ObjColisor->y >= c->Centro.y) && (ObjColisor->z <= c->Centro.z))
		return	CalculaOctante(c, ObjColisor, ObjAlvo, c->Centro.x, xmax,c->Centro.y, ymax,  zmin, c->Centro.z, levels+1, quant); 
														// xmin     xmax     ymin     ymax   zmin     zmax
	
	else 
		return	CalculaOctante(c, ObjColisor,ObjAlvo, c->Centro.x, xmax, c->Centro.y, ymax, c->Centro.z, zmax, levels+1, quant); 
														// xmin    xmax      ymin     ymax       zmin    zmax

}


int VerificaVertices(Triangulo* t, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax, int quant){
	int i;

	// VERIFICA PARA CADA VERTICE DE CADA TRIANGULO DO OBJETO ALVO SE ESSE DETERMINADO VERTICE ESTA NO OCTANTE
	// CASO ESTEJA, RETORNA QUE HOUVE A INTERSECCAO ENTRE OS OBJETOS
	// CASO NÃO HAJA NENHUM VERTICE DENTRO DO OCTANTE, RETORNA QUE NÃO HOUVE A INTERSECCACO ENTRE OS OBJETOS 


	for(i = 0; i < quant; i++){
		if((t[i].v1.x >= xmin && t[i].v1.x <= xmax) && (t[i].v1.y >= ymin && t[i].v1.y <=ymax) && (t[i].v1.z >= zmin && t[i].v1.z <= zmax))
			return IS_HERE;
	}	

	for(i = 0; i < quant; i++){
		if((t[i].v2.x >= xmin && t[i].v2.x <= xmax) && (t[i].v2.y >= ymin && t[i].v2.y <=ymax) && (t[i].v2.z >= zmin && t[i].v2.z <= zmax))
			return IS_HERE;
	}	

	for(i = 0; i < quant; i++){
		if((t[i].v3.x >= xmin && t[i].v3.x <= xmax) && (t[i].v3.y >= ymin && t[i].v3.y <=ymax) && (t[i].v3.z >= zmin && t[i].v3.z <= zmax))
			return IS_HERE;
	}	
	
	return IS_NOT_HERE;

}

int VerificaPontoMedio(Triangulo* t, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax, int quant){
	int i;
	// ALOCANDO UM PONTO QUE SERA USADO PARA CALCULAR E VERIFICAR SE O PONTO MEDIO DE PELO MENOS UMA ARESTA DE ALGUM TRIANGUL ESTÁ NO OCTANTE
	Ponto* Medio = (Ponto*) malloc(sizeof(Ponto));
	// ARESTA ENTRE E OS VÉRTICES 1 E 2
	for(i = 0; i < quant; i++){
		// PARA CADA TRIANGULO, CALCULA AS COORDENADAS DO PONTO MEDIO E VERIFICA SE ESTE ESTA NO OCTANTE
		Medio->x = (t[i].v1.x +t[i].v2.x)/2.0;
		Medio->y = (t[i].v1.y +t[i].v2.y)/2.0;
		Medio->z = (t[i].v1.z +t[i].v2.z)/2.0;
		if((Medio->x >= xmin && Medio->x<= xmax) && (Medio->y >= ymin && Medio->y <= ymax) && (Medio->z >= zmin && Medio->z <= ymax)){	
			// SE ESTIVER, LIBERA A MEMORIA ALOCADA E RETORNA VALOR POSITIVO, INDICANDO QUE HOUVE COLISAO
			return IS_HERE;
		}	
	}	

	// ARESTA ENTRE E OS VÉRTICES 1 E 3
	for(i = 0; i < quant; i++){
		// PARA CADA TRIANGULO, CALCULA AS COORDENADAS DO PONTO MEDIO E VERIFICA SE ESTE ESTA NO OCTANTE
		Medio->x = (t[i].v1.x +t[i].v3.x)/2.0;
		Medio->y = (t[i].v1.y +t[i].v3.y)/2.0;
		Medio->z = (t[i].v1.z +t[i].v3.z)/2.0;
		if((Medio->x >= xmin && Medio->x<= xmax) && (Medio->y >= ymin && Medio->y <=ymax) && (Medio->z >= zmin && Medio->z <= ymax)){	
			// SE ESTIVER, LIBERA A MEMORIA ALOCADA E RETORNA VALOR POSITIVO, INDICANDO QUE HOUVE COLISAO
			return IS_HERE;
		}	
	}

	// ARESTA ENTRE E OS VÉRTICES 2 E 3
	for(i = 0; i < quant; i++){
		// PARA CADA TRIANGULO, CALCULA AS COORDENADAS DO PONTO MEDIO E VERIFICA SE ESTE ESTA NO OCTANTE
		Medio->x = (t[i].v2.x +t[i].v3.x)/2.0;
		Medio->y = (t[i].v2.y +t[i].v3.y)/2.0;
		Medio->z = (t[i].v2.z +t[i].v3.z)/2.0;
		if((Medio->x >= xmin && Medio->x<= xmax) && (Medio->y >= ymin && Medio->y <=ymax) && (Medio->z >= zmin && Medio->z <= ymax)){	
			// SE ESTIVER, LIBERA A MEMORIA ALOCADA E RETORNA VALOR POSITIVO, INDICANDO QUE HOUVE COLISÃO
			return IS_HERE;
		}
	}

	// SE NENHUM PONTO MEDIO ESTIVER NOS OCTANTES, ENTÃO QUER DIZER QUE NÃO HOUVE COLISAO
	// ASSIM, LIBERA-SE A MEMORIA E RETORNA VALOR NEGATIVO, INDICANDO QUE NAO HOUVE COLISAO
	free(Medio);
	
	return IS_NOT_HERE;

}