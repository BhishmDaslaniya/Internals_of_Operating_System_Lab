/****************************************************************
	Author : Bhishm Daslaniya [17CE023]	
	"Make it work, make it right, make it fast." 
												– Kent Beck		
****************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main(int argc, char*  argv[]){
	char* args[]={"wc","-m",argv[1],0};
	execv("/usr/bin/wc",args);
}