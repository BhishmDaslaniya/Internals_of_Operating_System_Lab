/****************************************************************
	Author : Bhishm Daslaniya [17CE023]	
	"Make it work, make it right, make it fast." 
												– Kent Beck		
****************************************************************/

#include<bits/stdc++.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdarg.h> 
#include<string.h>
#include<sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include <dirent.h> 

using namespace std;


int main(){
	int pid = fork();
	if(pid < 0){
		perror("Child  process not created!");
		return -1;
	}else if(pid == 0){
		cout<<"Executing..."<<endl;
		return 0;
	}else{
		cout<<"waiting to complete execution of child process!"<<endl;
		sleep(5);
		wait();
		cout<<"Parent executed!"<<endl;
	}
}