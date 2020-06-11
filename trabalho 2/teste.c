#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[])
{ //Compilador

    char* teste [5];
    printf("---- %ld ---- \n", sizeof(teste));

    teste[0] = "abracadabra";
    
    teste[1] = "tuts";
    
    teste[2] = "dfdsafas";
    printf("---- %ld ---- \n", sizeof(teste));
    for(int i = 0; i < 3; i ++){
        printf("----- %s ------ \n",teste[i]);
    }
}
