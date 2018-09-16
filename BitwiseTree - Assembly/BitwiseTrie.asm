# NOMES: EDUARDO ALVES BARATELA             NºUSP: 10295270
#	 JOÃO VICTOR GARCIA COELHO	           10349540  
#	 MATHEUS SANCHEZ                            9081453
#        PEDRO PAULO ISNARD BRANDO                  9845221
   
.data
STR_MENU: .asciiz "Menu principal de opções\n1 - Inserção,\n2 - Remoção,\n3 - Busca,\n4 - Visualização,\n5 - Fim\n\n Escolha uma opção (1 a 5): \n " 

  
STR_DIG: .asciiz ">> Digite o binário para "
STR_INS: .asciiz "inserção: "
STR_REM: .asciiz "remoção: "
STR_BUS: .asciiz "busca: "
STR_SUC: .asciiz ">> Chave inserida com sucesso\n"
STR_CHAVE_REP: .asciiz ">> Chave repetida. Inserção não permitida.\n"
STR_INV: .asciiz ">> Chave inválida. Insira somente números binários (ou -1 retorna ao menu).\n"
STR_RET: .asciiz ">> Retornando ao menu.\n"
STR_REM_SUC: .asciiz ">> Chave removida com sucesso.\n"

STR_CORRETO: .asciiz ">> Chave encontrada na árvore: "
STR_INCORRETO: .asciiz ">> Chave não encontrada na árvore: -1\n"
STR_CAMINHO: .asciiz ">> Caminho percorrido: raiz"
STR_LEFT: .asciiz "esq"
STR_RIGHT: .asciiz "dir"
STR_VIRGULA: .asciiz ", "
STR_N: .asciiz "\n"
STR_SPACE: .asciiz " "

STR_RAIZ: .asciiz "raiz, " 
STR_ABRE: .asciiz "("
STR_FECHA: .asciiz ")"
STR_TERMINAL: .asciiz "T, "
STR_NAO_TERMINAL: .asciiz "NT, "
STR_NULL: .asciiz "null, "
STR_NULL2: .asciiz "null)"
STR_LEVEL: .asciiz ">> N"

.text

j main

print_str:                                                    #printa string que está em $a0
  li $v0,4    
  syscall
  jr $ra  
  
read_int:                                                     #leitura int, retorno em $v0
  li $v0,5    
  syscall
  jr $ra    

print_int:
  li $v0,1    
  syscall
  jr $ra 

read_str:                                                     
 li $v0, 9                                                    #leitura string
 li $a0, 17                                                   #alocando memória para a string a ser lida
 syscall
 
 move $s1, $v0                                                #Guarda o endereço da memoria alocada em $s1
 
 li $v0, 8                                                    #leitura de string
 la $a0, 0($s1)                                               #guarda a string a partir do byte 0 de $s1 que é onde a memoria está alocada
 li $a1, 17                                                   #17 é o tamanho máximo da string e também o numero de bytes alocados
 syscall
 
 move $t5, $s1                                                #$t5 e um auxiliar que ajuda a validar a string

 valida_str:
 	lb $t3, 0($t5)                                        #le o bit atual
 	li $t4, 10                                            #10 é \n em ASCII
 	bne $t3, $t4, verifica_negativo                       #se bit lido não for \n verificamos se é negativo, senao é valida
 	jr $ra
 	
verifica_negativo:                                      
	li $t4, 45                                            #45 é '-' em ASCII
 	bne $t3, $t4, verifica_bit                            #se o bit lido não for negativo verificamos se é 0 ou 1
 	la $a0, STR_RET                                       #printa "Retornando ao menu"
 	li $v0,4    
  	syscall
 	j Menu
 	
 	jr $ra
 	
verifica_bit:
  	li $t1, 49                                            #49 é 1 em ASCII                                     
 	li $t2, 48                                            #48 é 0 em ASCII
 	
 	addi $t5, $t5, 1                                      #ando na string
 	
 	beq $t3, $t1 valida_str                               #se o bit lido for 1 vamos ao proximo
 	beq $t3, $t2 valida_str                               #se o bit lido for 0 vamos ao proximo
 	                                                      #senão...
 	la $a0, STR_INV                                       #printa "Chave inválida. Insira somente números binários (ou -1 retorna ao menu).\n"
 	li $v0,4    
  	syscall
  	
  	la $a0, STR_DIG                                       #printa "Digite o binário para..."
  	li $v0,4    
  	syscall
  	
  	BEQ $t0, 1, INS                                       #Se o numero escolhido for igual a 1 printa "insercao"
  	BEQ $t0, 2, REM                                       #Se o numero esoclhido for igual a 2 printa "remocao"
  	BEQ $t0, 3, BUS                                       #Se o numero esoclhido for igual a 3 printa "busca"
  	
INS: 
 	la $a0, STR_INS                                       #printa "insercao"
  	li $v0,4    
  	syscall
  	j read_str
REM:
	la $a0, STR_REM                                       #printa "remocao"
  	li $v0,4    
  	syscall	
  	j read_str
  	
BUS: 	
	la $a0, STR_BUS                                       #printa "busca"
  	li $v0,4    
  	syscall
 	j read_str
 jr $ra
 
CHAVE_REP:
	la $a0, STR_CHAVE_REP                                 #printa "Chave repetida. Inserção não permitida.\n"
	jal print_str
	j Menu

aloca_cabeca:
  #    |END ESQUERDA|  => 0000
  #    |------------|
  #    |END DIREITA |  => 0000
  #    |------------|
  #    | NRO DE NOS |  => 1 -> SEMPRE HÁ RAIZ
  
  
  li $v0,9                                         #aloca raiz
  li $a0,9                                        
  syscall           
  move $s5,$v0                                     #guarda raiz em $s5   
  addi $t3, $t3, 1                                 #$t3 seta o numero de nós inicial para 1 (raiz)
  	
  sw  $zero, 0($s5)                                #    |END ESQUERDA|  => 0000
  sw  $zero, 4($s5)                                #    |END DIREITA |  => 0000
  sw  $t3, 8($s5)                                  #    | NRO DE NOS |  => 1 -> SEMPRE HÁ RAIZ 
  jr $ra                                       
  						     
aloca_nos:     
#      NÓ PADRÃO
#     ____________
#    |END ESQUERDA|
#    |------------|
#    |END DIREITA |
#    |------------| 
#    | FIM NÚMERO |  -> marcador que indica se é o fim do numero
#    |------------|
  
  li $v0,9  
  li $a0,9          
  syscall         
  
  move $s0, $v0                                    #coloca em $s0 o nó alocado 
  lw $t7, 8($s5)                                   #lemos o numero de nós da arvore
  addi $t7, $t7, 1                                 #somamos 1 ao numero de nós
  sw $t7, 8($s5)                                   #atualizamos o numero de nós
  beq $t3, 4, add_direita                          #$t3 indica em qual lado adicionar o novo nó (0 -> esq, 4 -> dir) 
  
add_esquerda:
  sw $s0, 0($s6)                                   #$s6 é o pai do novo nó. pai->esquerda = endereço do novo nó alocado
  move $s4,$s0                                     #andamos na arvore
  li $t1,1                                         #indica que o nó não é terminal
  sw $t1,8($s4)                                    
  
  j Insercao 
  
add_direita:
    sw $s0, 4($s6)                                 #$s6 é o pai do novo nó. pai->direita = endereço do novo nó alocado
    move $s4,$s0                                   #andamos na arvore
    li $t1,1                                       #indica que o nó não é terminal
    sw $t1,8($s4)
    
  j Insercao   
                  
main:  
  jal aloca_cabeca                                 #alocar o nó cabeça que ficara no registrador #t0 
 
 

Menu:
  li $t2, 10                                       #$t2 armazena o valor de \n em ASCII           
  li $t1, 48                                       # 48 é 0 em ASCII. Usado para comparar e ver se o bit lido é 0
  la $a0, STR_MENU                                 #Printa o menu
  jal print_str

  jal read_int                                     #Le as opcoes(um inteiro) #valor lido em $v0
  move $t0, $v0                                    #Guarda o valor em um temporario $t0
  
  BEQ $t0, 1, OP1                                  #Se o numero escolhido for igual a 1
  BEQ $t0, 2, OP2                                  #Se o numero esoclhido for igual a 2
  BEQ $t0, 3, OP3                                  #Se o numero esoclhido for igual a 3
  BEQ $t0, 4, OP4                                  #Se o numero esoclhido for igual a 4
  BEQ $t0, 5, fim_programa                         #Se o numero esoclhido for igual a 5
  
   MarcaNo:
   
   	lw $t4, 8($s4)
   	li $t7,2
	BEQ  $t4, $t7 CHAVE_REP
  	
  	sw $t7,	8($s4)
  	j Menu	
    
OP1:                                               #INSERCAO

  la $a0, STR_DIG                                  #printa "Digite o binario para..."
  jal print_str
  
  la $a0, STR_INS                                  #printa "...insercao: "
  jal print_str
  
  
  jal read_str                                     #le a string guardando a memória alocada em $s1
  
  move $s4, $s5                                    #$s4 e ponteiro auxiliar para raiz $s5 inicialmente, e aponta sempre para o atual
  
Insercao:
    li $t1, 48                                     #48 é 0 em ASCII. Usado para comparar e ver se o bit lido é 0
    li $t2, 10                                     #10 e \n em ASCII
    lb $t0, 0($s1)                                 #le o bit atual da string
    beq $t0, $t2 MarcaNo                           #se cheguei no \n, ou seja, fim da string, marco o nó como terminal
    beq $t0, $t1 goLeft                            #se o bit lido for 0 vou para a esquerda, senão vou para a direita
    
goRight:
    li $t3, 4                                      #$t3 = 4 -> direta, $t3 = 0 ->esquerda
    move $s6, $s4                                  #guarda o pai
    addi $s1, $s1, 1                               #anda na string
    lw $s4, 4($s4)                                 #$s4 = $s4->direta
    beqz $s4, aloca_nos                            #verifica se alcançamos o nulo
    j Insercao        

goLeft:
    li $t3, 0                                      #$t3 = 4 -> direta, $t3 = 0 ->esquerda 
    move $s6, $s4                                  #guarda o pai
    addi $s1, $s1, 1                               #anda na string
    lw $s4, 0($s4)                                 #$s4 = $s4->esquerda
    beqz $s4, aloca_nos                            #verifica se alcançamos o nulo
    
    j Insercao
      
  j Menu

verifica_rem:
	li $t6, 2                                  #$t6 armazena 2 que é o marcador que indica que o numero é terminal
  	lb $t4, 8($s4)                             #le o oitavo byte do nó atual 
  	
	BEQ  $t4, $t6  guarda_no                   #verifica se o numero existe, ou seja, se é igual a 2. Se sim, s7 = s4
	
	lw $t4, 0($s4)                             #$t4 recebe esquerda do nó atual
	bne $t4, $zero, testa_direita              #verifica se o nó possui filho a esquerda, senão voltamos pra chamada da funcao
	jr $ra
	
testa_direita:
	lw $t4, 4($s4)                             #$t4 recebe esquerda do nó atual
	bne $t4, $zero, guarda_no                  #verifica se o nó possui filho a direita. Se tiver atualiza $s7 e $s2
	jr $ra	     
	                     
guarda_no:
	move $s2, $t5
	move $s7, $s4	
	
	jr $ra

remove_no: 
	sw $zero, 8($s4)                          #Indica que o nó não é terminal 
	j Menu

OP2:               #REMOCAO
  la $a0, STR_DIG                                 #printa "Digite o binario para..."
  jal print_str
  
  la $a0, STR_REM                                 #printa "...insercao: "
  jal print_str
  
  jal read_str                                    #le a string guardando a memória alocada em $s1
  li $t7, 1                                       #indica que a busca foi chamada pela remocao
  move $s4, $s5                                   #$s4 = ponteiro auxiliar para raiz $s5 inicialmente
  move $t5, $s1                                   #move o inicio da string para registrador auxiliar que será incrementado
  move $s2, $s1                                   #$s2 indica para qual lado o "galho" da arvore será podado
  move $s7, $s5                                   #$s7 inidica o nó a partir do qual a arvore será podada
  j Busca
  
remove:
	beqz $t9, Menu                            #se $t9 for igual a 0 a busca foi mal sucedida            
	la $a0, STR_REM_SUC                       #printa "Remocao bem sucedida. Nó removido com sucesso"
	jal print_str
	
	lw $t4, 0($s4)                            #verifica se o nó atual tem filho a esquerda
	bne $t4, $zero, remove_no
	
	lw $t4, 4($s4)                            #verifica se o nó atual tem filho a direita
	bne $t4, $zero, remove_no                 #se tiver "remove" lógicamente o nó, setando o nó como não terminal
	
	lb $t0, 0($s2)                            #le o bit da posicao do primeiro nó a não ser podado
	addi $s2, $s2, 1                          #anda a partir dessa posicao
	
	lw $t7, 8($s5)                            #diminui o numero de nós da arvore
  	addi $t7, $t7, -1
  	sw $t7, 8($s5)
	
	beq $t0, $t1, remove_esq                  #remove o galho da esquerda de determinado nó ($s7)
	
remove_dir:
	sw $zero, 4($s7)                          #remove o galho da direita de determinado nó ($s7)
	j att_numero_nos                          #atualiza o numero de nós
 
remove_esq:
	sw $zero, 0($s7)                          #remove o galho da esquerda de determinado nó ($s7)

att_numero_nos:
	beq $s2, $t5, Menu                        
	lw $t7, 8($s5)                            #a partir de determinado nó ($s7) andarei pelo galho a ser podado, até
  	addi $t7, $t7, -1                         #o nó terminal do numero a ser removido, decrementado, em 1 o numero de nós
  	sw $t7, 8($s5)                            #da arvore, a cada iteracao.
	addi $s2, $s2, 1
	j att_numero_nos
	    
  j Menu
  
OP3:               #BUSCA
  la $a0, STR_DIG                                      #Digite o binário para: 
  jal print_str
  
  la $a0, STR_BUS                                      #busca
  jal print_str
  
  jal read_str                                         #le a string guardando a memória alocada em $s1
  li $t7, 0                                            #indica que a busca foi chamada no Menu
  move $s4, $s5                                        #$s4 = ponteiro auxiliar para raiz $s5
  move $t5, $s1                                        #move o inicio da string para registrador auxiliar que será incrementado
  move $s7, $s5                                        #$s7 inicia na raiz
  
Busca:
  li $t1, 48                                           #48 é 0 em ASCII. Usado para comparar e ver se o bit lido é 0
  li $t2, 10                                           #10 é \n em ASCII
  lb $t0, 0($t5)                                       #$t0 é o bit atual lido
  
  
  beq $t0, $t2, verifica_num                          #verifica se o bit atual é \n, se for comeca a printar a resposta
  jal verifica_rem                                    #verifica se o nó é terminal ou possui dois filhos
  beq $t0, $t1 SearchLeft                             #verifica se o bit lido é 0. Se for procura na esquerda senão na direta
    
SearchRight:
    move $s6, $s4                                      #guarda o pai
    lw $s4, 4($s4)                                     #le o endereço do filho da direita
    beqz $s4, print_resp                               #verifica se alcançamos o nulo, se sim printamos a resposta
    addi $t5, $t5, 1                                   #anda na string
    j Busca        

SearchLeft:
    move $s6, $s4                                      #guarda o pai
    lw $s4, 0($s4)                                     #le o endereço do filho da esquerda
    beqz $s4, print_resp                               #verifica se alcançamos o nulo
    addi $t5, $t5, 1                                   #anda na string
    j Busca
    
print_fim:
	la $a0, STR_N                                  #printa o \n
	jal print_str
	beqz $t7, Menu                                 #se a busca foi chamada pelo Menu, voltamos a ele, senão voltamos a remocao
	j remove
 
print_caminho:
	beq $s1, $t5 print_fim                         #verifica se o bit atual da string é um nó folha, senão...
	lb $t4, 0($s1)                                 #le bit atual da string
	addi $s1, $s1, 1                               #anda na string
	li $t1, 48                                     # 48 é 0 em ASCII. Usado para comparar e ver se o bit lido é 0
	la $a0, STR_VIRGULA                            #printa ", "
	jal print_str
	beq $t4, $t1 printLeft                         #verifica se o bit lido é 0, se for printa "esq"
	la $a0, STR_RIGHT                              #senão printa "dir"
  	jal print_str
  	beq $s1, $t5 print_fim                         #verifica se o bit atual da string é um nó folha, senão...
  	j print_caminho
  	
printLeft:
	la $a0, STR_LEFT                                    
  	jal print_str                                  #verifica se o bit lido é 1, se for printa "dir"
  	
  	beq $s1, $t5 print_fim                         
  	
	j print_caminho
 
print_incorreto:                                       #printa as strings de resposta caso o numero não tenha sido adicionado
 	la $a0, STR_INCORRETO                          #e segue para printar o caminho percorrido
  	jal print_str
  	la $a0, STR_CAMINHO
  	jal print_str
  	li $t9, 0 					# marca que a busca foi mal sucedida
  	j print_caminho
 
 print_correto:                                        #printa as strings de resposta caso o numero tenha sido adicionado
 	la $a0, STR_CORRETO                            #e segue para printar o caminho percorrido 
  	jal print_str
  	la $a0, 0($s1)
  	jal print_str
  	la $a0, STR_CAMINHO
  	jal print_str
  	li $t9, 1 					# marca que a busca foi bem sucedida
  	
  	j print_caminho
 
 verifica_num:
  	li $t6, 2                                      #$t6 armazena 2 que é o marcador que indica q o numero existe
  	lb $t4, 8($s4)                                 #le o oitavo byte do nó atual 
  	
	BEQ  $t4, $t6 print_correto                    #verifica se o numero existe, ou seja, se é igual a 2
	
	j print_incorreto
	  

  print_resp:
  	lb $t0,0($t5)	                               #le o bit atual e verifica se é \n
  	beq $t0, $t2 verifica_num                      #se for \n string acabou, então verificamos se o numero foi adicionado
        j print_incorreto                              #se o numero não foi adicionado printamos "chave não encontrada na arvore: -1"
  	
  	
  j Menu
  
aloca_fila:
	lw $t3, 8($s5) 				       #leio o numero de nós que tem na arvore
        mul $t3, $t3, 8 			       #multiplico por 4, pois será o numero de bytes necessários para a fila(um endereço necessita de 4 bytes)
	li $v0, 9				       #alocando a memória de tamanho de $t3
	move $a0, $t3 
	syscall
	
	move $t0, $v0                                  # t0 é o inicio da fila
	move $t1, $v0				       # t1 é o fim da fila
	move $t2, $v0				       # t2 é um ponteiro auxiliar 
	li $t8, -1
	sw $t8, 4($t0) 
	
	la $a0, STR_LEVEL
	jal print_str
	li $t6, 0                                      # auxiliar com o numero do nivel
	
	
	move $a0, $t6
	jal print_int
	
	sw $s5, 0($t0)				       # colocando o endereço da raiz na fila
	j Vizualizacao

adiciona_no:
	addi $t2, $t2, 8                              #$t2 é um ponteiro auxiliar que indica o ultimo nó adicionado na fila, andamos com ele
	sw $t4, 0($t2)                                #salvo o endereço do filho na fila 
	sw $t8, 4($t2)                                #salvo o inteiro que indica se é filho da esquerda ou direita
	jr $ra

add_filhos:
	lw $s4, 0($t0)                                #$s4 aponta para o endereço do nó no inicio da fila
	lw $t4, 0($s4)                                #$t4 aponta para o filho da esquerda do primeiro nó da fila
	
	
	beq $t4, $zero nao_add                        #se não tiver filho na esquerda não adiciono o filho na fila
	li $t8, 0                                     #determina se o nó era filho da esquerda ou da direita (0 ->esq, 1 -> dir)
	jal adiciona_no                               #adiciona o nó na fila
	
nao_add:
	lw $t4, 4($s4)                                #$t4 aponta para o filho da direita do primeiro nó da fila
	
	beq $t4, $zero print_no                       #se for 0 printa o nó atual
	li $t8, 1                                     #indica que o filho é da direita
	jal adiciona_no
	j print_no	
 
print_null:
	la $a0, STR_NULL                              #printa "null"
	jal print_str
	j print_right
	
print_null2:
	la $a0, STR_NULL2                             #printa "null)
	jal print_str
	
	beq $t0, $t1 print_end_level                  #verifica se o inicio é igual ao fim, se for printa "\n"
	addi $t0, $t0, 8                              #senao inicio++
	
	
	j Vizualizacao 
	
	
print_no:
	lw $s4, 0($t0)                                #armazena em $s4 o endereço para o nó a ser printado
	la $a0, STR_ABRE                              #printa "("
	jal print_str
	li $t8, -1                                    
	lw $t9, 4($t0)
	beq $t8, $t9, print_raiz                      #se o inteiro do inicio da fila for -1, então o no a ser printado é a raiz
	lw $a0, 4($t0)				      #senao printa o inteiro (0 ou 1)
	jal print_int
						      
	la $a0, STR_VIRGULA                           #printa ", "
	jal print_str
	j print_terminal                            
	
print_raiz:
	la $a0, STR_RAIZ                              #printa "raiz"
	jal print_str	
	
print_terminal:	 
	lw $t4, 8($s4)                                #$t4 recebe o indicador de nó terminal                    
	
	li $t8, 2                         
	beq $t4, $t8, print_t                         #verifica se o nó é terminal, ou seja, 8($s4) = 2
	
	la $a0, STR_NAO_TERMINAL                      #print "NT"
	jal print_str
	j print_left
	
print_t:
	la $a0, STR_TERMINAL                          #print "T"
	jal print_str

print_left:
	lw $t4, 0($s4)                                #$t4 recebe o endereço do filho a esquerda
	beqz $t4, print_null                          #se este for 0, printa "null" 
	la $a0, 0($s4)                                #senao...
	jal print_int                                 #printa o endereco do filho a esquerda
	
	la $a0, STR_VIRGULA                           #printa ", "
	jal print_str
	
	la $a0, STR_SPACE                             #printa " "
	jal print_str
	
	

print_right:	 
	lw $t4, 4($s4)                                #$t4 recebe o endereço do filho a direita
	beqz $t4, print_null2                         #se este for 0, printa "null)"
	la $a0, 0($s4)                                #senao...
	jal print_int                                 #printa o endereco do filho a direita
	
	la $a0, STR_FECHA                             #printa ")"
	jal print_str
	beq $t0, $t1 print_end_level                  #verifica se o fim é igual ao inicio, então o nó printado é o ultimo do nivel
	addi $t0, $t0, 8                              #inicio++
	
	
	j Vizualizacao
	
print_end_level:
	la $a0, STR_N                                  #printa o \n
	jal print_str
	
	move $t1, $t2                                  #atualiza o fim para o ponteiro do ultimo nó adicionado na fila
	beq $t0, $t2, Menu                             #verifica se inicio é igual a esse pomnteiro auxiliar, se for retorna ao menu
	addi $t0, $t0, 8                               #senao inicio++
	la $a0, STR_LEVEL                              #printa ">>N"
	jal print_str
	
	addi $t6, $t6, 1                               #incremento o nivel
	move $a0, $t6                                  #printa "N(nivel)"
	jal print_int

	j Vizualizacao

OP4:               #VISUALIZACAO
  move $s4, $s5                                        #$s4 = ponteiro auxiliar para raiz $s5
  
  jal aloca_fila			               #aloca fila estática

Vizualizacao:
	j add_filhos                                   #adiciona os filhos do nó atual na fila
  	beq $t0, $t1 print_end_level                   #se incio for igual ao fim printa o fim no nivel (\n)            
	
	j Menu

fim_programa:      #Terminando o programa 
  li $v0,10                                            #finaliza o programa
  syscall
