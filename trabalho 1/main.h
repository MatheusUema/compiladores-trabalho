#ifndef MAIN_H
#define MAIN_H

int analisador_lexico(char *arquivo);

int confere_numero (char caract);

int confere_letra (char caract);

int confere_branco (char caract);

int confere_reservado (char caract);

int check_valido_geral (char buffer, int automato, int *racional);

int seleciona_automato(char caract); 

int inicio_cadeia (char cadeia[], FILE* arquivo, int* controle_arquivo);

int devolve_cadeia(char cadeia[], int automato, FILE* saida, int acerto, int racional);

int condicao_final( char buffer, int automato);

#endif