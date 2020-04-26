#ifndef MAIN_H
#define MAIN_H

int analisador_lexico(char *arquivo);

int confere_numero (char caract);

int confere_letra (char caract);

int confere_branco (char caract);

int confere_reservado (char caract);

void checa_invalido_num(char caract, int controle, int racional, int quebra_linha);

void checa_invalido_palavra(char caract, int controle, int quebra_linha);

void checa_invalido_comentario(char caract, int controle, int quebra_linha, char *arquivo);

void checa_invalido_reservado(char caract, int controle);

int checa_invalido_geral(char caract, int controle, int automato, int racional, int quebra_linha, char *arquivo);

int check_valido_geral (char buffer, int automato, int *racional);

int seleciona_automato(char caract); 

int devolvo_cadeia(char cadeia [29], int controle, int automato, int racional);

int teste (char testes[]);

int inicio_cadeia (char cadeia[], FILE* arquivo, int* controle_arquivo);

int devolve_cadeia(char cadeia[], int automato, FILE* saida, int acerto, int racional);

int condicao_final( char buffer, int automato);

#endif