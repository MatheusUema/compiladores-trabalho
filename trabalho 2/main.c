#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#define token_size 50


int main(int argc, char *argv[]) { //Compilador

    /* Função que recebe arquivo */
    if(argc == 1) {  //Execução sem enviar arquivo txt
        printf("[ERRO] Arquivo a ser lido não enviado\n");
        return 0;
    }

    //Abertura do arquivo
    FILE *arquivo_entrada = fopen(argv[1], "r");
    FILE *arquivo_saida = fopen("saida.txt", "w"); 


    if (arquivo_entrada == NULL) {   
        printf("[ERRO] Arquivo não encontrado\n"); 
        return 0;
    } 

    /* Função que chama o Analisador Léxico */
    int controle_arquivo = 0; //Indicador da posição do ponteiro no arquivo
    int numero_linha = 1; //Indicador da linha do arquivo
    char* token; //Token do analisador léxico 

    while(!feof(arquivo_entrada)){
        token = analisador_lexico(arquivo_entrada, arquivo_saida, &controle_arquivo, &numero_linha);
        printf("token: %s \n", token);
        // printf("linha: %d \n", numero_linha);
        free(token);
    }

    fclose(arquivo_entrada);
    fclose(arquivo_saida);
}

char* analisador_lexico (FILE *arquivo_entrada, FILE *arquivo_saida, int* controle_arq, int* numero_linha){
    
    /* INICIALIZAÇÃO DE VARÍAVEIS USADAS NO ANALISADOR */
    char* token;

    char cadeia[29];
    char buffer;
    int automato = 0;
    int caracter_valido = 1; //essas sao variaveis de controle pra saber qndo tem quebra de linha qndo o nmero eh racional etc
    int controle_cadeia = 0; 
    int controle_arquivo = *controle_arq;
    int controle_racional = 0;
    int controle_linha = *numero_linha;
    int acerto = 1;

    for(int a=0; a<29; a++){ // zero a cadeia igualando ela a (null) da tabela ASCII
        cadeia[a] = 0; 
    }

    // printf("Início da análise léxica do arquivo \n\n");


    /* INÍCIO DA LEITURA DO ARQUIVO */
    while(!feof(arquivo_entrada)) {

         if(controle_cadeia == 0) {
            /* ANÁLISE PARA INÍCIO DE CADEIA */
            /* A análise de início de cadeia checa o caracter e seleciona qual o automato que será utilizado 
            e, em seguida, parte cada a Análise da cadeia, tendo o conhecimento de qual automato será utilizado */
            automato = inicio_cadeia(cadeia, arquivo_entrada, &controle_arquivo, &controle_linha); // a varialvel "automato" sai dessa func com o automato q eu to no momento
            controle_cadeia++;
        } else {    

            /*ANÁLISE DO RESTO DA CADEIA */
            fread(&buffer,1,1,arquivo_entrada);//buffer recebe o prox caracter

            if(buffer == 10) controle_linha += 1; //Contador de linhas 

            cadeia[controle_cadeia] = buffer;
            controle_cadeia++;
            caracter_valido = check_valido_geral(buffer, automato, &controle_racional);
            if (condicao_final(buffer,automato)){

                acerto = 1;
                if(automato != 3) {
                    cadeia[controle_cadeia - 1] = '\0';
                }
                // printf("condicao final atingida -- cadeia reconhecida %s \n", cadeia);
                token = devolve_cadeia(cadeia, automato, acerto, controle_racional, arquivo_saida, controle_linha);

                controle_racional = 0;
                controle_arquivo = controle_arquivo + controle_cadeia;

                if(automato != 3) {
                    controle_arquivo = controle_arquivo - 1;
                }                
                controle_cadeia = 0;
                break;

            } else if (!(caracter_valido) ) {

                cadeia[controle_cadeia - 1] = '\0';
                // printf("caracter invalido detectado -- cadeia reconhecida %s \n", cadeia);
                acerto = 0;
                token =  devolve_cadeia(cadeia, automato, acerto, controle_racional, arquivo_saida, controle_linha);

                controle_arquivo = controle_arquivo + controle_cadeia;
                controle_arquivo = controle_arquivo - 1;
                controle_cadeia = 0;
                controle_racional = 0;
                
                break;

            }
        }
    }
    cadeia[controle_cadeia-1] = '\0';
    *controle_arq = controle_arquivo;
    *numero_linha = controle_linha;
    // printf("fim de arquivo atingido -- cadeia reconhecida %s \n", cadeia);
    
    //REALIZAR CHECAGEM DA CADEIA FINAL
    if(feof(arquivo_entrada)) token = devolve_cadeia(cadeia, automato, acerto, controle_racional, arquivo_saida, controle_linha);


    return token;

}

int seleciona_automato(char caract){ 
    //analisa qual automato será utilizado a partir do caracter reconhecido.

    if (confere_numero (caract)){ //Automato de números reais e inteiros
        // printf("Automato de numeros selecionado \n");
        return 1;
    }

    if(confere_letra(caract)){ //Autômato para identificadores e palavras reservadas
        // printf("Automato de identificadores ou palavras reservadas selecionado \n");
        return 2;
    }

    if (caract == 123){ //Autômato para comentários
        // printf("Automato de comentários selecionado \n");
        return 3;
    }

    if (confere_reservado(caract) || !(confere_branco(caract))){ //Autômato para símbolos reservados
        // printf("Automato de simbolos reservados selecionado \n");
        return 4;
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
        if(confere_letra(buffer) || confere_numero(buffer)) return 1;
        return 0;
    }
    if (automato == 3){ //Automato para comentários
        /* Checa se eh quebra de linha ou final de arquivo*/
        if(buffer == 10 || buffer == 0) return 0;
        return 1;
    }

    if(automato == 4) {
        if (confere_reservado(buffer) || !(confere_branco(buffer))) return 1;
    }

    return 0;
}

int inicio_cadeia (char cadeia[], FILE* arquivo, int* controle_arquivo, int* controle_linha){
    

    //Limpa a cadeia
    for(int a=0; a<29; a++){ // zero a cadeia igualando ela a (null) da tabela ASCII
        cadeia[a] = 0; 
    }
    //Inicia cadeia e seleciona o automato que será utilizado
    fseek(arquivo, *controle_arquivo, SEEK_SET);
    fread(&cadeia[0], 1, 1, arquivo);
    while (confere_branco(cadeia[0]) && !feof(arquivo)) {
        fread(&cadeia[0], 1, 1, arquivo);
        if(cadeia[0] == 10) *controle_linha += 1; //Contador de linhas 

        *controle_arquivo = *controle_arquivo + 1; //Contador da posição do ponteiro no arquivo
    }
    // printf("Analisando caracter %c \n", cadeia[0]);
    return seleciona_automato(cadeia[0]);

}

char* devolve_cadeia(char cadeia [], int automato, int acerto,int racional, FILE* saida, int num_linha){
    char * token = (char*) malloc( (token_size) * sizeof(char));
    
    if (automato == 1){
        if(acerto && !racional){
            strncpy( token, "num_int", token_size);
            return token;

        } else if (acerto && racional) {
            strncpy( token, "num_real", token_size);
            return token;
        } else if (!acerto) {
            strncpy( token, "num_invalido", token_size);
            fprintf( saida, "Erro léxico na linha %d: num_invalido \n", num_linha);
            return token;
        }
    }

    if (automato == 2) {
        if(strcmp(cadeia, "program")==0){
                strncpy( token, "comando_reservado_program", token_size);
                return token;
            };

             if(strcmp(cadeia, "begin")==0){
                strncpy( token, "comando_reservado_begin", token_size);
                return token;
            }; 
            
            if(strcmp(cadeia, "end")==0){
                strncpy( token, "comando_reservado_end", token_size);
                return token;
            }; 
            
            if(strcmp(cadeia, "const")==0){
                strncpy( token, "comando_reservado_const", token_size);
                return token;
            }; 
            
            if(strcmp(cadeia, "var")==0){
                strncpy( token, "comando_reservado_var", token_size);
                return token;
            }; 
            
            if(strcmp(cadeia, "real")==0){
                strncpy( token, "comando_reservado_real", token_size);
                return token;
            };
            
             if(strcmp(cadeia, "integer")==0){
                strncpy( token, "comando_reservado_integer", token_size);
                return token;
            }; 
            
            if(strcmp(cadeia, "procedure")==0){
                strncpy( token, "comando_reservado_procedure", token_size);
                return token;
            }; 
            
            if(strcmp(cadeia, "else")==0){
                strncpy( token, "comando_reservado_else", token_size);
                return token;
            }; 
            
            if(strcmp(cadeia, "read")==0){
                strncpy( token, "comando_reservado_read", token_size);
                return token;
            }; 
            
            if(strcmp(cadeia, "write")==0){
                strncpy( token, "comando_reservado_write", token_size);
                return token;
            };
            
             if(strcmp(cadeia, "while")==0){
                strncpy( token, "comando_reservado_while", token_size);
                return token;
            };
            
             if(strcmp(cadeia, "do")==0){
                strncpy( token, "comando_reservado_do", token_size);
                return token;
            };
            
            
            if(strcmp(cadeia, "if")==0){
                strncpy( token, "comando_reservado_if", token_size);
                return token;
            }; 
            
            if(strcmp(cadeia, "for")==0){
                strncpy( token, "comando_reservado_for", token_size);
                return token;
            };
            // fprintf(saida, "%s, identificador\n", cadeia);
            strncpy( token, "identificador", token_size);
            return token;
    }

    if(automato == 3) {
        if (acerto) {
            strncpy( token, "comentario", token_size);
                return token;

        } else {
            strncpy( token, "comentario_nao_fechado", token_size);
            return token;
        }
    }

    if (automato == 4) {
          
        if(acerto && strlen(cadeia)<3){
            
 
            if(strncmp(cadeia, "=",1)==0 && cadeia[1] == '\0'){
                strncpy( token, "comando_reservado_igual", token_size);
                return token;
            };

             if(strncmp(cadeia, "<", 1)==0 ){
                
                  
                 if(strcmp(cadeia, "<>" )==0 ){
                    strncpy( token, "comando_reservado_diferente", token_size);
                    return token;
                 }

                 if(strcmp(cadeia, "<=")==0){
                    strncpy( token, "comando_reservado_menor_igual", token_size);
                    return token;
                 }
                 if( cadeia[1] == '\0'){
                    strncpy( token, "comando_reservado_menor", token_size);
                    return token;
                 }
                
            };


             if(strncmp(cadeia, ">",1)==0){
                 if(strncmp(cadeia, ">=",2)==0 ){
                    strncpy( token, "comando_reservado_maior_igual", token_size);
                    return token;
                 }
                 if( cadeia[1] == '\0'){
                    strncpy( token, "comando_reservado_maior", token_size);
                    return token;
                 }
               
            };

             if(strncmp(cadeia, ":",1)==0){
                 if(strncmp(cadeia, ":=",2)==0){
                    strncpy( token, "comando_reservado_atribuicao", token_size);
                return token;
                 }
                  if( cadeia[1] == '\0'){
                    strncpy( token, "comando_reservado_dois_pontos", token_size);
                    return token;
                 }
                
            };


             if(strcmp(cadeia, "+")==0 && cadeia[1] == '\0'){
                strncpy( token, "comando_reservado_soma", token_size);
                return token;
            };

             if(strncmp(cadeia, "-",1)==0 && cadeia[1] == '\0'){
                strncpy( token, "comando_reservado_subtracao", token_size);
                return token;
            };

             if(strncmp(cadeia, "*",1)==0 && cadeia[1] == '\0'){
                strncpy( token, "comando_reservado_multiplicacao", token_size);
                return token;
            };

             if(strncmp(cadeia, "/",1)==0 && cadeia[1] == '\0'){
                strncpy( token, "comando_reservado_divisao", token_size);
                return token;
            };

             if(strncmp(cadeia, ";",1)==0 && cadeia[1] == '\0'){
                strncpy( token, "comando_reservado_ponto_virgula", token_size);
                return token;
            };

             if(strncmp(cadeia, ".",1)==0 && cadeia[1] == '\0'){
                strncpy( token, "comando_reservado_ponto", token_size);
                return token;
            };

             if(strncmp(cadeia, ",",1)==0 && cadeia[1] == '\0'){
                strncpy( token, "comando_reservado_virgula", token_size);
                return token;
            };

             if(strncmp(cadeia, "(",1)==0 && cadeia[1] == '\0'){
                strncpy( token, "comando_reservado_abre_parenteses", token_size);
                return token;
            };
            
            if(strncmp(cadeia, ")",1)==0 && cadeia[1] == '\0'){
                strncpy( token, "comando_reservado_fecha_parenteses", token_size);
                return token;
            };
            strncpy( token, "simbolo_nao_reconhecido", token_size);
            fprintf( saida, "Erro léxico na linha %d: identificador mal formado \n", num_linha);
            return token;
        } else {
            strncpy( token, "simbolo_nao_reconhecido", token_size);
            fprintf( saida, "Erro léxico na linha %d: identificador mal formado \n", num_linha);
            return token;
        }
    }
}

int condicao_final(char buffer, int automato) {
    if (automato == 1){
        if(confere_branco(buffer)) return 1;
        if(confere_reservado(buffer) && buffer != '.') return 1;
        return 0;
    }

    if(automato == 2) {
        if(confere_branco(buffer)) return 1;
        if(confere_reservado(buffer)) return 1;
        return 0;
    }

    if (automato == 3){
        if(buffer == '}') return 1;
        return 0;
    }

    if(automato==4){
        if(!confere_reservado(buffer)) return 1;
    }
    return 0;
}