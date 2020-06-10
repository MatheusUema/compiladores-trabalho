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

int programa();

char* dc_c();

char* dc_v();

char* variaveis();

char* dc_p();

char* parametros();

char* corpo_p();

char* comandos();

char* cmd();

int condicao();

int fator();

int expressao();

int termo();

int lista_arg();

#endif