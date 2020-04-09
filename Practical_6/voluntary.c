#include <stdio.h>
#include <unistd.h>
#include<string.h>
int main(int argc, char *argv[]){
	printf("PID : %d\n",getpid() );
	int i,j;
	char file[10];
	const char b;

	FILE *fp;
	strcpy(file,"a.txt");
	for(i = 0; i < 50000; i++){
		fp = fopen(file,"r");
		sleep(1);
		fclose(fp);
	}
}
