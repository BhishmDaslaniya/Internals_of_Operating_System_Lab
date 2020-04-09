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
    int file_desc = open("dup.txt", O_WRONLY | O_APPEND | O_CREAT);
    if(file_desc < 0)
        printf("Error opening the file\n");
       
	int copy_desc = dup(file_desc);
    write(copy_desc,"This will be output to the file named dup.txt\n", 46);
    write(file_desc,"This will also be output to the file named dup.txt\n", 51);
	printf("old file descriptor: %d \nnew file descriptor: %d \n",file_desc,copy_desc);

    return 0;
}