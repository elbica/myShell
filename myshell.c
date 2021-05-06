#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include<stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include "shell_func.h"
#include "symbols.h"

#define PARSER " \t\r\n" 

//최근에 입력된 10개의 명령어들을 history로 관리, 환형 큐 구조.changed
char* history[11];
int historyFront = 0, historyRear = 0, historySize = 0;

//명령어의 타입들을 bitmask 형식으로 저장한다.
int type = 0;

//입력된 명령어를 파싱해 배열에 저장하고, 파싱된 총 명령어 개수를 반환 
size_t parse (char* input, char** arrs){
    size_t count = 0;
    char* temp = strtok(input, PARSER);
    
    while(temp != NULL){
        arrs[count++]= temp;
        temp = strtok(NULL, PARSER);
    }
    arrs[count] = NULL;
    return count;
}

//최근 명령어 10개 history print
void printHistory(){
    int start = historyFront;
    for(int i=0; i<historySize; start = (start+1)%11,i++) printf("%d : %s",i+1,history[start]);
}

//help manual print
void help(){
    char* helpStrings[16];
    helpStrings[0] = "*********************************************\n";
    helpStrings[1] = "* THIS MINI SHELL IS DEVELOPED BY SOHEE KIM *\n";
    helpStrings[2] = "*********************************************\n";
    helpStrings[3] = "\n";
    helpStrings[4] = "DESCRIPTION : ";
    helpStrings[5] = "\tBash  is  an  sh-compatible  command language interpreter that executes \n\tcommands read from the standard input or from a file.  Bash also incorporates \n\tuseful features from the Korn and C shells (ksh and csh).\n\n";
    helpStrings[6] = "*********************************************\n\n";
    helpStrings[7] = "COMMANDS : \n";
    helpStrings[8] = "\tls, cd, mkdir, mv, rm, touch, pwd, help, history, quit ...\n\n";
    helpStrings[9] = "OPTIONS : \n";
    helpStrings[10] = "\t-a, -l, -r, -f, -p, -v ...\n";
    helpStrings[11] = "\n";
    helpStrings[12] = "*********************************************\n";
    helpStrings[13] = "PLEASE USE & TO EXCUTE COMMAND BY BACKGROUND PROCESS!\n";
    helpStrings[14] = "                                        ex) ls -al &\n";
    helpStrings[15] = "THANK YOU FOR USING MYSHELL\n";
    for(int i=0; i<16; i++ ) printf("%s",helpStrings[i]);
}
void quit(){
    printf("myshell developed by 김소희(12191704)\n");
    exit(1);
}

//**명령어 실행**
void run(char** lines, size_t inputSize){
    if(!strcmp(lines[0],"quit")) quit();
    else if(!strcmp(lines[0],"help")) help();
    else if(!strcmp(lines[0], "history")) printHistory();
    else if(!strcmp(lines[0],"cd")) sh_cd(inputSize,lines);
    else { 
        //background 여부 검사
        bool background = type & (1<<BACKGROUND);

        //내장형 명령어가 아닌 경우 자식 프로세스 fork
        pid_t pid, d_pid; int status;
        pid = fork();
        
        if(pid > 0){ //parent process
            if(background == 0){
            //background가 설정되어있지 않으면, 생성한 자식 프로세스가 종료될 때까지 기다린다.
            while ((d_pid = wait(&status)) != pid && d_pid != -1){};
            }
        }else if(pid == 0){ //child process
            if(!strcmp(lines[0],"pwd")) sh_pwd();

            //이외의 명령어들을 핸들링한다. 
            //오류 발생 시, 자식 프로세스를 종료시킨다. 
            else sh_default(inputSize, lines);
            
        }else {
            printf("fork errer\n");
        }

    }
}

int main(){
    char input[100];
    for(int i=0; i<11; i++) history[i] = malloc(sizeof(char)*50);
    while(1){
        printf("12191704_shell$ ");

        //fgets : 스트림에서 문자열을 받아서 (num - 1) 개의 문자를 입력 받을 때, 
        //개행 문자나 EoF 에 도달할 때까지 입력 받아 C 형식의 문자열로 저장
        //error 발생 시 break
        if(!fgets(input,sizeof(input)-1,stdin)) break;

        //enter만 입력 됐을 시 continue
        if(strcmp(input,"\n")==0) continue;

        //명령어 저장
        strcpy(history[historyRear],input);
        historyRear = (historyRear + 1) % 11;
        if(historySize!=10) historySize++;
        else historyFront =(historyFront + 1) % 11;

        //type check(background)
        type = getType(input);

        //명령어 파싱
        char* lines[100];
        size_t inputSize = parse(input,lines);
        lines[inputSize] = NULL;

        //파싱된 명령어 실행
        run(lines,inputSize);

        //printf("\n");
    }
}