#include <stdio.h>
#include <stdlib.h>
#include<string.h>
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
    
    /* INICIALIZAÇÃO DE VARÍAVEIS USADAS NO ANALISADOR */
    char cadeia[29];
    char buffer;
    int automato = 0;
    int caracter_valido = 1; //essas sao variaveis de controle pra saber qndo tem quebra de linha qndo o nmero eh racional etc
    int controle_cadeia = 0; 
    int controle_arquivo = 0;
    int controle_racional = 0;
    int controle_quebra_linha = 0;
    int acerto = 1;

    for(int a=0; a<29; a++){ // zero a cadeia igualando ela a (null) da tabela ASCII
        cadeia[a] = 0; 
    }


    FILE *arquivo_entrada = fopen(arquivo, "r");

    FILE *arquivo_saida = fopen("saida.txt", "a"); 
    fprintf(arquivo_saida,"Analise lexica: \n");
    if (arquivo_entrada == NULL) {   
        printf("[ERRO] Arquivo não encontrado\n"); 
        return 0;
    } 
    printf("Início da análise léxica do arquivo \n\n");


    /* INÍCIO DA LEITURA DO ARQUIVO */
    while(!feof(arquivo_entrada)) {

         if(controle_cadeia == 0) {
            /* ANÁLISE PARA INÍCIO DE CADEIA */
            /* A análise de início de cadeia checa o caracter e seleciona qual o automato que será utilizado 
            e, em seguida, parte cada a Análise da cadeia, tendo o conhecimento de qual automato será utilizado */
            automato = inicio_cadeia(cadeia, arquivo_entrada, &controle_arquivo); // a varialvel "automato" sai dessa func com o automato q eu to no momento
            controle_cadeia++;
        } else {    

            /*ANÁLISE DO RESTO DA CADEIA */
            fread(&buffer,1,1,arquivo_entrada);//buffer recebe o prox caracter
            cadeia[controle_cadeia] = buffer;
            controle_cadeia++;
            caracter_valido = check_valido_geral(buffer, automato, &controle_racional);
            if (condicao_final(buffer,automato)){

                acerto = 1;
                cadeia[controle_cadeia - 1] = '\0';
                printf("condicao final atingida -- cadeia reconhecida %s \n", cadeia);
                devolve_cadeia(cadeia, automato, arquivo_saida, acerto, controle_racional);
                controle_racional = 0;
                controle_arquivo = controle_arquivo + controle_cadeia;
                controle_cadeia = 0;

            } else if (!(caracter_valido) ) {

                cadeia[controle_cadeia - 1] = '\0';
                printf("caracter invalido detectado -- cadeia reconhecida %s \n", cadeia);
                acerto = 0;
                devolve_cadeia(cadeia, automato, arquivo_saida, acerto, controle_racional);
                controle_arquivo = controle_arquivo + controle_cadeia;
                controle_arquivo = controle_arquivo - 1;
                controle_cadeia = 0;
                controle_racional = 0;

            }
        }
    }
    cadeia[controle_cadeia-1] = '\0';
    printf("fim de arquivo atingido -- cadeia reconhecida %s \n", cadeia);
    
    //REALIZAR CHECAGEM DA CADEIA FINAL

    devolve_cadeia(cadeia, automato, arquivo_saida, acerto, controle_racional);

    fclose(arquivo_entrada);
    fclose(arquivo_saida);
    return 0;

}

int seleciona_automato(char caract){ 
    //analisa qual automato será utilizado a partir do caracter reconhecido.

    if (confere_numero (caract)){ //Automato de números reais e inteiros
        printf("Automato de numeros selecionado \n");
        return 1;
    }

    if(confere_letra(caract)){ //Autômato para identificadores e palavras reservadas
        printf("Automato de identificadores ou palavras reservadas selecionado \n");
        return 2;
    }

    if (caract == 123){ //Autômato para comentários
        printf("Automato de comentários selecionado \n");
        return 3;
    }

    if (confere_reservado(caract)){ //Autômato para símbolos reservados
        printf("Automato de simbolos reservados selecionado \n");
        return 4;
    }
        
    if(confere_branco(caract)){ //Checagem para espaços brancos e quebras de linha.
        return 5;
    }
    
    return 0;
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

int confere_reservado (char caract) {
    //Confere se caracter está dentro da tabela de símbolos reservados;
    char simbolos_reservados [14] = {'=', '<', '>', '+', '-', '*', '/', ':', ';', ',', '.', '(', ')', '}'};
    for (int i = 0; i < 14; i++) {
        if(caract == simbolos_reservados[i]) return 1;
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

int check_valido_geral (char buffer, int automato, int* racional) {
    if (automato == 1){ //Automato de numeros
        /* Checa se é número ou, caso seja racional
        se existe apenas um único . na cadeia */
        if(confere_numero(buffer)){
             return 1;
        } else if( (buffer == '.')) {
            *racional = *racional + 1;
            if(*racional >= 2) return 0;
            return 1;
        }
        return 0;
    } 

    if (automato == 2){ //Automato de identificadores reservadas
        /* Para palavras e identificados reservados
        qualquer caracter que não seja letra retorna invalido para esse caso */
        return confere_letra(buffer);
    }
    if (automato == 3){ //Automato para comentários
        /* Checa se eh quebra de linha ou final de arquivo*/
        if(buffer == 10 || buffer == 0) return 0;
        return 1;
    }

    return 0;
}

int inicio_cadeia (char cadeia[], FILE* arquivo, int* controle_arquivo){
    //Limpa a cadeia
    for(int a=0; a<29; a++){ // zero a cadeia igualando ela a (null) da tabela ASCII
        cadeia[a] = 0; 
    }
    //Inicia cadeia e seleciona o automato que será utilizado
    fseek(arquivo, *controle_arquivo, SEEK_SET);
    fread(&cadeia[0], 1, 1, arquivo);
    while (confere_branco(cadeia[0]) && !feof(arquivo)) {
        fread(&cadeia[0], 1, 1, arquivo);
        *controle_arquivo++;
    }
    printf("Analisando caracter %c \n", cadeia[0]);
    return seleciona_automato(cadeia[0]);

}

int devolve_cadeia(char cadeia [], int automato, FILE* saida, int acerto,int racional){
    fflush(saida);
    if (automato == 1){
        if(acerto && !racional){
            fprintf(saida, "%s, num_int \n", cadeia);
            return 1;
        } else if (acerto && racional) {
            fprintf(saida, "%s, num_real \n", cadeia);
            return 1;
        } else if (!acerto) {
            fprintf(saida, "%s, num_invalido \n", cadeia);
            return 1;
        }
    }
    if(automato == 3) {
        if (acerto) {
            fprintf(saida, "%s, comentario \n", cadeia);
            return 1;

        } else {
            fprintf(saida, "%s, comentario_nao_fechado \n", cadeia);
            return 1;
        }
    }

    if (automato == 4) {
        if(acerto){
            fprintf(saida, "%s, simbolo \n", cadeia);
            return 1;
        } else {
            fprintf(saida, "%s, simbolo_nao_reconhecido\n", cadeia);
            return 1;
        }
    }
}

int condicao_final(char buffer, int automato) {
    if (automato == 1){
        if(confere_branco(buffer)) return 1;
        if(confere_reservado(buffer) && buffer != '.') return 1;
    }
    if (automato == 3){
        if(buffer == '}') return 1;

    }
    return 0;
}
