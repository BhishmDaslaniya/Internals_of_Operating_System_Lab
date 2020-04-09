/****************************************************************
	Author : Bhishm Daslaniya [17CE023]	
	"Make it work, make it right, make it fast." 
												– Kent Beck		
****************************************************************/

#include<bits/stdc++.h>
#include<stdlib.h>
#include<sys/types.h>
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

	int a[100];
	int sum = 0;
	int *s;
	s= shm_ptr;
	for(int i = 0 ; i < 100 ;i++){
		a[i] = *s++;
		sum+=a[i];
	}
	printf("sum is %d \n",sum);
	
	*shm_ptr = '*';

	shmdt(shm_ptr);
	
	shmctl(shm_id,IPC_RMID,NULL);
	return 0;
}