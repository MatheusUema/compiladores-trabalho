#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int main(int argc, char *argv[]) { //Compilador


    /* Função que recebe arquivo */

    if(argc == 1) {  //Execução sem enviar arquivo txt
        printf("[ERRO] Arquivo a ser lido não enviado\n");
        return 0;
    }
    
    FILE *arquivo_entrada  = fopen(argv[1], "r");
    if (arquivo_entrada == NULL) {   
        printf("[ERRO] Arquivo não encontrado\n"); 
        return 0;
    } 

    /* Função que chama o Analisador Léxico */
    analisador_lexico(arquivo_entrada);



}

int analisador_lexico (FILE *arquivo){
    



}