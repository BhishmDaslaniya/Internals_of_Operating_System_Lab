/****************************************************************
   Author : Bhishm Daslaniya [17CE023] 
   "Make it work, make it right, make it fast." 
                                    – Kent Beck    
****************************************************************/
#include<bits/stdc++.h>
#include<unistd.h>
using namespace std;

int main() {
   int pipefds[2],status,pid,sum = 0;
   int count=101,writeArray[101] = {0},readArray[101] = {0};
   for(int i=1;i<=100;i++){
      writeArray[i]=i;
   }
   status = pipe(pipefds);
   if (status == -1) {
      perror("Error in creating pipe\n");
      return 1;
   }
   pid = fork();
   
   // Child process
   if (pid != 0) {
      printf("Parent Process - Writing to pipe - Message \n");
      write(pipefds[1], writeArray, (count*sizeof(int)));
      
   } else { //Parent process
      
      read(pipefds[0], readArray, (count*sizeof(int)));
      printf("Child Process - Reading from pipe - Message\n");
      for(int i=1;i<=100;i++){
         sum+=readArray[i];
      }
   printf("Sum is : %d\n",sum);
   }   
   return 0;
}