#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#define token_size 50

int main(int argc, char *argv[])
{ //Compilador

    /* Função que recebe arquivo */
    if (argc == 1)
    { //Execução sem enviar arquivo txt
        printf("[ERRO] Arquivo a ser lido não enviado\n");
        return 0;
    }

    //Abertura do arquivo
    FILE *arquivo_entrada = fopen(argv[1], "r");
    FILE *arquivo_saida = fopen("saida.txt", "w");

    if (arquivo_entrada == NULL)
    {
        printf("[ERRO] Arquivo não encontrado\n");
        return 0;
    }

    /* Função que chama o Analisador Léxico */
    int controle_arquivo = 0; //Indicador da posição do ponteiro no arquivo
    int numero_linha = 1;     //Indicador da linha do arquivo
    char *token;              //Token do analisador léxico

    // token = analisador_lexico(arquivo_entrada, arquivo_saida, &controle_arquivo, &numero_linha);
    // while (!feof(arquivo_entrada))
    // {
    //     token = analisador_lexico(arquivo_entrada, arquivo_saida, &controle_arquivo, &numero_linha);
    // printf("token: %s \n", token);
    programa(token, arquivo_entrada, arquivo_saida, &controle_arquivo, &numero_linha);

    fclose(arquivo_entrada);
    fclose(arquivo_saida);
}

char *analisador_lexico(FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha)
{

    /* INICIALIZAÇÃO DE VARÍAVEIS USADAS NO ANALISADOR */
    char *token;

    char cadeia[29];
    char buffer;
    int automato = 0;
    int caracter_valido = 1; //essas sao variaveis de controle pra saber qndo tem quebra de linha qndo o nmero eh racional etc
    int controle_cadeia = 0;
    int controle_arquivo = *controle_arq;
    int controle_racional = 0;
    int controle_linha = *numero_linha;
    int acerto = 1;

    for (int a = 0; a < 29; a++)
    { // zero a cadeia igualando ela a (null) da tabela ASCII
        cadeia[a] = 0;
    }

    // printf("Início da análise léxica do arquivo \n\n");

    /* INÍCIO DA LEITURA DO ARQUIVO */
    while (!feof(arquivo_entrada))
    {

        if (controle_cadeia == 0)
        {
            /* ANÁLISE PARA INÍCIO DE CADEIA */
            /* A análise de início de cadeia checa o caracter e seleciona qual o automato que será utilizado 
            e, em seguida, parte cada a Análise da cadeia, tendo o conhecimento de qual automato será utilizado */
            automato = inicio_cadeia(cadeia, arquivo_entrada, &controle_arquivo, &controle_linha); // a varialvel "automato" sai dessa func com o automato q eu to no momento
            // printf("cadeia inicial -- cadeia reconhecida %s \n", cadeia);
            controle_cadeia++;
        }
        else
        {
            // printf("analise de resto de cadeia -- cadeia reconhecida %s \n", cadeia);

            /*ANÁLISE DO RESTO DA CADEIA */
            fread(&buffer, 1, 1, arquivo_entrada); //buffer recebe o prox caracter
            // printf("analise de resto de cadeia -- prox char %c \n", buffer);

            if (buffer == 10) controle_linha += 1; //Contador de linhas

            cadeia[controle_cadeia] = buffer;
            controle_cadeia++;
            caracter_valido = check_valido_geral(buffer, automato, &controle_racional);
            if (condicao_final(buffer, automato))
            {

                acerto = 1;
                if (automato != 3)
                {
                    cadeia[controle_cadeia - 1] = '\0';
                }
                // printf("condicao final atingida -- cadeia reconhecida %s \n", cadeia);
                token = devolve_cadeia(cadeia, automato, acerto, controle_racional, arquivo_saida, controle_linha);

                controle_racional = 0;
                controle_arquivo = controle_arquivo + controle_cadeia;

                if (automato != 3)
                {
                    controle_arquivo = controle_arquivo - 1;
                }
                controle_cadeia = 0;
                break;
            }
            else if (!(caracter_valido))
            {

                cadeia[controle_cadeia - 1] = '\0';
                // printf("caracter invalido detectado -- cadeia reconhecida %s \n", cadeia);
                acerto = 0;
                token = devolve_cadeia(cadeia, automato, acerto, controle_racional, arquivo_saida, controle_linha);

                controle_arquivo = controle_arquivo + controle_cadeia;
                controle_arquivo = controle_arquivo - 1;
                controle_cadeia = 0;
                controle_racional = 0;

                break;
            }
        }
    }
    cadeia[controle_cadeia - 1] = '\0';
    *controle_arq = controle_arquivo;
    *numero_linha = controle_linha;
    // printf("fim de arquivo atingido -- cadeia reconhecida %s \n", cadeia);

    //REALIZAR CHECAGEM DA CADEIA FINAL
    if (feof(arquivo_entrada))
        token = devolve_cadeia(cadeia, automato, acerto, controle_racional, arquivo_saida, controle_linha);

    return token;
}

int seleciona_automato(char caract)
{
    //analisa qual automato será utilizado a partir do caracter reconhecido.

    if (confere_numero(caract))
    { //Automato de números reais e inteiros
        // printf("Automato de numeros selecionado \n");
        return 1;
    }

    if (confere_letra(caract))
    { //Autômato para identificadores e palavras reservadas
        // printf("Automato de identificadores ou palavras reservadas selecionado \n");
        return 2;
    }

    if (caract == 123)
    { //Autômato para comentários
        // printf("Automato de comentários selecionado \n");
        return 3;
    }

    if (confere_reservado(caract) || !(confere_branco(caract)))
    { //Autômato para símbolos reservados
        // printf("Automato de simbolos reservados selecionado \n");
        return 4;
    }

    return 0;
}

int confere_numero(char caract)
{
    //Confere se o caractere é numero, checando seu valor na tabela ASCII
    if (caract >= 48 && caract <= 57)
    {
        return 1;
    }
    return 0;
}

int confere_letra(char caract)
{
    //Confere se caracter é letra minúscula ou maiúscula
    if ((caract >= 65 && caract <= 90) || (caract >= 97 && caract <= 122))
    {
        return 1;
    }
    return 0;
}

int confere_reservado(char caract)
{
    //Confere se caracter está dentro da tabela de símbolos reservados;
    char simbolos_reservados[14] = {'=', '<', '>', '+', '-', '*', '/', ':', ';', ',', '.', '(', ')', '}'};
    for (int i = 0; i < 14; i++)
    {
        if (caract == simbolos_reservados[i])
            return 1;
    }
    return 0;
}

int confere_branco(char caract)
{
    //Confere se caracter checado é espaço ou quebra de linha;
    if (caract == 32 || caract == 10)
    {
        return 1;
    }
    return 0;
}

int check_valido_geral(char buffer, int automato, int *racional)
{

    if (automato == 1)
    { //Automato de numeros
        /* Checa se é número ou, caso seja racional
        se existe apenas um único . na cadeia */
        if (confere_numero(buffer))
        {
            return 1;
        }
        else if ((buffer == '.'))
        {
            *racional = *racional + 1;
            if (*racional >= 2)
                return 0;
            return 1;
        }
        return 0;
    }

    if (automato == 2)
    { //Automato de identificadores reservadas
        /* Para palavras e identificados reservados
        qualquer caracter que não seja letra retorna invalido para esse caso */
        if (confere_letra(buffer) || confere_numero(buffer))
            return 1;
        return 0;
    }
    if (automato == 3)
    { //Automato para comentários
        /* Checa se eh quebra de linha ou final de arquivo*/
        if (buffer == 10 || buffer == 0)
            return 0;
        return 1;
    }

    if (automato == 4)
    {
        if (confere_reservado(buffer) || !(confere_branco(buffer)))
            return 1;
    }

    return 0;
}

int inicio_cadeia(char cadeia[], FILE *arquivo, int *controle_arquivo, int *controle_linha)
{

    //Limpa a cadeia
    for (int a = 0; a < 29; a++)
    { // zero a cadeia igualando ela a (null) da tabela ASCII
        cadeia[a] = 0;
    }
    //Inicia cadeia e seleciona o automato que será utilizado
    fseek(arquivo, *controle_arquivo, SEEK_SET);
    fread(&cadeia[0], 1, 1, arquivo);
    while (confere_branco(cadeia[0]) && !feof(arquivo))
    {
        fread(&cadeia[0], 1, 1, arquivo);
        if (cadeia[0] == 10)
            *controle_linha += 1; //Contador de linhas

        *controle_arquivo = *controle_arquivo + 1; //Contador da posição do ponteiro no arquivo
    }
    // printf("Analisando caracter %c \n", cadeia[0]);
    return seleciona_automato(cadeia[0]);
}

char *devolve_cadeia(char cadeia[], int automato, int acerto, int racional, FILE *saida, int num_linha)
{
    char *token = (char *)malloc((token_size) * sizeof(char));

    if (automato == 1)
    {
        if (acerto && !racional)
        {
            strncpy(token, "num_int", token_size);
            return token;
        }
        else if (acerto && racional)
        {
            strncpy(token, "num_real", token_size);
            return token;
        }
        else if (!acerto)
        {
            strncpy(token, "num_invalido", token_size);
            fprintf(saida, "Erro léxico na linha %d: num_invalido \n", num_linha);
            return token;
        }
    }

    if (automato == 2)
    {
        if (strcmp(cadeia, "program") == 0)
        {
            strncpy(token, "comando_reservado_program", token_size);
            return token;
        };

        if (strcmp(cadeia, "begin") == 0)
        {
            strncpy(token, "comando_reservado_begin", token_size);
            return token;
        };

        if (strcmp(cadeia, "end") == 0)
        {
            strncpy(token, "comando_reservado_end", token_size);
            return token;
        };

        if (strcmp(cadeia, "const") == 0)
        {
            strncpy(token, "comando_reservado_const", token_size);
            return token;
        };

        if (strcmp(cadeia, "var") == 0)
        {
            strncpy(token, "comando_reservado_var", token_size);
            return token;
        };

        if (strcmp(cadeia, "real") == 0)
        {
            strncpy(token, "comando_reservado_real", token_size);
            return token;
        };

        if (strcmp(cadeia, "integer") == 0)
        {
            strncpy(token, "comando_reservado_integer", token_size);
            return token;
        };

        if (strcmp(cadeia, "procedure") == 0)
        {
            strncpy(token, "comando_reservado_procedure", token_size);
            return token;
        };

        if (strcmp(cadeia, "else") == 0)
        {
            strncpy(token, "comando_reservado_else", token_size);
            return token;
        };

        if (strcmp(cadeia, "read") == 0)
        {
            strncpy(token, "comando_reservado_read", token_size);
            return token;
        };

        if (strcmp(cadeia, "write") == 0)
        {
            strncpy(token, "comando_reservado_write", token_size);
            return token;
        };

        if (strcmp(cadeia, "while") == 0)
        {
            strncpy(token, "comando_reservado_while", token_size);
            return token;
        };

        if (strcmp(cadeia, "do") == 0)
        {
            strncpy(token, "comando_reservado_do", token_size);
            return token;
        };

        if (strcmp(cadeia, "to") == 0)
        {
            strncpy(token, "comando_reservado_to", token_size);
            return token;
        };

        if (strcmp(cadeia, "if") == 0)
        {
            strncpy(token, "comando_reservado_if", token_size);
            return token;
        };

        if (strcmp(cadeia, "for") == 0)
        {
            strncpy(token, "comando_reservado_for", token_size);
            return token;
        };
        // fprintf(saida, "%s, identificador\n", cadeia);
        strncpy(token, "identificador", token_size);
        return token;
    }

    if (automato == 3)
    {
        if (acerto)
        {
            strncpy(token, "comentario", token_size);
            return token;
        }
        else
        {
            strncpy(token, "comentario_nao_fechado", token_size);
            return token;
        }
    }

    if (automato == 4)
    {

        if (acerto && strlen(cadeia) < 3)
        {

            if (strncmp(cadeia, "=", 1) == 0 && cadeia[1] == '\0')
            {
                strncpy(token, "comando_reservado_igual", token_size);
                return token;
            };

            if (strncmp(cadeia, "<", 1) == 0)
            {

                if (strcmp(cadeia, "<>") == 0)
                {
                    strncpy(token, "comando_reservado_diferente", token_size);
                    return token;
                }

                if (strcmp(cadeia, "<=") == 0)
                {
                    strncpy(token, "comando_reservado_menor_igual", token_size);
                    return token;
                }
                if (cadeia[1] == '\0')
                {
                    strncpy(token, "comando_reservado_menor", token_size);
                    return token;
                }
            };

            if (strncmp(cadeia, ">", 1) == 0)
            {
                if (strncmp(cadeia, ">=", 2) == 0)
                {
                    strncpy(token, "comando_reservado_maior_igual", token_size);
                    return token;
                }
                if (cadeia[1] == '\0')
                {
                    strncpy(token, "comando_reservado_maior", token_size);
                    return token;
                }
            };

            if (strncmp(cadeia, ":", 1) == 0)
            {
                if (strncmp(cadeia, ":=", 2) == 0)
                {
                    strncpy(token, "comando_reservado_atribuicao", token_size);
                    return token;
                }
                if (cadeia[1] == '\0')
                {
                    strncpy(token, "comando_reservado_dois_pontos", token_size);
                    return token;
                }
            };

            if (strcmp(cadeia, "+") == 0 && cadeia[1] == '\0')
            {
                strncpy(token, "comando_reservado_soma", token_size);
                return token;
            };

            if (strncmp(cadeia, "-", 1) == 0 && cadeia[1] == '\0')
            {
                strncpy(token, "comando_reservado_subtracao", token_size);
                return token;
            };

            if (strncmp(cadeia, "*", 1) == 0 && cadeia[1] == '\0')
            {
                strncpy(token, "comando_reservado_multiplicacao", token_size);
                return token;
            };

            if (strncmp(cadeia, "/", 1) == 0 && cadeia[1] == '\0')
            {
                strncpy(token, "comando_reservado_divisao", token_size);
                return token;
            };

            if (strncmp(cadeia, ";", 1) == 0 && cadeia[1] == '\0')
            {
                strncpy(token, "comando_reservado_ponto_virgula", token_size);
                return token;
            };

            if (strncmp(cadeia, ".", 1) == 0 && cadeia[1] == '\0')
            {
                strncpy(token, "comando_reservado_ponto", token_size);
                return token;
            };

            if (strncmp(cadeia, ",", 1) == 0 && cadeia[1] == '\0')
            {
                strncpy(token, "comando_reservado_virgula", token_size);
                return token;
            };

            if (strncmp(cadeia, "(", 1) == 0 && cadeia[1] == '\0')
            {
                strncpy(token, "comando_reservado_abre_parenteses", token_size);
                return token;
            };

            if (strncmp(cadeia, ")", 1) == 0 && cadeia[1] == '\0')
            {
                strncpy(token, "comando_reservado_fecha_parenteses", token_size);
                return token;
            };
            strncpy(token, "simbolo_nao_reconhecido", token_size);
            fprintf(saida, "Erro léxico na linha %d: identificador mal formado \n", num_linha);
            return token;
        }
        else
        {
            strncpy(token, "simbolo_nao_reconhecido", token_size);
            fprintf(saida, "Erro léxico na linha %d: identificador mal formado \n", num_linha);
            return token;
        }
    }
}

int condicao_final(char buffer, int automato)
{
    if (automato == 1)
    {
        if (confere_branco(buffer))
            return 1;
        if (confere_reservado(buffer) && buffer != '.')
            return 1;
        return 0;
    }

    if (automato == 2)
    {
        if (confere_branco(buffer))
            return 1;
        if (confere_reservado(buffer))
            return 1;
        return 0;
    }

    if (automato == 3)
    {
        if (buffer == '}')
            return 1;
        return 0;
    }

    if (automato == 4)
    {
        if (!confere_reservado(buffer) || buffer == ';')
            return 1;
    }
    return 0;
}

char *erro(char *token, char *token_esperado[], int size, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha)
{
    //Continua lendo token a token o arquivo e comparando com os tokens do conjunto de tokens esperados
    //Caso encontre, ele retorna o token encontrado, caso não, ele retorna o token erro

    printf("entrou na erro \n");
    while (!feof(arquivo_entrada))
    {
        for (int i = 0; i < size; i++)
        {
            if (strcmp(token, token_esperado[i]) == 0)
                return token;
        }
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }
    return "erro";
}

int programa(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha)
{
    int modo_panico = -1; // Modo pânico desativado
    token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    // printf("token: %s \n", token);

    if (strcmp(token, "comando_reservado_program") == 0)
    {
        printf("'program' lido\n");
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }
    else if (modo_panico == -1) //Só entra no tratamento de erro se o modo panico for falso
    {
        modo_panico = 1;                                                           //Muda modo panico para verdadeiro
        fprintf(arquivo_saida, "Erro sintático na linha %d: 'program' esperado\n", *numero_linha);

        //Determina quais sao os seguidores e o tamanho desse vetor de strings e chama a função de erro
        char *token_esperados[1];
        token_esperados[0] = "identificador";
        token = erro(token, token_esperados, 1, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }

    if (strcmp(token, "identificador") == 0)
    {
        int modo_panico = -1; //Desativa modo panico
        printf("'identificador' lido\n");
        free(token); //Liberar memória alocada para token sempre antes da próxima chamada da função analisador léxico
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    else if (modo_panico == -1)
    {
        modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: 'identificador' esperado\n", *numero_linha - 1);
        char *token_esperados[1];
        token_esperados[0] = "comando_reservado_ponto_virgula";
        int tamanho = sizeof(token_esperados) / 8;
        token = erro(token, token_esperados, tamanho, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }

    if (strcmp(token, "comando_reservado_ponto_virgula") == 0)
    {
        int modo_panico = -1;
        printf("';' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    else if (modo_panico == -1)
    {
        modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: ';' esperado\n", *numero_linha - 1);
        char *token_esperados[4];
        token_esperados[0] = "comando_reservado_const";
        token_esperados[1] = "comando_reservado_var";
        token_esperados[2] = "comando_reservado_procedure";
        token_esperados[3] = "comando_reservado_begin";

        int tamanho = sizeof(token_esperados) / 8;
        token = erro(token, token_esperados, tamanho, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }

    //Tratamento de erro a mais. Caso o programa termine de ler a linha "program p;" e seja sucedida por qualquer token que não seja nenhum desses quatro
    //então deve ser ativado o modo pânico (caso de erro exemplificado pelo programa_teste_3);
    if (strcmp(token, "comando_reservado_const") != 0 && strcmp(token, "comando_reservado_var") != 0 && strcmp(token, "comando_reservado_procedure") != 0 && strcmp(token, "comando_reservado_begin") != 0)
    {
        modo_panico = 1;
        fprintf(arquivo_saida,"Erro sintático na linha %d: 'const', 'var', 'procedure' ou 'begin' esperado\n", *numero_linha);
        char *token_esperados[4];
        token_esperados[0] = "comando_reservado_const";
        token_esperados[1] = "comando_reservado_var";
        token_esperados[2] = "comando_reservado_procedure";
        token_esperados[3] = "comando_reservado_begin";
        token = erro(token, token_esperados, 4, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }

    token = dc_c(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, &modo_panico);
    printf("voltei para programa()\n");
    // printf("token: %s \n", token);

    if (strcmp(token, "comando_reservado_var") != 0 && strcmp(token, "comando_reservado_procedure") != 0 && strcmp(token, "comando_reservado_begin") != 0)
    {
        modo_panico = 1;
        fprintf(arquivo_saida,"Erro sintático na linha %d: 'var', 'procedure' ou 'begin' esperado\n", *numero_linha);
        char *token_esperados[3];
        token_esperados[0] = "comando_reservado_var";
        token_esperados[1] = "comando_reservado_procedure";
        token_esperados[2] = "comando_reservado_begin";
        int tamanho = sizeof(token_esperados) / 8;
        token = erro(token, token_esperados, tamanho, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }

    char *tokens_seguidores_dc_v[2];
    tokens_seguidores_dc_v[0] = "comando_reservado_begin";
    tokens_seguidores_dc_v[1] = "comando_reservado_procedure";

    token = dc_v(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, &modo_panico, tokens_seguidores_dc_v, 2);
    printf("voltei para programa()\n");

    if (strcmp(token, "comando_reservado_procedure") != 0 && strcmp(token, "comando_reservado_begin") != 0)
    {
        modo_panico = 1;
        fprintf(arquivo_saida,"Erro sintático na linha %d: 'procedure' ou 'begin' esperado\n", *numero_linha);
        char *token_esperados[2];
        token_esperados[0] = "comando_reservado_procedure";
        token_esperados[1] = "comando_reservado_begin";
        token = erro(token, token_esperados, 2, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }

    token = dc_p(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, &modo_panico);
    printf("voltei para programa()\n");

    if (strcmp(token, "comando_reservado_begin") == 0)
    {   
        modo_panico = -1;
        printf("'begin' lido\n");
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }
    else if(modo_panico == -1)
    {
        modo_panico = 1;
        printf("Erro sintático na linha %d: 'begin' esperado\n", *numero_linha);
        char *token_esperados[7];
        token_esperados[0] = "comando_reservado_read";
        token_esperados[1] = "comando_reservado_write";
        token_esperados[2] = "comando_reservado_while";
        token_esperados[3] = "comando_reservado_if";
        token_esperados[4] = "comando_reservado_for";
        token_esperados[5] = "comando_reservado_begin";
        token_esperados[6] = "identificador";
        token = erro(token, token_esperados, 7, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
    }

    token = comandos(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, &modo_panico);

    if (strcmp(token, "comando_reservado_end") == 0)
    {   
        modo_panico = -1;
        printf("'end' lido\n");
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }
    else if (modo_panico == -1)
    {
        modo_panico = 1;
        printf("Erro sintático na linha %d: 'end' esperado\n", *numero_linha);
        char *token_esperados[1];
        token_esperados[0] = "comando_reservado_ponto";
        token = erro(token, token_esperados, 1, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
    }

    if (strcmp(token, "comando_reservado_ponto") == 0)
    {   
        modo_panico = -1;
        printf("'ponto' lido\n");
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }
    else if (modo_panico == -1)
    {
        modo_panico = 1;
        printf("Erro sintático na linha %d: 'ponto' esperado\n", *numero_linha);
    }
    printf("fim da analise\n");
    return 1;

    
}

char *dc_c(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha, int *modo_panico)
{
    printf("entrei em dc_c() \n");
    if (strcmp(token, "comando_reservado_const") != 0)
    {
        // printf("token: %s \n", token);
        printf("sai de dc_c()\n");
        return token;
    }
    else // Presumi aqui que se um simbolo não é diferente de const, ele é igual
    {
        *modo_panico = -1;
        printf("'const' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }

    if (strcmp(token, "identificador") == 0)
    {
        *modo_panico = -1;
        printf("'identificador' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: 'identificador' esperado\n", *numero_linha);
        char *token_esperados[5];
        token_esperados[0] = "comando_reservado_igual";
        token_esperados[1] = "comando_reservado_const";
        token_esperados[2] = "comando_reservado_var";
        token_esperados[3] = "comando_reservado_procedure";
        token_esperados[4] = "comando_reservado_begin";
        token = erro(token, token_esperados, 5, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }

    if (strcmp(token, "comando_reservado_igual") == 0)
    {
        *modo_panico = -1;
        printf("'=' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
    }
    else if (*modo_panico == -1)
    {
        fprintf(arquivo_saida,"Erro sintático na linha %d: '=' esperado\n", *numero_linha);
        *modo_panico = 1;
        char *token_esperados[6];
        token_esperados[0] = "num_int";
        token_esperados[1] = "num_real";
        token_esperados[2] = "comando_reservado_const";
        token_esperados[3] = "comando_reservado_var";
        token_esperados[4] = "comando_reservado_procedure";
        token_esperados[5] = "comando_reservado_begin";
        token = erro(token, token_esperados, 6, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }

    if ((strcmp(token, "num_int") == 0) || (strcmp(token, "num_real") == 0))
    {
        *modo_panico = -1;
        printf("'número' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: 'numero' esperado\n", *numero_linha - 1);
        char *token_esperados[5];
        token_esperados[0] = "comando_reservado_ponto_virgula";
        token_esperados[1] = "comando_reservado_const";
        token_esperados[2] = "comando_reservado_var";
        token_esperados[3] = "comando_reservado_procedure";
        token_esperados[4] = "comando_reservado_begin";
        token = erro(token, token_esperados, 5, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }

    if (strcmp(token, "comando_reservado_ponto_virgula") == 0)
    {
        *modo_panico = -1;
        printf("';' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: ';' esperado\n", *numero_linha - 1);
        char *token_esperados[4];
        token_esperados[0] = "comando_reservado_const";
        token_esperados[1] = "comando_reservado_var";
        token_esperados[2] = "comando_reservado_procedure";
        token_esperados[3] = "comando_reservado_begin";

        token = erro(token, token_esperados, 4, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }
    dc_c(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico);
}

char *dc_v(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha, int *modo_panico, char *tokens_seguidores[], int tamanho)
{
    printf("entrei em dc_v()\n");
    int tam_tokens = 0;
    if (strcmp(token, "comando_reservado_var") != 0)
    {
        // printf("token: %s \n", token);
        printf("sai dc_v()\n");
        return token;
    }
    else
    {
        *modo_panico = -1;
        printf("'var' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
        char *tokens_seguidores_variaveis[1];
        tokens_seguidores_variaveis[0] = "comando_reservado_dois_pontos";
        token = variaveis(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, tokens_seguidores_variaveis, 1);
    }

    if (strcmp(token, "comando_reservado_dois_pontos") == 0)
    {
        *modo_panico = -1;
        printf("':' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: ':' esperado\n", *numero_linha);
        tam_tokens = 3 + tamanho;
        char *token_esperados[tam_tokens];
        token_esperados[0] = "comando_reservado_integer";
        token_esperados[1] = "comando_reservado_real";
        token_esperados[2] = "comando_reservado_var";
        for (int i = 0; i < tamanho; i++)
        {
            token_esperados[i + 3] = tokens_seguidores[i];
        }
        token = erro(token, token_esperados, tam_tokens, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
    }
    if ((strcmp(token, "comando_reservado_integer") == 0) || (strcmp(token, "comando_reservado_real") == 0))
    {
        *modo_panico = -1;
        if ((strcmp(token, "comando_reservado_integer") == 0))
            printf("'integer' lido\n");
        else
            printf("'real' lido\n");

        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: 'real' ou 'integer' esperado\n", *numero_linha - 1);
        tam_tokens = 2 + tamanho;
        char *token_esperados[tam_tokens];
        token_esperados[0] = "comando_reservado_ponto_virgula";
        token_esperados[1] = "comando_reservado_var";
        for (int i = 0; i < tamanho; i++)
        {
            token_esperados[i + 2] = tokens_seguidores[i];
        }
        token = erro(token, token_esperados, tam_tokens, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    if (strcmp(token, "comando_reservado_ponto_virgula") == 0)
    {
        *modo_panico = -1;
        printf("';' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: ';' esperado\n", *numero_linha - 1);
        tam_tokens = 1 + tamanho;
        char *token_esperados[tam_tokens];
        token_esperados[0] = "comando_reservado_var";
        for (int i = 0; i < tamanho; i++)
        {
            token_esperados[i + 1] = tokens_seguidores[i];

        }
        token = erro(token, token_esperados, tam_tokens, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    dc_v(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, tokens_seguidores, tamanho);
}

char *variaveis(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha, int *modo_panico, char* tokens_seguidores[], int tamanho)
{
    int tam_tokens = 0;
    printf("entrei em variaveis()\n");
    if (strcmp(token, "identificador") == 0)
    {
        *modo_panico = -1;
        printf("'identificador' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: 'identificador' esperado\n", *numero_linha);
        tam_tokens = 1 + tamanho;
        char *token_esperados[tam_tokens];
        token_esperados[0] = "comando_reservado_virgula";
        for (int i = 0; i < tamanho; i++)
        {
            token_esperados[i + 1] = tokens_seguidores[i];
        }
        token = erro(token, token_esperados, tam_tokens, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    if(strcmp(token, "comando_reservado_virgula") == 0)
    {
         *modo_panico = -1;
        printf("',' lido \n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
        variaveis(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, tokens_seguidores, tamanho);
    }
    else if(strcmp(token, tokens_seguidores[0]) == 0)
    {
        *modo_panico = -1;
        printf("sai de variaveis()\n");
        return token;
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: ',' esperado\n", *numero_linha);
        token = erro(token, tokens_seguidores, tamanho, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        return token;
    }
}

char *dc_p(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha, int *modo_panico)
{

    printf("entrei em dc_p()\n");
    if (strcmp(token, "comando_reservado_procedure") != 0)
    {
        // printf("token: %s \n", token);
        return token;
    }
    else // Presumi aqui que se um simbolo não é diferente de var, ele é igual
    {
        *modo_panico = -1;
        printf("'procedure' lido \n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }

    if (strcmp(token, "identificador") == 0)
    {
        *modo_panico = -1;
        printf("'identificador' lido \n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: 'identificador' esperado\n", *numero_linha);
        char *token_esperados[3];
        token_esperados[0] = "comando_reservado_abre_parenteses";
        token_esperados[1] = "comando_reservado_procedure";
        token_esperados[2] = "comando_reservado_begin";
        token = erro(token, token_esperados, 3, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }

    //Tratamento de erros: caso não seja ; ou (
    if (strcmp(token, "comando_reservado_ponto_virgula") != 0 && strcmp(token, "comando_reservado_abre_parenteses") != 0)
    {
        *modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: ';' ou '(' esperado\n", *numero_linha);
        char *token_esperados[3];
        token_esperados[0] = "comando_reservado_var";
        token_esperados[1] = "comando_reservado_procedure";
        token_esperados[2] = "comando_reservado_begin";
        token = erro(token, token_esperados, 3, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }

    token = parametros(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico);

    if (strcmp(token, "comando_reservado_ponto_virgula") == 0)
    {
        *modo_panico = -1;
        printf("';' lido \n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: ';' esperado\n", *numero_linha - 1);
        char *token_esperados[3];
        token_esperados[0] = "comando_reservado_var";
        token_esperados[1] = "comando_reservado_procedure";
        token_esperados[2] = "comando_reservado_begin";
        token = erro(token, token_esperados, 3, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
    }

    token = corpo_p(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico);
    printf("token: %s \n", token);
    dc_p(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico);
}

char *parametros(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha, int *modo_panico)
{
    printf("entrei em parametros()\n");
    char *tokens_seguidores_variaveis[1];
    tokens_seguidores_variaveis[0] = "comando_reservado_dois_pontos";

    if (strcmp(token, "comando_reservado_abre_parenteses") != 0)
    {
        printf("sai de parametros()\n");
        // printf("token: %s\n", token);
        return token;
    }
    else // Presumi aqui que se um simbolo não é diferente de var, ele é igual
    {
        *modo_panico = -1;
        printf("'(' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }

    token = variaveis(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, tokens_seguidores_variaveis, 1);
    // printf("token: %s \n", token);

    if (strcmp(token, "comando_reservado_dois_pontos") == 0)
    {
        *modo_panico = -1;
        printf("':' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: ':' esperado\n", *numero_linha);
        char *token_esperados[3];
        token_esperados[0] = "comando_reservado_real";
        token_esperados[1] = "comando_reservado_integer";
        token_esperados[2] = "comando_reservado_ponto_virgula";
        token = erro(token, token_esperados, 3, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    if ((strcmp(token, "comando_reservado_integer") == 0) || (strcmp(token, "comando_reservado_real") == 0))
    {
        *modo_panico = -1;
        if ((strcmp(token, "comando_reservado_integer") == 0))
            printf("'integer' lido\n");
        else
            printf("'real' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: 'tipo de numero' esperado\n", *numero_linha);
        char *token_esperados[2];
        token_esperados[0] = "comando_reservado_ponto_virgula";
        token_esperados[1] = "comando_reservado_fecha_parenseteses";
        token = erro(token, token_esperados, 2, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }

    while (strcmp(token, "comando_reservado_ponto_virgula") == 0)
    {
        printf("';' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
        
        if(strcmp(token, "identificador") == 0) token = variaveis(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, tokens_seguidores_variaveis, 1);
        else if (*modo_panico == -1)
        {
            *modo_panico = 1;
            fprintf(arquivo_saida, "Erro sintático na linha %d: 'identificador' esperado\n", *numero_linha);
            char *token_esperados[4];
            token_esperados[0] = "comando_reservado_ponto_virgula";
            token_esperados[1] = "comando_reservado_fecha_parenteses";
            token_esperados[2] = "comando_reservado_var";
            token_esperados[3] = "comando_reservado_begin";
            token = erro(token, token_esperados, 4, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            break;
        }

        if (strcmp(token, "comando_reservado_dois_pontos") == 0)
        {
            *modo_panico = -1;
            printf("':' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }
        else if (*modo_panico == -1)
        {
            *modo_panico = 1;
            fprintf(arquivo_saida, "Erro sintático na linha %d: ':' esperado\n", *numero_linha);
            char *token_esperados[3];
            token_esperados[0] = "comando_reservado_real";
            token_esperados[1] = "comando_reservado_integer";
            token_esperados[2] = "comando_reservado_ponto_virgula";
            token = erro(token, token_esperados, 3, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }
        if ((strcmp(token, "comando_reservado_integer") == 0) || (strcmp(token, "comando_reservado_real") == 0))
        {
            *modo_panico = -1;
            printf("'integer ou real' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            // printf("token: %s \n", token);
        }
        else if (*modo_panico == -1)
        {
            *modo_panico = 1;
            fprintf(arquivo_saida, "Erro sintático na linha %d: 'tipo de numero' esperado\n", *numero_linha);
            char *token_esperados[2];
            token_esperados[0] = "comando_reservado_ponto_virgula";
            token_esperados[1] = "comando_reservado_fecha_parenteses";
            token = erro(token, token_esperados, 2, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            if(strcmp(token, "comando_reservado_fecha_parenteses") == 0) break; //caso tenha faltando 'real' ou 'integer', e colocou ')', tem que sair do loop
        }

    }

    if (strcmp(token, "comando_reservado_fecha_parenteses") == 0)
    {
        *modo_panico = -1;
        printf("')' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: ')' esperado\n", *numero_linha);
        char *token_esperados[1];
        token_esperados[0] = "comando_reservado_ponto_virgula";
        token = erro(token, token_esperados, 1, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }
    printf("sai de parametros()\n");
    return token;
}

char *corpo_p(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha, int *modo_panico)
{
    printf("entrei em corpo_p()\n");

    char *tokens_seguidores_dc_v[1];
    tokens_seguidores_dc_v[0] = "comando_reservado_begin";

    char *tokens_inicio_cmd[7];
    tokens_inicio_cmd[0] = "comando_reservado_read";
    tokens_inicio_cmd[1] = "comando_reservado_write";
    tokens_inicio_cmd[2] = "comando_reservado_while";
    tokens_inicio_cmd[3] = "comando_reservado_if";
    tokens_inicio_cmd[4] = "comando_reservado_begin";
    tokens_inicio_cmd[5] = "comando_reservado_for";
    tokens_inicio_cmd[6] = "identificador";
    
    token = dc_v(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, tokens_seguidores_dc_v, 1);
    *numero_linha -=1;
    if (strcmp(token, "comando_reservado_begin") == 0)
    {
        *modo_panico = -1;
        printf("'begin' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
        
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: 'begin' esperado\n", *numero_linha);
        char *token_esperados[8];
        token_esperados[0] = "comando_reservado_read";
        token_esperados[1] = "comando_reservado_write";
        token_esperados[2] = "comando_reservado_while";
        token_esperados[3] = "comando_reservado_if";
        token_esperados[4] = "comando_reservado_begin";
        token_esperados[5] = "comando_reservado_for";
        token_esperados[6] = "identificador";
        token_esperados[7] = "comando_reservado_procedure";

        token = erro(token, token_esperados, 8, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    int ativa_comandos = 0;
    for(int i = 0; i < 7; i++){
        if(strcmp(token, tokens_inicio_cmd[i]) == 0)
        {
            ativa_comandos = 1;
            token = comandos(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico);
            break;
        }
    }
    if(ativa_comandos == 0)
    {
        *modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: comando esperado\n", *numero_linha);
        char *token_esperados[3];
        token_esperados[0] = "comando_reservado_end";
        token_esperados[1] = "comando_reservado_begin";
        token_esperados[2] = "comando_reservado_procedure";

        token = erro(token, token_esperados, 3, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    
    if (strcmp(token, "comando_reservado_end") == 0)
    {
        *modo_panico = -1;
        printf("'end' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        fprintf(arquivo_saida, "Erro sintático na linha %d: 'end' esperado\n", *numero_linha);
        char *token_esperados[3];
        token_esperados[0] = "comando_reservado_ponto_virgula";
        token_esperados[1] = "comando_reservado_procedure";
        token_esperados[2] = "comando_reservado_begin";
        token = erro(token, token_esperados, 3, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    if (strcmp(token, "comando_reservado_ponto_virgula") == 0)
    {
        *modo_panico = -1;
        printf("';' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        
        fprintf(arquivo_saida, "Erro sintático na linha %d: ';' esperado\n", *numero_linha-1);
        char *token_esperados[2];
        token_esperados[0] = "comando_reservado_procedure";
        token_esperados[1] = "comando_reservado_begin";
        token = erro(token, token_esperados, 2, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        // printf("token: %s \n", token);
    }

    printf("sai de corpo_p()\n");
    return token;
}

char *comandos(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha, int *modo_panico)
{
    printf("entrei em comandos() \n");
    // printf("token: %s \n", token);
    
    // Para caso cmd não precise ser executado, visto que todo caminho de comandos é seguido por 'end'
    if (strcmp(token, "comando_reservado_end") == 0)
    {
        printf("sai de comandos() \n");
        return token;
    }

    char* token_seguidor[1];
    token_seguidor[0] = "comando_reservado_ponto_virgula";
    token = cmd(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, token_seguidor);
    printf("token: %s \n", token);

    if (strcmp(token, "comando_reservado_ponto_virgula") == 0)
    {
        *modo_panico = -1;
        printf("';' lido\n");
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
        
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        printf("Erro sintático na linha %d: ';' esperado\n", *numero_linha);
        char *token_esperados[8];
        token_esperados[0] = "comando_reservado_read";
        token_esperados[1] = "comando_reservado_write";
        token_esperados[2] = "comando_reservado_while";
        token_esperados[3] = "comando_reservado_if";
        token_esperados[4] = "comando_reservado_begin";
        token_esperados[5] = "comando_reservado_for";
        token_esperados[6] = "identificador";
        token_esperados[7] = "comando_reservado_end";
        token = erro(token, token_esperados, 8, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }
    
    char *tokens_inicio_cmd[7];
    tokens_inicio_cmd[0] = "comando_reservado_read";
    tokens_inicio_cmd[1] = "comando_reservado_write";
    tokens_inicio_cmd[2] = "comando_reservado_while";
    tokens_inicio_cmd[3] = "comando_reservado_if";
    tokens_inicio_cmd[4] = "comando_reservado_begin";
    tokens_inicio_cmd[5] = "comando_reservado_for";
    tokens_inicio_cmd[6] = "identificador";

    for(int i = 0; i < 7; i ++)
    {
        if(strcmp(token, tokens_inicio_cmd[i]) == 0)  comandos(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico);
    }

    if (strcmp(token, "comando_reservado_end") == 0)
    {
        comandos(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico);
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        printf("Erro sintático na linha %d: ';' esperado\n", *numero_linha);
        char *token_esperados[2];
        token_esperados[0] = "comando_reservado_ponto_virgula";
        token_esperados[1] = "comando_reservado_end";
        token = erro(token, token_esperados, 2, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
    }
    return token;
}

char *cmd(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha, int *modo_panico, char* token_seguidor[])
{
    char *tokens_seguidores_variaveis[1];
    tokens_seguidores_variaveis[0] = "comando_reservado_fecha_parenteses";

    printf("entrei em cmd() \n");

    if (strcmp(token, "comando_reservado_read") == 0)
    {
        *modo_panico = -1;
        printf("'read' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);

        if (strcmp(token, "comando_reservado_abre_parenteses") == 0)
        {
            *modo_panico = -1;
            printf("'(' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);

        }
        else if (*modo_panico == -1)
        {
            *modo_panico = 1;
            printf("Erro sintático na linha %d: '(' esperado\n", *numero_linha);
            char *token_esperados[2];
            token_esperados[0] = "comando_reservado_identificador";
            token_esperados[1] = token_seguidor[0];

            token = erro(token, token_esperados, 2, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }

        token = variaveis(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, tokens_seguidores_variaveis, 1);

        if (strcmp(token, "comando_reservado_fecha_parenteses") == 0)
        {
            *modo_panico = -1;
            printf("')' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }
        else if (*modo_panico == -1)
        {
            *modo_panico = 1;
            printf("Erro sintático na linha %d: ')' esperado\n", *numero_linha);
            char *token_esperados[1];
            token_esperados[0] = token_seguidor[0];
            token = erro(token, token_esperados, 1, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }

        printf("sai de cmd() \n");
        return token;
    }

    if (strcmp(token, "comando_reservado_write") == 0)
    {
        *modo_panico = -1;
        printf("'write' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);

        if (strcmp(token, "comando_reservado_abre_parenteses") == 0)
        {
            *modo_panico = -1;
            printf("'(' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
            
        }
        else if (*modo_panico == -1)
        {
            *modo_panico = 1;
            printf("Erro sintático na linha %d: '(' esperado\n", *numero_linha);
            char *token_esperados[2];
            token_esperados[0] = "comando_reservado_identificador";
            token_esperados[1] = token_seguidor[0];
            token = erro(token, token_esperados, 2, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }

        token = variaveis(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, tokens_seguidores_variaveis, 1);

        if (strcmp(token, "comando_reservado_fecha_parenteses") == 0)
        {
            *modo_panico = -1;
            printf("')' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
            
        }
        else if(*modo_panico == -1)
        {
            *modo_panico = 1;
            char *token_esperados[1];
            token_esperados[0] = token_seguidor[0];

            token = erro(token, token_esperados, 1, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }

        printf("sai de cmd() \n");
        return token;
    }

    if (strcmp(token, "comando_reservado_while") == 0)
    {
        *modo_panico = -1;
        printf("'while' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);

        if (strcmp(token, "comando_reservado_abre_parenteses") == 0)
        {
            *modo_panico = -1;
            printf("'(' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }
        else if (*modo_panico == -1)
        {
            *modo_panico = 1;
            printf("Erro sintático na linha %d: '(' esperado\n", *numero_linha);
            char *token_esperados[2];
            token_esperados[0] = "comando_reservado_identificador";
            token_esperados[1] = token_seguidor[0];
            token = erro(token, token_esperados, 2, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }

        token = variaveis(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, tokens_seguidores_variaveis, 1);

        if (strcmp(token, "comando_reservado_fecha_parenteses") == 0)
        {
            *modo_panico = -1;
            printf("')' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }
        else if(*modo_panico == -1)
        {
            *modo_panico = 1;
            char *token_esperados[2];
            token_esperados[0] = "comando_reservado_do";
            token_esperados[1] = token_seguidor[0];

            token = erro(token, token_esperados, 2, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }

        if (strcmp(token, "comando_reservado_do") == 0)
        {
            *modo_panico = -1;
            printf("'do' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
            
        }
        else if(*modo_panico == -1)
        {
            *modo_panico = 1;
            printf("Erro sintático na linha %d: 'do' esperado\n", *numero_linha);
            char *token_esperados[8];
            token_esperados[0] = "comando_reservado_read";
            token_esperados[1] = "comando_reservado_write";
            token_esperados[2] = "comando_reservado_while";
            token_esperados[3] = "comando_reservado_if";
            token_esperados[4] = "comando_reservado_begin";
            token_esperados[5] = "comando_reservado_for";
            token_esperados[6] = "identificador";
            token_esperados[7] = token_seguidor[0];


            token = erro(token, token_esperados, 8, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }

        token = cmd(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, token_seguidor);
        printf("sai de cmd() \n");
        return token;
    }

    if (strcmp(token, "comando_reservado_if") == 0)
    {
        *modo_panico = -1;
        printf("'if' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
        token = condicao(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico);

        if (strcmp(token, "comando_reservado_then") == 0)
        {
            *modo_panico = -1;
            printf("'then' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }
        else if(*modo_panico == -1)
        {
            *modo_panico = 1;
            printf("Erro sintático na linha %d: 'do' esperado\n", *numero_linha);
            char *token_esperados[8];
            token_esperados[0] = "comando_reservado_read";
            token_esperados[1] = "comando_reservado_write";
            token_esperados[2] = "comando_reservado_while";
            token_esperados[3] = "comando_reservado_if";
            token_esperados[4] = "comando_reservado_begin";
            token_esperados[5] = "comando_reservado_for";
            token_esperados[6] = "identificador";
            token_esperados[7] = token_seguidor[0];

            token = erro(token, token_esperados, 8, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }
        char* token_seguidor_else[1];
        token_seguidor_else[0] = "comando_reservado_else";
        token = cmd(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, token_seguidor_else);

        if (strcmp(token, "comando_reservado_else") == 0)
        {
            *modo_panico = -1;
            printf("'else' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
            token = cmd(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, token_seguidor);
        }

        
        printf("sai de cmd() \n");
        return token;
    }

    if (strcmp(token, "identificador") == 0) // p/ esse não vi a necessidade de tratamento de erros, vê se você concorda
    {
        *modo_panico = -1;
        printf("'identificador' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);

        if (strcmp(token, "comando_reservado_atribuicao") == 0)
        {
            *modo_panico = -1;
            printf("':=' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
            token = expressao(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, token_seguidor, 1);
            printf("sai de cmd() \n");
        }
        else if (strcmp(token, "comando_reservado_abre_parenteses") == 0)
        {
            *modo_panico = -1;
            token = lista_arg(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico);
            printf("sai de cmd() \n");
        }
        else if (*modo_panico == -1)
        {
            *modo_panico = 1;
            printf("Erro sintático na linha %d: ':=' ou '(' esperado\n", *numero_linha);
            char *token_esperados[1];
            token_esperados[0] = token_seguidor[0];

            token = erro(token, token_esperados, 1, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }

        return token;
    }

    if (strcmp(token, "comando_reservado_begin") == 0)
    {
        *modo_panico = -1;
        printf("'begin' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
        token = comandos(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico);

        if (strcmp(token, "comando_reservado_end") == 0)
        {
            *modo_panico = -1;
            printf("'end' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }
        else if(*modo_panico == -1)
        {
            *modo_panico = 1;
            printf("Erro sintático na linha %d: 'end' esperado\n", *numero_linha);
            char *token_esperados[1];
            token_esperados[0] = token_seguidor[0];
            token = erro(token, token_esperados, 1, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }

        printf("sai de cmd() \n");
        return token;
    }

    if (strcmp(token, "comando_reservado_for") == 0)
    {
        *modo_panico = -1;
        printf("'for' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);

        if (strcmp(token, "comando_reservado_ident") == 0)
        {
            *modo_panico = -1;
            printf("'identificador' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }
        else if (*modo_panico == -1)
        {
            *modo_panico = 1;
            printf("Erro sintático na linha %d: 'identificador' esperado\n", *numero_linha);
            char *token_esperados[2];
            token_esperados[0] = "comando_reservado_atribuicao";
            token_esperados[1] = token_seguidor[0];
            token = erro(token, token_esperados, 2, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }

        if (strcmp(token, "comando_reservado_atribuicao") == 0)
        {
            *modo_panico = -1;
            printf("':=' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }
        else if (*modo_panico == -1)
        {
            *modo_panico = 1;
            printf("Erro sintático na linha %d: ':=' esperado\n", *numero_linha);
            char *token_esperados[2];
            token_esperados[0] = "num_int";
            token_esperados[1] = token_seguidor[0];
            token = erro(token, token_esperados, 2, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }

        if (strcmp(token, "num_int") == 0)
        {
            *modo_panico = -1;
            printf("'número inteiro' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }
        else if(*modo_panico == -1)
        {
            *modo_panico = 1;
            printf("Erro sintático na linha %d: 'número inteiro' esperado\n", *numero_linha);
            char *token_esperados[2];
            token_esperados[0] = "comando_reservado_to";
            token_esperados[1] = token_seguidor[0];
            token = erro(token, token_esperados, 2, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }

        if (strcmp(token, "comando_reservado_to") == 0)
        {
            *modo_panico = -1;
            printf("'to' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }
        else if (*modo_panico == -1)
        {
            *modo_panico = 1;
            printf("Erro sintático na linha %d: 'to' esperado\n", *numero_linha);
            char *token_esperados[2];
            token_esperados[0] = "num_int";
            token_esperados[1] = token_seguidor[0];
            token = erro(token, token_esperados, 2, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }

        if (strcmp(token, "num_int") == 0)
        {
            *modo_panico = -1;
            printf("'número inteiro' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }
        else
        {
            *modo_panico = 1;
            printf("Erro sintático na linha %d: 'número inteiro' esperado\n", *numero_linha);
            char *token_esperados[2];
            token_esperados[0] = "comando_reservado_do";
            token_esperados[1] = token_seguidor[0];
            token = erro(token, token_esperados, 2, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }

        if (strcmp(token, "comando_reservado_do") == 0)
        {
            *modo_panico = -1;
            printf("'do' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
            
        }
        else
        {
            *modo_panico = 1;
            printf("Erro sintático na linha %d: 'do' esperado\n", *numero_linha);
            char *token_esperados[1];
            token_esperados[0] = token_seguidor[0];
            token = erro(token, token_esperados, 1, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }
        
        token = cmd(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, token_seguidor);
        printf("sai de cmd() \n");
        return token;
    }
    return token;
}

char *condicao(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha, int *modo_panico)
{
    printf("entrei em condicao() \n");

    char* seguidores_expressao_1[6];
    seguidores_expressao_1[0] = "comando_reservado_igual";
    seguidores_expressao_1[1] = "comando_reservado_diferente";
    seguidores_expressao_1[2] = "comando_reservado_maior_igual";
    seguidores_expressao_1[3] = "comando_reservado_menor_igual";
    seguidores_expressao_1[4] = "comando_reservado_maior";
    seguidores_expressao_1[5] = "comando_reservado_menor";

    token = expressao(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, seguidores_expressao_1, 6);
    if ((strcmp(token, "comando_reservado_igual") == 0) || (strcmp(token, "comando_reservado_diferente") == 0) || (strcmp(token, "comando_reservado_maior_igual") == 0) || (strcmp(token, "comando_reservado_menor_igual") == 0) || (strcmp(token, "comando_reservado_maior") == 0) || (strcmp(token, "comando_reservado_menor") == 0))
    {
        *modo_panico = -1;
        if ((strcmp(token, "comando_reservado_igual") == 0))
            printf("'=' lido\n");
        if ((strcmp(token, "comando_reservado_diferente") == 0))
            printf("'<>' lido\n");
        if ((strcmp(token, "comando_reservado_maior_igual") == 0))
            printf("'>=' lido\n");
        if ((strcmp(token, "comando_reservado_menor_igual") == 0))
            printf("'<=' lido\n");
        if ((strcmp(token, "comando_reservado_maior") == 0))
            printf("'>' lido\n");
        else
            printf("'<' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);

    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        printf("Erro sintático na linha %d: '=', '<>', '>=', '<=', '>' ou '<' esperado\n", *numero_linha);
        char *token_esperados[7];
        token_esperados[0] = "comando_reservado_igual";
        token_esperados[1] = "comando_reservado_diferente";
        token_esperados[2] = "comando_reservado_maior_igual";
        token_esperados[3] = "comando_reservado_menor_igual";
        token_esperados[4] = "comando_reservado_maior";
        token_esperados[5] = "comando_reservado_menor";
        token_esperados[6] = "comando_reservado_then";
        token = erro(token, token_esperados, 7, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
    }

    char* seguidores_expressao_2[1];
    seguidores_expressao_2[0] = "comando_reservado_then";
    token = expressao(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, seguidores_expressao_2, 1);
    printf("sai de condicao() \n");
    return token;
}

char *fator(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha, int *modo_panico, char* seguidores_pai[], int tamanho)
{
    printf("entrei em fator() \n");
    if ((strcmp(token, "identificador") == 0) || (strcmp(token, "num_int") == 0) || (strcmp(token, "num_real") == 0))
    {
        *modo_panico = -1;
        if ((strcmp(token, "identificador") == 0))
            printf("'identificador' lido\n");
        if ((strcmp(token, "num_int") == 0))
            printf("'número inteiro' lido\n");
        else
            printf("'número real' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
    }
    else if (strcmp(token, "comando_reservado_abre_parenteses") == 0)
    {
        *modo_panico = -1;
        printf("'(' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);

        char* seguidores_expressao[1+ tamanho];
        seguidores_expressao[0] = "comando_reservado_fecha_parenteses";
        for(int i = 0; i < tamanho; i++) {
            seguidores_expressao[i+1] = seguidores_pai[i];
        }

        token = expressao(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, seguidores_expressao, 1+tamanho);

        if (strcmp(token, "comando_reservado_fecha_parenteses") == 0)
        {
            *modo_panico = -1;
            printf("')' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }
        else if (*modo_panico == -1)
        {
            *modo_panico = 1;
            printf("Erro sintático na linha %d: ')' esperado\n", *numero_linha);
            token = erro(token, seguidores_pai, tamanho, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }
    }

    printf("sai de fator() \n");
    return token;
}

char *expressao(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha, int *modo_panico, char* seguidores_pai[], int tamanho) // não vi necessidade de implementar o tratamento de erros aqui
{
    printf("entrei em expressao()\n");
    char* seguidores_termo [2+tamanho];
    seguidores_termo [0] = "comando_reservado_soma";
    seguidores_termo [1] = "comando_reservado_subtracao";
    for(int i = 0; i < tamanho; i ++){
        seguidores_termo [i+2] = seguidores_pai[i];
    }
    token = termo(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, seguidores_termo, 2+tamanho);
    while ((strcmp(token, "comando_reservado_soma") == 0) || (strcmp(token, "comando_reservado_subtracao") == 0))
    {
        if ((strcmp(token, "comando_reservado_soma") == 0))
            printf("'+' lido\n");
        else
            printf("'-' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
        token = termo(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, seguidores_termo, 2 + tamanho);
    }
    printf("sai de expressao()\n");
    // free(seguidores_termo);
    return token;
}

char *termo(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha, int *modo_panico, char* seguidores_pai[], int tamanho)
{
    printf("entrei em termo()\n");
    if ((strcmp(token, "comando_reservado_soma") == 0) || (strcmp(token, "comando_reservado_subtracao") == 0))
    {
        if ((strcmp(token, "comando_reservado_soma") == 0))
            printf("'+' lido\n");
        else
            printf("'-' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
    }

    char* seguidores_fator[2+tamanho];
    seguidores_fator[0] = "comando_reservado_multiplicacao";
    seguidores_fator[1] = "comando_reservado_divisao";
    for(int i = 0; i < tamanho; i ++){
        seguidores_fator[i + 2] = seguidores_pai[i];
    }
    
    token = fator(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, seguidores_fator, 2+tamanho);
    while ((strcmp(token, "comando_reservado_multiplicacao") == 0) || (strcmp(token, "comando_reservado_divisao") == 0))
    {
        if ((strcmp(token, "comando_reservado_multiplicacao") == 0))
            printf("'*' lido\n");
        else
            printf("'/' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
        token = fator(token, arquivo_entrada, arquivo_saida, controle_arq, numero_linha, modo_panico, seguidores_fator, 2+tamanho);
    }
    printf("sai de termo()\n");
    // free(seguidores_fator);
    return token;
    
}

char *lista_arg(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha, int *modo_panico)
{
    printf("entrei em lista_arg()\n");

    if (strcmp(token, "comando_reservado_abre_parenteses") == 0)
    {
        *modo_panico = -1;
        printf("'(' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        printf("Erro sintático na linha %d: '(' esperado\n", *numero_linha);
        char *token_esperados[1];
        token_esperados[0] = "identificador";
        token = erro(token, token_esperados, 1, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
    }
    if (strcmp(token, "comando_reservado_identificador") == 0)
    {
        *modo_panico = -1;
        printf("'identificador' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
    }
    else if (*modo_panico == -1)
    {
        *modo_panico = 1;
        printf("Erro sintático na linha %d: 'identificador' esperado\n", *numero_linha);
        char *token_esperados[2];
        token_esperados[0] = "comando_reservado_ponto_virgula";
        token_esperados[1] = "comando_reservado_fecha_parenteses";
        token = erro(token, token_esperados, 2, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
    }
    while (strcmp(token, "comando_reservado_ponto_virgula") == 0)
    {
        printf("';' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
        if (strcmp(token, "identificador") == 0)
        {
            *modo_panico = -1;
            printf("'identificador' lido\n");
            free(token);
            token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
        }
        else if (*modo_panico == -1)
        {
            *modo_panico = 1;
            printf("Erro sintático na linha %d: 'identificador' esperado\n", *numero_linha);
            char *token_esperados[2];
            token_esperados[0] = "comando_reservado_ponto_virgula";
            token_esperados[1] = "comando_reservado_fecha_parenteses";
            token = erro(token, token_esperados, 2, arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
            printf("token: %s \n", token);
            if(strcmp(token, token_esperados[1]) == 0) break;
        }
    }
    if (strcmp(token, "comando_reservado_fecha_parenteses") == 0)
    {
        *modo_panico = -1;
        printf("')' lido\n");
        free(token);
        token = analisador_lexico(arquivo_entrada, arquivo_saida, controle_arq, numero_linha);
        printf("token: %s \n", token);
        printf("sai de lista_arg()\n");
        
    }
    else if(*modo_panico == -1)
    {
        *modo_panico = 1;
        printf("Erro sintático na linha %d: ')' esperado\n", *numero_linha);
    }
    return token;
}
