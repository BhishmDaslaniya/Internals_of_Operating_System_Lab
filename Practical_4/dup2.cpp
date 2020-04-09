/****************************************************************
	Author : Bhishm Daslaniya [17CE023]	
	"Make it work, make it right, make it fast." 
												– Kent Beck		
****************************************************************/
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
extern int errno;
int main()
{  
	int file_desc = open("dup2.txt",O_WRONLY | O_APPEND | O_CREAT);
	// here the newfd is the file descriptor of stdout (i.e. 1)
	printf("file_desc: %d\n", file_desc);
    dup2(file_desc, 1);
    // Following lines will be printed in file dup2
    printf("I will be printed in the file dup2.txt\n");  
	printf("file_desc_after: %d\n", file_desc);
    return 0;
}