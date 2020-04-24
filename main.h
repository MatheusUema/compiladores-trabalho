#ifndef MAIN_H
#define MAIN_H

int analisador_lexico(char *arquivo);

int confere_numero (char caract);

int confere_letra (char caract);

int confere_branco (char caract);

void checa_invalido_num(char caract, int controle);

void checa_invalido_palavra(char caract, int controle);

void checa_invalido_comentario(char caract, int controle);

void checa_invalido_reservado(char caract, int controle);

int checa_invalido_geral(char caract, int controle, int automato);

int seleciona_automato(char caract, int controle); 
#endif