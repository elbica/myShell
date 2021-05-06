#ifndef __FUNC
#define __FUNC
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>

void sh_cd(int count, char *lines[])
{   //lines : cd 경로 
	char *path;

	// 경로가 있을 경우 path를 설정
	if(count > 1){  
		path = lines[1];
	}
	// 경로가 없을 경우 현재 디렉토리로 설정
	else if(count==1){
		path = "./";
	}

	// 디렉토리를 변경한다.
	if(chdir(path) == -1){
		printf("%s: bad directory.\n", path);
	}
}

void sh_ls(int count, char *lines[]){
    char *argv[10]; argv[0] = "/bin/ls";
    
    for(int j=1; j<count; ++j) argv[j] = lines[j];
    argv[count] = NULL;
    //현재 작업공간 내에 있는 파일과 폴더를 나열한다
    if(execv( argv[0], argv )==-1) perror("execv");
    
}
void sh_mkdir(int count, char *lines[]){
    char* args[10]; args[0] = "/bin/mkdir";
    for(int j=1; j<count; ++j) args[j] = lines[j];
    args[count] = NULL;
    if(execv( args[0], args )==-1) perror("execv");
}
void sh_rm(int count, char* lines[]){
    char* args[11]; args[0] = "/bin/rm";
    for(int j=1; j<count; ++j) args[j] = lines[j];
    args[count] = "-rf"; args[count+1]=NULL;
    if(execv( args[0], args )==-1) perror("execv");
}
void sh_mv(int count, char* lines[]){
    char* args[11]; args[0] = "/bin/mv";
    for(int j=1; j<count; ++j) args[j] = lines[j];
    args[count] = NULL;
    if(execv( args[0], args )==-1) perror("execv");
}
void sh_touch(int count, char* lines[]){
    char* args[11]; args[0] = "/bin/touch";
    for(int j=1; j<count; ++j) args[j] = lines[j];
    args[count] = NULL;
    if(execv( args[0], args )==-1) perror("execv");
}
void sh_pwd(){
    //현재 작업공간의 경로를 알려 준다.
    char buf[128];
    getcwd(buf,128);
    printf("Current directory : %s",buf);
    exit(1);
}
void sh_default(int count, char* lines[]){
    char* args[50]; 
    char temp[20]; 
    args[0] = malloc(sizeof(char)*50);

    strcpy(temp,lines[0]);
    strcpy(args[0],"/bin/");
    strcat(args[0],temp);

    for(int j=1; j<count; ++j) args[j] = lines[j];
    args[count] = NULL;
    if(execv( args[0], args )==-1) perror("execv");
    printf("illegal command!\n");
    free(args[0]);
    exit(1);
}
#endif