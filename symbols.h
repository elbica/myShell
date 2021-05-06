#ifndef __SYMBOLS
#define __SYMBOLS
#include <stdio.h>
#include <string.h>

typedef enum
{
	BACKGROUND,
} SYMBOLS;

int getType(char* input){
    int idx = 0;
    int ret = 0;
    while(input[idx]!='\n'){
        char c = input[idx];
        switch (c){
        case '&':
            input[idx]=' ';
            ret |= (1<<BACKGROUND);
            break;
        default:
            break;
        }
        idx++;
    }
    return ret;
}
#endif
