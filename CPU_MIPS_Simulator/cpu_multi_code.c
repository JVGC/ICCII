#include <stdio.h>
#include <stdlib.h>

#define NOT ! 
#define AND && 
#define OR || 

typedef struct state{

	char s0;
	char s1;
	char s2;
	char s3;

}State;


typedef struct uc{

	char RegDst0;
	char RegDst1;
	char RegWrite;
	char ALUSrcA;
	char ALUSrcB0;
	char ALUSrcB1;
	char ALUOp0;
	char ALUOp1;
	char PCSource0;
	char PCSource1;
	char PCWriteCond;
	char PCWrite;
	char IorD;
	char MemRead;
	char MemWrite;
	char BNE;
	char IRWrite;
	char MemtoReg0;
	char MemtoReg1;

	State estado_atual;
	State proximo_estado;


}UC;


typedef struct ula{
	int param1;
	int param2;

	int op;
	int result;
	int zero;
}ULA;

int decifra_operacao(int inst);

int decifra_instrucao(unsigned inst);

void seta_sinal_controle(UC* u, int cod_inst);

void Imprime_Sinais(UC Control_Unit);

void Imprime_registradores(int* Registradores);

void Imprime_RAM(unsigned int* RAM);


int main()
{

	int i;
	
	// Lendo e abrindo o Arquivo
	char nome_arq[15];
	printf("Digite o nome do arquivo: ");
	scanf("%s", nome_arq);
	printf("\n");

	FILE* arq = fopen(nome_arq, "rb");
	if(arq == NULL){
		printf("Erro ao abrir o arquivo.\n");
		return 0;
	}

	// Professor falou que a 256 bytes para a RAM eram suficientes, então 256/4 words
	unsigned int* RAM = (unsigned int*) malloc(sizeof(unsigned int)* (256/4));
	// Lendo os números como unsigned
	for(i = 0; !feof(arq); i++){
		fscanf(arq,"%u", &RAM[i]);
	}

	fclose(arq);

	// Vetor para o Banco de Registradores. Zerando todos eles. 
	int Registradores[32];

	for(i = 0; i < 32; i++)
		Registradores[i] = 0;

	UC Control_Unit;

	// Estado Inicial da Unidade de Controle é sempre 0
	Control_Unit.estado_atual.s0 = 0;
	Control_Unit.estado_atual.s1 = 0;
	Control_Unit.estado_atual.s2 = 0;
	Control_Unit.estado_atual.s3 = 0;
	

	ULA Arithmetic_Unit;

	int PC = 0;
	int cod_inst = 0;
	int IR;
	int MBR, MAR;
	int A, B;
	short int immediate; // immediate tem 16 bits
	int ALUout;
	int ENDjump; // endereço da JUMP
	int PCaux; // variavel com os 4 bits mais significativos de PC
	int write_register;
	int write_data;
	int BNE; // variavel que sai do MUX controlado pelo BNE (recebe zero ou NOT(zero))

	// Execução do programa
	while(1){
		seta_sinal_controle(&Control_Unit, cod_inst);

		// MUX DO PC (DIVIDIDO em duas parte: Caso IorD for igual a 0). Caso for igual a 1, está no final do while
		if(Control_Unit.IorD == 0){ // Caso for igual a 0, a saida do mux recebe PC
			if(Control_Unit.MemRead == 1){ // Verifico se é possivel ler a memória
				// li a memoria 
				MAR = PC;
				if((MAR < 0) || (MAR > 256)){
					printf("Status da Saída: Término devido a acesso inválido de memória.\n");
					break;
				}

				MBR = RAM[MAR];
				if(Control_Unit.IRWrite == 1){ // Verifico se posso escrever no registrador de instrução 	
					IR = MBR;
				}
			}
		}

		// ULA
	
		//Decifro qual a operação será feita na ULA no estado atual
		if(Control_Unit.ALUOp0 == 0 && Control_Unit.ALUOp1 == 0){ // operacao de soma
			Arithmetic_Unit.op = 0;
		}else if(Control_Unit.ALUOp0 == 1 && Control_Unit.ALUOp1 == 0){ // operacao de subtracao
			Arithmetic_Unit.op = 1;
		}else if(Control_Unit.ALUOp0 == 1 && Control_Unit.ALUOp1 == 1){ // operacao de AND
			Arithmetic_Unit.op = 3;
		}else{ // chama a unidade de controle da ULA para decifrar a operacao
			Arithmetic_Unit.op = decifra_operacao(IR);
		}

		// Se OP == -1, operação não existe. FIM DO PROGRAMA
		if(Arithmetic_Unit.op == -1){
			printf("Status da Saída: Término devido à operação inválida da ULA.\n");
			break;
		}

		// MUX DO PARAMETRO A DA ULA
		if(Control_Unit.ALUSrcA == 0){ // Quando for somar PC com 4
			 Arithmetic_Unit.param1 = PC;
		}else{ 
			if((A < 0) || (A > 31)){
					printf("Status da Saída: Término devido a acesso inválido ao Banco de Registradores.\n");
					break;
			}

			Arithmetic_Unit.param1 =  Registradores[A];
		}
		

		// MUX DO PARAMETRO B DA ULA
		if(Control_Unit.ALUSrcB0 == 0 && Control_Unit.ALUSrcB1 == 0){ //parametro 2 da ULA recebe registrador B
			if((B < 0) || (B > 31)){
					printf("Status da Saída: Término devido a acesso inválido ao Banco de Registradores.\n");
					break;
			}

			Arithmetic_Unit.param2 =  Registradores[B];
		}else if(Control_Unit.ALUSrcB0 == 1 && Control_Unit.ALUSrcB1 == 0){ //parametro 2 da ula recebe 4
			Arithmetic_Unit.param2 = 4/4; // divido por 4 (tamanho de uma palavra)
		}else if(Control_Unit.ALUSrcB0 == 0 && Control_Unit.ALUSrcB1 == 1){ //parametro 2 da ula recebe valor imediato da instrucao
			// Ando com o  valor imediato da isntrução 16 bits para a esquerda (extensão de sinal)
			// faço o casting para inteiro(transformo short int que tem 16 bits em um int que tem 32)
			// e retorno o valor 16 bits para trás, para manter-se o valor correto
			// depois, shifto dois bits para a esquerda
			Arithmetic_Unit.param2  = (int)((immediate) << 16) >> 16; 
		}else{ // parametro 2 da ULA recebe valor imediato shiftado 2 bits
			
			// Ando com o  valor imediato da isntrução 16 bits para a esquerda (extensão de sinal)
			// faço o casting para inteiro(transformo short int que tem 16 bits em um int que tem 32)
			// e retorno o valor 16 bits para trás, para manter-se o valor correto
			// depois, shifto dois bits para a esquerda
			// divido por 4 para manter o valor orientado a palavra
			Arithmetic_Unit.param2  = ((int)(((immediate) << 16) >> 16) << 2) / 4; 
		}


		// Resolve a Operação na ULA
		if(Arithmetic_Unit.op == 0){ // Soma
			Arithmetic_Unit.result = Arithmetic_Unit.param1 + Arithmetic_Unit.param2;
		}else if(Arithmetic_Unit.op == 1){ // Subtração
			Arithmetic_Unit.result = Arithmetic_Unit.param1 - Arithmetic_Unit.param2;
			if(Arithmetic_Unit.result == 0){ // Se os dois parametros forem iguais
				Arithmetic_Unit.zero = 1;
			}else{ // se forem diferentes
				Arithmetic_Unit.zero = 0;
			}
		}else if(Arithmetic_Unit.op == 3){ // And
			Arithmetic_Unit.result = Arithmetic_Unit.param1 & Arithmetic_Unit.param2;
		}else if(Arithmetic_Unit.op == 4){ // Or
			Arithmetic_Unit.result = Arithmetic_Unit.param1 | Arithmetic_Unit.param2;
		}else if(Arithmetic_Unit.op == 5){ // Set less than
			Arithmetic_Unit.result = Arithmetic_Unit.param1 - Arithmetic_Unit.param2;
			if(Arithmetic_Unit.result < 0){ // Se o param1 for menor que o param2
				Arithmetic_Unit.zero = 0; // não é igual
				Arithmetic_Unit.result = 1; // resultado do SLT
			}else if (Arithmetic_Unit.result > 0){ // se param1 maior que param2
				Arithmetic_Unit.zero = 0;
			}else{ // se os dois forem igual, zero = 1
				Arithmetic_Unit.zero = 1;
			}
		}

		// MUX DO REGDst

		if(Control_Unit.RegDst0 == 0 && Control_Unit.RegDst1 == 0){ // write register recebe os bits de 20 a 16 da instrucao 
			write_register = B;
		}else if(Control_Unit.RegDst0 == 1 && Control_Unit.RegDst1 == 0){ // write register recebe os bits de 15 a 11 da instrucao (TIPO-R)
			write_register = (IR >> 11) & 31; // 31 = 11111
		}else if(Control_Unit.RegDst0 == 0 && Control_Unit.RegDst1 == 1){ // write register recebe $ra(31) (Jal ou Jalr)
			write_register = 31; // $ra = 31
		}


		// MUX DO MemtoReg
		if(Control_Unit.MemtoReg0 == 0 && Control_Unit.MemtoReg1 == 0){ // write data ALUout (Instruções do TIPO-R)
			write_data = ALUout;
		}else if(Control_Unit.MemtoReg0 == 1 && Control_Unit.MemtoReg1 == 0){ // write data recebe MBR(vai ser usada em casa de LW)
			write_data = MBR;
		}else if(Control_Unit.MemtoReg0 == 0 && Control_Unit.MemtoReg1 == 1){ // write data recebe PC (em jal ou jalr)
			write_data = PC * 4; // Multiplico por 4 para guardar o valor orientado a byte e não a palavra como está o PC
		}

		if(Control_Unit.RegWrite == 1){ // Se for permitido escrever no Banco de Registradores
				if((write_register < 0) || (write_register > 31)){
					printf("Status da Saída: Término devido a acesso inválido ao Banco de Registradores.\n");
					break;
				}

				Registradores[write_register] = write_data;
		}

		// MUX DO BNE

		if(Control_Unit.BNE == 0){ // Se nao for uma BNE
			//BNE recebe sinal zero da ULA
			BNE = Arithmetic_Unit.zero;
		}else{
			//BNE recebe NOT(sinal zero) da ULA
			BNE = NOT(Arithmetic_Unit.zero);
			
		}

		// MUX DO PC SOURCE

		if(Control_Unit.PCSource0 == 0 && Control_Unit.PCSource1 == 0){ // PC RECEBE ALU RESULT
			
			// Acontece no 1ºciclo de toda instrução quando somo PC com 4
			if(Control_Unit.PCWrite == 1){ // Se PCWrite for igual a 1, é permitido escrever em PC
				PC = Arithmetic_Unit.result;
			}else{ // Senão, só escrevo em PC se (BNE & PCWriteCond) ==1
				if((BNE & Control_Unit.PCWriteCond) == 1){
					PC = Arithmetic_Unit.result;
				}
			}
		}else if(Control_Unit.PCSource0 == 1 && Control_Unit.PCSource1 == 0){// PC RECEBE ALUOut
			
			// Caso de ĩnstruções do tipo Branch (BEQ e BNE)
			if(Control_Unit.PCWrite == 1){ // Se PCWrite for igual a 1, é permitido escrever em PC
				PC = ALUout;
			}else{ // Senão, só escrevo em PC se (BNE & PCWriteCond) ==1
				if((BNE & Control_Unit.PCWriteCond) == 1){
					PC = ALUout;
				}
			}
		}else if(Control_Unit.PCSource0 == 0 && Control_Unit.PCSource1 == 1){ //PC recebe endereço da jump
			//Caso de Jump, Jump and Link, e Jump And Link Register
			
			if(Control_Unit.PCWrite == 1){ // Se PCWrite for igual a 1, é permitido escrever em PC
				PC = ENDjump / 4; // Divido por 4 pois o valor da Jump está orientado a byte e não a palavra 
			}else{ // Senão, só escrevo em PC se (BNE & PCWriteCond) ==1
				if((BNE & Control_Unit.PCWriteCond) == 1){
					PC = ENDjump / 4; // Divido por 4 pois o valor da Jump está orientado a byte e não a palavra
				}
			}
		}else{ // PC RECEBE O REGISTRADOR A (Caso de JUMP Register)

			
			if(Control_Unit.PCWrite == 1){  // Se PCWrite for igual a 1, é permitido escrever em PC
				if((A < 0) || (A > 31)){
					printf("Status da Saída: Término devido a acesso inválido ao Banco de Registradores.\n");
					break;
				}
				
				PC = Registradores[A] / 4; // Divido por 4 pois o valor guardado está a byte e não a palavra

			}else{ // Senão, só escrevo em PC se (BNE & PCWriteCond) ==1
				if((BNE & Control_Unit.PCWriteCond) == 1){
					if((A < 0) || (A > 31)){
						printf("Status da Saída: Término devido a acesso inválido ao Banco de Registradores.\n");
						break;
					}

					PC = Registradores[A];
				}
			}

		}


		// Segunda parte do mux do IorD: Se for igual a 1(SW e LW)
		if(Control_Unit.IorD == 1){
			if(Control_Unit.MemRead == 1){ // Se for permitido ler (Caso de LW)
				MAR = ALUout;
				if((MAR < 0) || (MAR > 256)){
					printf("Status da Saída: Término devido a acesso inválido de memória.\n");
					break;
				}

				MBR = RAM[MAR/4]; // Aluout contém um valor a byte, divido por 4 pois a memória está a palavra
			}else if(Control_Unit.MemWrite == 1){ // Se for permitido escrever (Caso de SW)
				MAR = ALUout;
				if((B< 0) || (B > 31)){
					printf("Status da Saída: Término devido a acesso inválido ao Banco de Registradores.\n");
					break;
				}else if((MAR < 0) || (MAR > 256)){
					printf("Status da Saída: Término devido a acesso inválido de memória.\n");
					break;
				}
				RAM[MAR/4] = Registradores[B]; // Aluout contém um valor a byte, divido por 4 pois a memória está a palavra
			}
		}	

		// Mudo de estado
		Control_Unit.estado_atual = Control_Unit.proximo_estado;

		//Atualizo o ALUout
		ALUout = Arithmetic_Unit.result;
		
		// Decifro o codigo de Instrução
		cod_inst = decifra_instrucao(IR);
		
		// Se ela for inválida
		if(cod_inst == -1){
			// seto os sinais de controle do novo estado
			seta_sinal_controle(&Control_Unit, cod_inst);
			printf("Status da Saída: Término devido à tentativa de execução de instrução inválida.\n");
			break;			
		}

		// Bits de 25 a 21 da instrução
		A = IR >> 21;
		A = (A & 31); // 31 = 11111
		
		// Bits de 20 a 16 da instrução
		B = IR >> 16;
		B = (B & 31); // 31 = 11111
		
		// Limpo os 16 bits mais significativos da instrução, e retorno para o valor original
		immediate = ((IR) << 16) >> 16;

		// Calculo o valor da JUMP
		PCaux = (PC >> 27) << 27; // Bits mais significativos de PC
		// Limpo os 6 bits mais significativos da instrução. Shifto 2 bits a esquerda
		// Faco um OR com os 4 bits mais significativos de PC (concatenacao)
		ENDjump = (((IR << 6) >> 6) << 2) | PCaux;

	}
	//Impressão da Saída
	Imprime_Sinais(Control_Unit);
	
	printf("PC = %d    IR = %u      MDR = %u\n", PC*4, IR, MBR);
	printf("A =  %d    B = %d       AluOut = %d\n", Registradores[A], Registradores[B], ALUout*4);
	
	Imprime_registradores(Registradores);
	Imprime_RAM(RAM);

	free(RAM);

	return 0;
}


int decifra_instrucao(unsigned int inst){
	int cod_inst = inst >> 26;
	if(cod_inst != 0 && cod_inst != 2 && cod_inst != 3 && cod_inst != 4 && cod_inst != 5  && cod_inst != 8 && cod_inst != 12 && cod_inst != 20 && cod_inst != 21 && cod_inst != 35 && cod_inst != 43)
		return -1;

	return cod_inst;
}


int  decifra_operacao(int inst){
	int cod_op;

	cod_op = (inst & 63); // 111111

	 if(cod_op == 32){ // add
	 	cod_op = 0;
	 }else if(cod_op == 34){ // sub
	 	cod_op = 1;
	 }else if(cod_op == 36){ // and
	 	cod_op = 3;
	 }else if(cod_op == 37){ // or
	 	cod_op = 4;
	 }else if(cod_op == 42){ // set less than
	 	cod_op = 5;
	 }else{
	 	return -1;
	 }

	 return cod_op;

}

void seta_sinal_controle(UC* u, int cod_inst){


	char S0 = u->estado_atual.s0;
	char S1 = u->estado_atual.s1;
	char S2 = u->estado_atual.s2;
	char S3 = u->estado_atual.s3;

	// Setando os sinal de Controle da ULA segundo uma PLA
	u->IorD = (NOT(S3) AND S2 AND NOT(S1) AND S0) OR (NOT(S3) AND NOT(S2) AND S1 AND S0);
	u->MemtoReg0 = (NOT(S3) AND S2 AND NOT(S1) AND NOT(S0));
	u->MemtoReg1 = (S3 AND S2 AND S1 AND NOT(S0)) OR (S3 AND S2 AND NOT(S1) AND S0);
	u->IRWrite = NOT(S3) AND NOT(S2) AND NOT(S1) AND NOT(S0);
	u->PCSource0 = (S3 AND NOT(S2) AND NOT(S1) AND NOT(S0)) OR (S3 AND S2 AND NOT(S1) AND NOT(S0)) OR (S3 AND S2 AND S1 AND S0);
	u->PCSource1 = (S3 AND NOT(S2) AND NOT(S1) AND S0) OR (S3 AND S2 AND NOT(S1) AND S0) OR (S3 AND S2 AND S1 AND S0);
	u->ALUOp0 = (S3 AND NOT(S2) AND NOT(S1) AND NOT(S0)) OR (S3 AND S2 AND NOT(S1) AND NOT(S0)) OR (S3 AND NOT(S2) AND S1 AND NOT(S0));
	u->ALUOp1 = (NOT(S3) AND S2 AND S1 AND NOT(S0)) OR (S3 AND NOT(S2) AND S1 AND NOT(S0));
	u->ALUSrcB0 = (NOT(S3) AND NOT(S2) AND NOT(S1) AND NOT(S0)) OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0);
	u->ALUSrcB1 = (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0) OR (NOT(S3) AND NOT(S2) AND S1 AND NOT(S0)) OR (S3 AND NOT(S2) AND S1 AND NOT(S0));
	u->ALUSrcA = (NOT(S3) AND NOT(S2) AND S1 AND NOT(S0)) OR (S3 AND NOT(S2) AND NOT(S1) AND NOT(S0)) OR (S3 AND S2 AND NOT(S1) AND NOT(S0)) OR (S3 AND NOT(S2) AND S1 AND NOT(S0)) OR (NOT(S3) AND S2 AND S1 AND NOT(S0));
	u->RegDst0 = NOT(S3) AND S2 AND S1 AND S0;
	u->RegDst1 = S3 AND S2 AND NOT(S1) AND S0;
	u->BNE = S3 AND S2 AND NOT(S1) AND NOT(S0);
	u->PCWriteCond = (S3 AND NOT(S2) AND NOT(S1) AND NOT(S0)) OR (S3 AND S2 AND NOT(S1) AND NOT(S0));
	u->PCWrite =  (NOT(S3) AND NOT(S2) AND NOT(S1) AND NOT(S0)) OR (S3 AND NOT(S2) AND NOT(S1) AND S0) OR (S3 AND S2 AND NOT(S1) AND S0) OR (S3 AND S2 AND S1 AND S0);
	u->MemWrite = NOT(S3) AND S2 AND NOT(S1) AND S0;
	u->MemRead = (NOT(S3) AND NOT(S2) AND NOT(S1) AND NOT(S0)) OR (NOT(S3) AND NOT(S2) AND S1 AND S0);
	u->RegWrite = (NOT(S3) AND S2 AND NOT(S1) AND NOT(S0)) OR (NOT(S3) AND S2 AND S1 AND S0) OR (S3 AND NOT(S2) AND S1 AND S0) OR (S3 AND S2 AND NOT(S1) AND S0) OR (S3 AND S2 AND S1 AND NOT(S0));

	int OP0, OP1, OP2, OP3, OP4, OP5;

	// Extraindo cada Bite do Codido de instrução
	OP5 = cod_inst >> 5;
	OP4 = (cod_inst & 16) >> 4; // 16 = 10000
	OP3 = (cod_inst & 8) >> 3; // 8 = 01000
	OP2 = (cod_inst & 4) >> 2; // 4 = 00100
	OP1 = (cod_inst & 2) >> 1; // 2 = 00010
	OP0 = (cod_inst & 1); // 1 = 00001

	// Calculando os bits do Proximo Estado segundo uma PLA
	u->proximo_estado.s0 = (NOT(S3) AND NOT(S2) AND NOT(S1) AND NOT(S0))
		OR (NOT(S3) AND S2 AND S1 AND NOT(S0))
		OR (S3 AND NOT(S2) AND S1 AND NOT(S0))
	 	OR (S3 AND S2 AND S1 AND NOT(S0))
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND NOT(OP4) AND NOT(OP3) AND NOT(OP2) AND OP1 AND NOT(OP0))
	 	OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND NOT(OP4) AND NOT(OP3) AND NOT(OP2) AND OP1 AND OP0)
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND OP4 AND NOT(OP3) AND OP2 AND NOT(OP1) AND NOT(OP0))
		OR (NOT(S3) AND NOT(S2) AND S1 AND NOT(S0) AND OP5 AND NOT(OP4) AND NOT(OP3) AND NOT(OP2) AND OP1 AND OP0)
		OR (NOT(S3) AND NOT(S2) AND S1 AND NOT(S0) AND OP5 AND NOT(OP4) AND OP3 AND NOT(OP2) AND OP1 AND OP0)
		OR (NOT(S3) AND NOT(S2) AND S1 AND NOT(S0) AND NOT(OP5) AND NOT(OP4) AND OP3 AND NOT(OP2) AND NOT(OP1) AND NOT(OP0));

	u->proximo_estado.s1 = (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND OP5 AND NOT(OP4) AND NOT(OP3) AND NOT(OP2) AND OP1 AND OP0)
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND OP5 AND NOT(OP4) AND OP3 AND NOT(OP2) AND OP1 AND OP0)
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND NOT(OP4) AND OP3 AND NOT(OP2) AND NOT(OP1) AND NOT(OP0))
		OR (NOT(S3) AND NOT(S2) AND S1 AND NOT(S0) AND OP5 AND NOT(OP4) AND NOT(OP3) AND NOT(OP2) AND OP1 AND OP0)
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND OP4 AND NOT(OP3) AND OP2 AND NOT(OP1) AND NOT(OP0))
		OR (NOT(S3) AND NOT(S2) AND S1 AND NOT(S0) AND NOT(OP5) AND NOT(OP4) AND OP3 AND NOT(OP2) AND NOT(OP1) AND NOT(OP0))
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND NOT(OP4) AND NOT(OP3) AND NOT(OP2) AND NOT(OP1) AND NOT(OP0))
		OR (NOT(S3) AND S2 AND S1 AND NOT(S0))
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND NOT(OP4) AND OP3 AND OP2 AND NOT(OP1) AND NOT(OP0))
	 	OR (S3 AND NOT(S2) AND S1 AND NOT(S0))
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND OP4 AND NOT(OP3) AND OP2 AND NOT(OP1) AND OP0)
		OR (S3 AND S2 AND S1 AND NOT(S0));

	u->proximo_estado.s2 = (NOT(S3) AND NOT(S2) AND S1 AND S0)
		OR (NOT(S3) AND NOT(S2) AND S1 AND NOT(S0) AND OP5 AND NOT(OP4) AND OP3 AND NOT(OP2) AND OP1 AND OP0)
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND NOT(OP4) AND NOT(OP3) AND NOT(OP2) AND NOT(OP1) AND NOT(OP0))
		OR (NOT(S3) AND S2 AND S1 AND NOT(S0))
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND NOT(OP4) AND NOT(OP3) AND OP2 AND NOT(OP1) AND OP0)
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND NOT(OP4) AND NOT(OP3) AND NOT(OP2) AND OP1 AND OP0)
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND OP4 AND NOT(OP3) AND OP2 AND NOT(OP1) AND OP0)
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND OP4 AND NOT(OP3) AND OP2 AND NOT(OP1) AND NOT(OP0))
		OR (S3 AND S2 AND S1 AND NOT(S0));

	u->proximo_estado.s3 =(NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND NOT(OP4) AND NOT(OP3) AND OP2 AND NOT(OP1) AND NOT(OP0))
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND NOT(OP4) AND NOT(OP3) AND NOT(OP2) AND OP1 AND NOT(OP0))
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND NOT(OP4) AND OP3 AND OP2 AND NOT(OP1) AND NOT(OP0))
		OR (S3 AND NOT(S2) AND S1 AND NOT(S0))
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND NOT(OP4) AND NOT(OP3) AND OP2 AND NOT(OP1) AND OP0)
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND NOT(OP4) AND NOT(OP3) AND NOT(OP2) AND OP1 AND OP0)
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND OP4 AND NOT(OP3) AND OP2 AND NOT(OP1) AND OP0)
		OR (NOT(S3) AND NOT(S2) AND NOT(S1) AND S0 AND NOT(OP5) AND OP4 AND NOT(OP3) AND OP2 AND NOT(OP1) AND NOT(OP0))
		OR (S3 AND S2 AND S1 AND NOT(S0))
		OR (NOT(S3) AND NOT(S2) AND S1 AND NOT(S0) AND NOT(OP5) AND NOT(OP4) AND OP3 AND NOT(OP2) AND NOT(OP1) AND NOT(OP0));
}

void Imprime_Sinais(UC Control_Unit){

	printf("Controle: \n");

	printf("MemWrite = %d\n", Control_Unit.MemWrite);
	printf("RegWrite = %d\n", Control_Unit.RegWrite);
	printf("MemRead = %d\n", Control_Unit.MemRead);
	printf("ALUSrcA = %d\n", Control_Unit.ALUSrcA);
	printf("IorD = %d\n", Control_Unit.IorD);
	printf("IRWrite = %d\n", Control_Unit.IRWrite);
	printf("ALUSrcB0 = %d\n", Control_Unit.ALUSrcB0);
	printf("ALUSrcB1 = %d\n", Control_Unit.ALUSrcB1);
	printf("ALUOp0 = %d\n", Control_Unit.ALUOp0);
	printf("ALUOp1 = %d\n", Control_Unit.ALUOp1);
	printf("PCWrite = %d\n", Control_Unit.PCWrite);
	printf("PCSource0 = %d\n", Control_Unit.PCSource0);
	printf("PCSource1 = %d\n", Control_Unit.PCSource1);
	printf("MemtoReg0 = %d\n", Control_Unit.MemtoReg0);
	printf("MemtoReg1 = %d\n", Control_Unit.MemtoReg1);
	printf("RegDst0 = %d\n", Control_Unit.RegDst0);
	printf("RegDst1 = %d\n", Control_Unit.RegDst1);
	printf("PCWriteCond = %d\n", Control_Unit.PCWriteCond);
	printf("BNE = %d\n", Control_Unit.BNE);

	printf("\n");

}


void Imprime_registradores(int* Registradores){

	printf("\nBanco de Registradores\n");

	printf("R00(r0)=%d\n", Registradores[0]);
	printf("R01(at)=%d\n", Registradores[1]);
	printf("R02(v0)=%d\n", Registradores[2]);
	printf("R03(v1)=%d\n", Registradores[3]);
	printf("R04(a0)=%d\n", Registradores[4]);
	printf("R05(a1)=%d\n", Registradores[5]);
	printf("R06(a2)=%d\n", Registradores[6]);
	printf("R07(a3)=%d\n", Registradores[7]);
	printf("R08(t0)=%d\n", Registradores[8]);
	printf("R09(t1)=%d\n", Registradores[9]);
	printf("R10(t2)=%d\n", Registradores[10]);
	printf("R11(t3)=%d\n", Registradores[11]);
	printf("R12(t4)=%d\n", Registradores[12]);
	printf("R13(t5)=%d\n", Registradores[13]);
	printf("R14(t6)=%d\n", Registradores[14]);
	printf("R15(t7)=%d\n", Registradores[15]);
	printf("R16(s0)=%d\n", Registradores[16]);
	printf("R17(s1)=%d\n", Registradores[17]);
	printf("R18(s2)=%d\n", Registradores[18]);
	printf("R19(s3)=%d\n", Registradores[19]);
	printf("R20(s4)=%d\n", Registradores[20]);
	printf("R21(s5)=%d\n", Registradores[21]);
	printf("R22(s6)=%d\n", Registradores[22]);
	printf("R23(s7)=%d\n", Registradores[23]);
	printf("R24(t8)=%d\n", Registradores[24]);
	printf("R25(t9)=%d\n", Registradores[25]);
	printf("R26(k0)=%d\n", Registradores[26]);
	printf("R27(k1)=%d\n", Registradores[27]);
	printf("R28(gp)=%d\n", Registradores[28]);
	printf("R29(sp)=%d\n", Registradores[29]);
	printf("R30(fp)=%d\n", Registradores[30]);
	printf("R31(ra)=%d\n", Registradores[31]);
	printf("\n");

}

void Imprime_RAM(unsigned int* RAM){

	int i;
	printf("Memória (endereços a byte)\n");
	for(i = 0; i < (256/4); i++){
		if((i*4) < 10)
			printf("[0%d] = %u\n", i*4, RAM[i]);
		else
			printf("[%d] = %u\n", i*4, RAM[i]);
	}
	printf("\n");
}