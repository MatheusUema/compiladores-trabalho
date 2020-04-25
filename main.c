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

int confere_branco (char caract) {
     if (caract == 32 || caract == 10){
        return 1;
    } 
    return 0;
}
void checa_invalido_num(char caract, int controle, int racional,int quebra_linha){
    if(confere_numero(caract) || caract == 46){ // se numero ou ponto
        if(caract == 46){
            racional = 1;
        }
        controle = 0;
    }else{
        if(confere_branco(caract)){
            quebra_linha = 1;
        }else{
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
    FILE *teste_fim_arq;
    teste_fim_arq = fopen(arquivo, "r");

    if(!feof(teste_fim_arq)){
        controle = 0;
    }else{if(confere_branco(caract)){
            quebra_linha = 1;
        }else{
            controle = 1;
        }
        
    }

}

void checa_invalido_reservado(char caract, int controle);

int seleciona_automato(char caract, int controle){ 

    if (confere_numero (caract)){
        controle = 1;
        return 0;
    }

    if(confere_letra(caract)){
        controle = 2;
        return 0;
    }

    if (caract == 123){
        controle = 3;
        return 0;
    }

    if (confere_reservado(caract)){
        controle = 4;
        return 0;
    }
        
    if(confere_branco(caract) == 0){
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
        checa_invalido_palavra(caract,controle,quebra_linha);
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

    if(automato == 1){
        if(racional == 1){
            int x=0;
            while(cadeia[x] != NULL || x<29){
                printf("&c", cadeia[x]);
                x++;
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


    if(automato == 2){

        // precisa identificar se eh comando ou variael

         int x=0;
            while(cadeia[x] != NULL || x<29){
                printf("&c", cadeia[x]);
                x++;
            }

            printf(", comando/n");
            return 0;


    }






}




int le_arq(char *arqler){

    char cadeia[29];
    char buffer;
    int automato = 0;
    int caracter_invalido = 0;
    int controle_cadeia = 0;
    int controle_racional = 0;
    int controle_quebra_linha = 0;

    for(int a=0; a<29; a++){ // zero a cadeia
        cadeia[a] = NULL;
    }

    FILE *arquivo_em_leitura;
 
    arquivo_em_leitura = fopen(arqler, "r");

    if(arquivo_em_leitura == NULL){
        printf("Arquivo Vazio.\n");
    }

    fseek(arquivo_em_leitura,1,SEEK_CUR);

    fread(&buffer,1,1,arquivo_em_leitura);

    seleciona_automato(buffer,automato); // varialvel automato sai dessa func com o automato q eu to no momento


    checa_invalido_geral(buffer,caracter_invalido, automato, controle_racional, controle_quebra_linha , arqler);

    while( !feof(arquivo_em_leitura) ){ // assumindo q 1 caracter eh valido

        cadeia[controle_cadeia] = buffer;
        fread(&buffer,1,1,arquivo_em_leitura);
        controle_cadeia ++;

        checa_invalido_geral(buffer,caracter_invalido, automato, controle_racional, controle_quebra_linha, arqler);
        
        if(controle_quebra_linha==1){

            devolvo_cadeia();

            for(int a=0; a<29; a++){ // zero a cadeia
                cadeia[a] = NULL;
            }

            controle_cadeia = 0;
            controle_quebra_linha = 0;

        }

        if (caracter_invalido == 1){
            chamada_de_erro();
        }

    }




















}