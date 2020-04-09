/****************************************************************
	Author : Bhishm Daslaniya [17CE023]	
	"Make it work, make it right, make it fast." 
												– Kent Beck		
****************************************************************/

#include<bits/stdc++.h>
#include<sys/types.h>
#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include<sys/ipc.h>
#include<sys/shm.h>

int main(){
	// key_t somekey;
	// somekey = 12345;

	key_t k = ftok("./",'x');

	// printf("%d\n",somekey);
	printf("generated key : %d\n",k);

	int shm_id = shmget(k,100*sizeof(int),IPC_CREAT | 0666);
	if(shm_id == -1){
		printf("Error in shmget!!!\n");
	}
	printf("shared area key : %d\n",shm_id);
	
	int *shm_ptr = (int*)shmat(shm_id, NULL, 0);

	if(shm_ptr == (int*) -1){
		printf("Error in shmat!!!\n");
	}
	int *s = shm_ptr;
	for(int i = 1 ; i <= 100 ;i++){
		*s++ = i;
	}

	while(*shm_ptr != '*'){
		sleep(1);
	}

	printf("End!!!\n");
	shmdt(shm_ptr);
	
	
	return 0;
}