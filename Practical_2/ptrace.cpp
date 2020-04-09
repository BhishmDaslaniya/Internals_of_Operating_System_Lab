/****************************************************************
    Author : Bhishm Daslaniya [17CE023] 
    "Make it work, make it right, make it fast." 
                                                – Kent Beck     
****************************************************************/

#include<bits/stdc++.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h> 
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_SIZE 1024

using namespace std;

/// check for regular file
int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

/// check for directory
int isDirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

/// concat function for generate complete path!!!
char* concat(int count, ...)
{
    va_list ap;

    // Find required length to store merged string
    int len = 1; // room for NULL
    va_start(ap, count);
    for(int i=0 ; i<count ; i++)
        len += strlen(va_arg(ap, char*));
    va_end(ap);

    // Allocate memory to concat strings
    char *merged = (char*)calloc(sizeof(char),len);
    int null_pos = 0;

    // Actually concatenate strings
    va_start(ap, count);
    for(int i=0 ; i<count ; i++)
    {
        char *s = va_arg(ap, char*);
        strcpy(merged+null_pos, s);
        null_pos += strlen(s);
    }
    va_end(ap);

    return merged;
}


int copy_file(char *src, char *dest)
{
    int fdold, fdnew;
    fdold = open(src, O_RDONLY);

    if (fdold == -1){
        perror("Cannot Open File!");
        return -1;
    }

    fdnew = creat(dest, 0666);

    if (fdnew == -1){
        printf("Cannot Create File %s!", dest);
        return -1;
    }

    char buffer[BUF_SIZE];
    int count;
    while ((count = read(fdold, buffer, sizeof(buffer) ))> 0)
    {
        write(fdnew, buffer, count);
    }
    printf("Copied Successfull\n");

    return 0;
}


int copy_dir(char const *src, char const *dest)
{
    struct dirent *dr;

    DIR *source_dir = opendir(src);
    DIR *destination_dir = opendir(dest);


    if (source_dir == NULL)
    {
        perror("Could't open source directory");
        return -1;
    }
    if (destination_dir == NULL)
    {
        printf("Creating destination directory\n");
        mkdir(dest, 0777);
    }

    while ((dr = readdir(source_dir)) != NULL)
    {
        if ((( strcmp(dr->d_name, ".")) && ( strcmp(dr->d_name, ".."))))
        {
            printf("\n%s\n", dr->d_name);
            if (isDirectory(concat(3, src, "/", dr->d_name))){
                if (mkdir(concat(3, dest, "/", dr->d_name), 0777) == -1){
                    perror("Directory Creation Failed!");
                }  
                else{
                    printf("Directory Created!\n");
                    copy_dir(concat(3, src, "/", dr->d_name),concat(3, dest, "/", dr->d_name));
                }
            }
            else{
                copy_file(concat(3, src, "/", dr->d_name), concat(3, dest, "/", dr->d_name));
            }
        }
    }
    
    closedir(source_dir);
    closedir(destination_dir);

    return 0;
}


int main(int argc, char* argv[]){
	
	if(argc < 3){ 
        perror("Needs 2 Arguments for copy commands\nEx: exeFile source_file/source_dir dest_file/dest_dir");
        return -1;
    }

    pid_t child;
    int insyscall = 0;
    long orig_eax, eax;

    child = fork();
    
    if(child == 0){
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        raise(SIGCONT);
        if(is_regular_file(argv[1]) && (isDirectory(argv[2]) || is_regular_file(argv[2]))){
    	   copy_file(argv[1],argv[2]);
        }else if(isDirectory(argv[1])){
    	   copy_dir(argv[1],argv[2]);
        }else{
    	   /// File format not match!!!
    	   perror("File format is not regular or directory!");
        }
        }else{
            int status;
            printf("------------------------------------------");
            while (waitpid(child, &status, 0) && !WIFEXITED(status)) {
                struct user_regs_struct regs;
                ptrace(PTRACE_GETREGS, child, NULL, &regs);
                fprintf(stderr, "[!] Intercepted system call %lld.\n", (regs.orig_rax));
                ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            }
        }
}