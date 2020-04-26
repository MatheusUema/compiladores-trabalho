#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

int main(){
    char teste[29] ;
    teste[0]='w';
    teste[1]='h';
    teste[2]='i';
    teste[3]='l';
    teste[4]='e';
    
    
  
    int q = strncmp(teste, "abcdefgh",7);

    if (strncmp(teste, "abcdefgh",7)==0){
        printf ("Tu eh corno :)");
        return 0;

    }


            if(strncmp(teste, "program",7)==0){
                printf(", comando_reservado_program/n");
                return 0;
            };

             if(strncmp(teste, "begin",5)==0){
                printf(", comando_reservado_begin/n");
                return 0;
            }; 
            
            if(strncmp(teste, "end",3)==0){
                printf(", comando_reservado_end/n");
                return 0;
            }; 
            
            if(strncmp(teste, "const",5)==0){
                printf(", comando_reservado_const/n");
                return 0;
            }; 
            
            if(strncmp(teste, "var",3)==0){
                printf(", comando_reservado_var/n");
                return 0;
            }; 
            
            if(strncmp(teste, "real",4)==0){
                printf(", comando_reservado_real/n");
                return 0;
            };
            
             if(strncmp(teste, "integer",7)==0){
                printf(", comando_reservado_integer/n");
                return 0;
            }; 
            
            if(strncmp(teste, "procedure",9)==0){
                printf(", comando_reservado_procedure/n");
                return 0;
            }; 
            
            if(strncmp(teste, "else",4)==0){
                printf(", comando_reservado_else/n");
                return 0;
            }; 
            
            if(strncmp(teste, "read",4)==0){
                printf(", comando_reservado_read/n");
                return 0;
            }; 
            
            if(strncmp(teste, "write",5)==0){
                printf(", comando_reservado_write/n");
                return 0;
            };
            
             if(strncmp(teste, "while",5)==0){
                printf(", comando_reservado_while/n");
                return 0;
            };
            
             if(strncmp(teste, "do",2)==0){
                printf(", comando_reservado_do/n");
                return 0;
            };
            
             if(strncmp(teste, "for",2)==0){
                printf(", comando_reservado_for/n");
                return 0;
            }; 
            
            if(strncmp(teste, "if",2)==0){
                printf(", comando_reservado_if/n");
                return 0;
            }; 
            
            if(strncmp(teste, "for",3)==0){
                printf(", comando_reservado_for/n");
                return 0;
            };

}
