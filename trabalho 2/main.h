#ifndef MAIN_H
#define MAIN_H


char* analisador_lexico(FILE *arquivo_entrada, FILE* arquivo_saida, int* controle_arq, int* numero_linha);

int confere_numero (char caract);

int confere_letra (char caract);

int confere_branco (char caract);

int confere_reservado (char caract);

int check_valido_geral (char buffer, int automato, int *racional);

int seleciona_automato(char caract); 

int inicio_cadeia (char cadeia[], FILE* arquivo, int* controle_arquivo, int* controle_linha);

char* devolve_cadeia(char cadeia[], int automato, int acerto, int racional, FILE* saida, int num_linha);

int condicao_final( char buffer, int automato);

char* erro(char *token, char *token_esperado[], int size, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha);

int programa(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha);

char* dc_c(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha, int *modo_panico);

char* dc_v(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha, int *modo_panico, char *tokens_seguidores[], int tamanho);

char* variaveis(char *token, FILE *arquivo_entrada, FILE *arquivo_saida, int *controle_arq, int *numero_linha, int *modo_panico, char *tokens_seguidores[], int tamanho);

char* dc_p();

char* parametros();

char* corpo_p();

char* comandos();

char* cmd();

char* condicao();

char* fator();

char* expressao();

char* termo();

char* lista_arg();

#endif