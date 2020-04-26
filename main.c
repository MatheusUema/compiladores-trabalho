#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char *argv[]) { //Compilador


    /* Função que recebe arquivo */

    if(argc == 1) {  //Execução sem enviar arquivo txt
        printf("[ERRO] Arquivo a ser lido não enviado\n");
        return 0;
    }
    

    /* Função que chama o Analisador Léxico */
    analisador_lexico(argv[1]);


}

int analisador_lexico (char *arquivo){
    
    FILE *arquivo_entrada = fopen(arquivo, "r");
    FILE *arquivo_saida = fopen("saida.txt", "w"); 
    if (arquivo_entrada == NULL) {   
        printf("[ERRO] Arquivo não encontrado\n"); 
        return 0;
    } 

    char caract;
    caract = fgetc(arquivo_entrada);
    while(fgetc(arquivo_entrada) != 32) {
        printf("aaaa\n");
    }




    fclose(arquivo_entrada);
    fclose(arquivo_saida);
}

int confere_numero (char caract){
    //Confere se o caractere é numero, checando seu valor na tabela ASCII
    if (caract >= 48 && caract <= 57){
        return 1;
    } 
    return 0;
}

int confere_letra (char caract) {
    //Confere se caracter é letra minúscula ou maiúscula
     if ((caract >= 65 && caract <= 90) || (caract >= 97 && caract <= 122)){
        return 1;
    } 
    return 0;
}

int confere_branco (char caract) {
    //Confere se caracter checado é espaço ou quebra de linha;
     if (caract == 32 || caract == 10){
        return 1;
    } 
    return 0;
}

void checa_invalido_num(char caract, int controle, int racional,int quebra_linha){

    if(confere_numero(caract) || caract == 46){ // se numero ou ponto o caracter eh aceito
        if(caract == 46){ //se for ponto eu marco o racional = 1 pra na hra q eu devolver a cadeia saber q eh racional
            racional = 1;
        }
        controle = 0; 
    } else { // se nao for num ou ponto, ou eh caracter invalido ou quebra de linha/"\n" 
        if(confere_branco(caract)){
            quebra_linha = 1;
        }else{// se nao for quebra de linha ai eh caracer invalido msmo eu marco com o controle = 1
            controle = 1;
        }
        
    }

}

void checa_invalido_palavra(char caract, int controle, int quebra_linha){

    // se for letra ou underline eh aceito, se nao ele checa se eh quebra d linha ou espaco, se nao quer dizer q eh caract invalido
    if(confere_letra(caract) || caract == 176 || caract == 177 || caract == 178 || caract ==242 || caract == 254){
        controle = 0;
    }else{
        if(confere_branco(caract)){
            quebra_linha = 1;
        }else{
            controle = 1;
        }
    }


}

void checa_invalido_comentario(char caract, int controle,int quebra_linha, char *arquivo){
    FILE *teste_fim_arq; //variavel q recebe o arq q eu to lendo
    teste_fim_arq = fopen(arquivo, "r");//abro pra leitura

    if(!feof(teste_fim_arq)){ // se nao ta no fim do arquivo entao continua lendo
        controle = 0;
    }else{
        controle = 1;
    }
    
    if(caract == 32){ // se eu tiver quebra de linha o comentario para
        quebra_linha = 1;
    }
        

}

void checa_invalido_reservado(char caract, int controle);

int seleciona_automato(char caract, int controle){ 

    //aqui n tem segredo, pra cada carct inicial eu tenho um automato definido

    if (confere_numero (caract)){ //Automato de números reais e inteiros
        controle = 1;
        return 0;
    }

    if(confere_letra(caract)){ //Autômato para identificadores e palavras reservadas
        controle = 2;
        return 0;
    }

    if (caract == 123){ //Autômato para comentários
        controle = 3;
        return 0;
    }

    if (confere_reservado(caract)){ //Autômato para símbolos reservados
        controle = 4;
        return 0;
    }
        
    if(confere_branco(caract) == 0){ //Checagem para espaços brancos e quebras de linha.
        controle = 5;
        return 0;
    }
       
}

int checa_invalido_geral (char caract, int controle, int automato, int racional, int quebra_linha, char *arquivo){

    if (automato == 1){
        checa_invalido_num(caract,controle, racional,quebra_linha);
        //analise de racional aqui
        //analise de quebra de linha
        return 0;
    }

    if (automato == 2){
        checa_valido_palavra(caract,controle,quebra_linha);
        //analise de quebra de linha
        return 0;
    }

    if (automato == 3){
        checa_invalido_comentario(caract,controle,quebra_linha, arquivo);
        //analise quebra linha
        return 0;
    }

     if (automato == 4){
        checa_invalido_reservado(caract,controle);
        return 0;
    }

    if (automato == 5){
        controle = 1;
        return 0;
    }

}


int devolvo_cadeia(char cadeia [29], int controle, int automato, int racional){

    if(automato == 1){// se no automato 1 entao eh numero
        if(racional == 1){// se o controle de racional for 1 entao eh nmero racional
            int x=0;
            while(cadeia[x] != NULL || x<29){ //percorro a cadeia ateh o final ou ateh achar um NULL
                printf("&c", cadeia[x]); //imprimo o caracter 
                x++;//repito o processo
            }

            printf(", numero racional/n");
            return 0;
        }

        int x=0;
            while(cadeia[x] != NULL || x<29){
                printf("&c", cadeia[x]);
                x++;
            }

            printf(", numero inteiro/n");
            return 0;

    }


    if(automato == 2){ // se automato 2 entao to em palavra

        // falta aqui identificar se eh comando ou variael

         int x=0;
            while(cadeia[x] != NULL || x<29){
                printf("&c", cadeia[x]);
                x++;
            }

            if(strncmp(cadeia, "program",7)==0){
                printf(", comando_reservado_program\n");
                return 0;
            };

             if(strncmp(cadeia, "begin",5)==0){
                printf(", comando_reservado_begin\n");
                return 0;
            }; 
            
            if(strncmp(cadeia, "end",3)==0){
                printf(", comando_reservado_end\n");
                return 0;
            }; 
            
            if(strncmp(cadeia, "const",5)==0){
                printf(", comando_reservado_const\n");
                return 0;
            }; 
            
            if(strncmp(cadeia, "var",3)==0){
                printf(", comando_reservado_var\n");
                return 0;
            }; 
            
            if(strncmp(cadeia, "real",4)==0){
                printf(", comando_reservado_ral\n");
                return 0;
            };
            
             if(strncmp(cadeia, "integer",7)==0){
                printf(", comando_reservado_integer\n");
                return 0;
            }; 
            
            if(strncmp(cadeia, "procedure",9)==0){
                printf(", comando_reservado_procedure\n");
                return 0;
            }; 
            
            if(strncmp(cadeia, "else",4)==0){
                printf(", comando_reservado_else\n");
                return 0;
            }; 
            
            if(strncmp(cadeia, "read",4)==0){
                printf(", comando_reservado_read\n");
                return 0;
            }; 
            
            if(strncmp(cadeia, "write",5)==0){
                printf(", comando_reservado_write\n");
                return 0;
            };
            
             if(strncmp(cadeia, "while",5)==0){
                printf(", comando_reservado_while\n");
                return 0;
            };
            
             if(strncmp(cadeia, "do",2)==0){
                printf(", comando_reservado_do\n");
                return 0;
            };
            
             if(strncmp(cadeia, "for",2)==0){
                printf(", comando_reservado_for\n");
                return 0;
            }; 
            
            if(strncmp(cadeia, "if",2)==0){
                printf(", comando_reservado_if\n");
                return 0;
            }; 
            
            if(strncmp(cadeia, "for",3)==0){
                printf(", comando_reservado_for\n");
                return 0;
            };

           


    }

    if(automato == 3){ // se automato 3 entao to em comentario

         int x=0;
            while(cadeia[x] != NULL || x<29){
                printf("&c", cadeia[x]);
                x++;
            }

            printf(", comentario/n");
            return 0;



    }

    if(automato == 4){ //se automato 4 entao to em simbolos reservados

         int x=0;
            while(cadeia[x] != NULL || x<29){
                printf("&c", cadeia[x]);
                x++;
            }

             if(strncmp(cadeia, "=",1)==0){
                printf(", comando_reservado_igual\n");
                return 0;
            };

             if(strncmp(cadeia, "<",1)==0){
                 if(strncmp(cadeia, "<>",2)==0){
                      printf(", comando_reservado_menor_maior\n");
                      return 0;
                 }

                 if(strncmp(cadeia, "<=",2)==0){
                     printf(", comando_reservado_menor_igual\n");
                     return 0;
                 }
                printf(", comando_reservado_menor\n");
                return 0;
            };


             if(strncmp(cadeia, ">",1)==0){
                 if(strncmp(cadeia, ">=",1)==0){
                      printf(", comando_reservado_maior_igual\n");
                 }
                printf(", comando_reservado_maior\n");
                return 0;
            };
             if(strncmp(cadeia, "for",3)==0){
                printf(", comando_reservado_for\n");
                return 0;
            };
             if(strncmp(cadeia, "for",3)==0){
                printf(", comando_reservado_for\n");
                return 0;
            };
             if(strncmp(cadeia, "for",3)==0){
                printf(", comando_reservado_for\n");
                return 0;
            };
             if(strncmp(cadeia, "for",3)==0){
                printf(", comando_reservado_for\n");
                return 0;
            };
             if(strncmp(cadeia, "for",3)==0){
                printf(", comando_reservado_for\n");
                return 0;
            };
             if(strncmp(cadeia, "for",3)==0){
                printf(", comando_reservado_for\n");
                return 0;
            };
             if(strncmp(cadeia, "for",3)==0){
                printf(", comando_reservado_for\n");
                return 0;
            };
             if(strncmp(cadeia, "for",3)==0){
                printf(", comando_reservado_for\n");
                return 0;
            };
             if(strncmp(cadeia, "for",3)==0){
                printf(", comando_reservado_for\n");
                return 0;
            };
             if(strncmp(cadeia, "for",3)==0){
                printf(", comando_reservado_for\n");
                return 0;
            };
             if(strncmp(cadeia, "for",3)==0){
                printf(", comando_reservado_for\n");
                return 0;
            };
             if(strncmp(cadeia, "for",3)==0){
                printf(", comando_reservado_for\n");
                return 0;
            };
             if(strncmp(cadeia, "for",3)==0){
                printf(", comando_reservado_for\n");
                return 0;
            };
             if(strncmp(cadeia, "for",3)==0){
                printf(", comando_reservado_for\n");
                return 0;
            };
            
            printf(", palavra reservada/n");
            return 0;



    }



    if(automato == 5){// se automato 5 entao to em caracter invalido

            int x=0;
                while(cadeia[x] != NULL || x<29){
                    printf("&c", cadeia[x]);
                    x++;
                }

                printf(", palavra invalida/n");
                return 0;



        }





}



int le_arq(char *arqler){

    // inicializa as variaveis
    char cadeia[29];
    char buffer;
    int automato = 0;
    int caracter_invalido = 0; //essas sao variaveis de controle pra saber qndo tem quebra de linha qndo o nmero eh racional etc
    int controle_cadeia = 0; 
    int controle_racional = 0;
    int controle_quebra_linha = 0;

    for(int a=0; a<29; a++){ // zero a cadeia
        cadeia[a] = NULL;
    }

    FILE *arquivo_em_leitura; //inicializa a variavel que vai pegar o arquivo a ser lido
 
    arquivo_em_leitura = fopen(arqler, "r");// variavel recebe o arquivo e o abre pra leitura

    if(arquivo_em_leitura == NULL){ // checo se o arquivo ta vazio
        printf("Arquivo Vazio.\n");
    }

    fseek(arquivo_em_leitura,1,SEEK_CUR);// abro o arquivo e aponto sua leitura para o começo doa arquivo

    fread(&buffer,1,1,arquivo_em_leitura);// variavel buffer vai receber os caracteres lidos

    seleciona_automato(buffer,automato); // a varialvel "automato" sai dessa func com o automato q eu to no momento


    checa_invalido_geral(buffer,caracter_invalido, automato, controle_racional, controle_quebra_linha , arqler);// checo o 1 caracter

    while( !feof(arquivo_em_leitura && caracter_invalido == 0) ){ // assumindo q 1 caracter eh valido

        cadeia[controle_cadeia] = buffer; // cadeia recebe o primeiro caracter
        fread(&buffer,1,1,arquivo_em_leitura);//buffer recebe o prox caracter
        controle_cadeia ++;//essa variavel controla a posicao que o caracter entra na cadeia


        //checo se o novo caracter eh valido
        checa_invalido_geral(buffer,caracter_invalido, automato, controle_racional, controle_quebra_linha, arqler);
        
        if(controle_quebra_linha==1){//se eu achei quebra de linha ou /n eu printo a cadeia e reseto ela pra receber novos caracteres

            devolvo_cadeia();//func q printa a cadeia e a qual grupo ela pertence

            for(int a=0; a<29; a++){ // zero a cadeia
                cadeia[a] = NULL;
            }

            controle_cadeia = 0; // reseto os controles
            controle_quebra_linha = 0;

        }

        if (caracter_invalido == 1){// se eu achei caracter invalido e chamo a func que trata os erros
            chamada_de_erro();
             controle_cadeia = 0; // reseto os controles
        }

        if(controle_cadeia == 29){// aqui o erro se da por atingir o tam max da cadeia
            chamada_de_erro();// tenho q passar nessa func caracter invalido e tam da cadeia, pra ele decidir se o erro eh por exceder tam ou caracter invalido
        }

    }




















}