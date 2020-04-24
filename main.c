#include <stdio.h>
#include <stdlib.h>
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
     if ((caract >= 65 && caract <= 90) || (caract >= 97 && caract <= 122)){
        return 1;
    } 
    return 0;
}