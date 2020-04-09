/****************************************************************
    Author : Bhishm Daslaniya [17CE023] 
    "Make it work, make it right, make it fast." 
                                                – Kent Beck     
****************************************************************/

// #include<bits/stdc++.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdarg.h> 
#include<string.h>
#include<sys/types.h>
#include <sys/stat.h>
#include<fcntl.h>
#include <dirent.h> 
#include<time.h>
#include<pthread.h>

#define BUF_SIZE 1024

using namespace std;

struct thread_args {
    char *src;
    char *dest;
};


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


void *copy_file(void *args)
{
    time_t   start = time(0);
 
    struct thread_args * str = (struct thread_args *) args;

    char *src = str->src;
    char *dest = str->dest;
    
    int fdold, fdnew;
    fdold = open(src, O_RDONLY);

    if (fdold == -1){
        perror("Cannot Open File!");
        return 0;
    }

    fdnew = creat(dest, 0666);

    if (fdnew == -1){
        printf("Cannot Create File %s!", dest);
        return 0;
    }

    char buffer[BUF_SIZE];
    int count;
    while ((count = read(fdold, buffer, sizeof(buffer) ))> 0)
    {
        write(fdnew, buffer, count);
    }

    time_t end = time(0);

    // double calc_time = (end - start)  * 1000 / CLOCKS_PER_SEC;
    printf("Copied Successfull! [Time taken to copy file = %lds]\n", end - start);

    return 0;
}


void *copy_dir(void * args)
{
    struct dirent *dr;

    time_t start = time(0);
 
    struct thread_args * str = (struct thread_args *) args;

    char *src = str->src;
    char *dest = str->dest;

    DIR *source_dir = opendir(src);
    DIR *destination_dir = opendir(dest);


    if (source_dir == NULL)
    {
        perror("Could't open source directory");
        return 0    ;
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
                    str->src = concat(3, src, "/", dr->d_name);
                    str->dest = concat(3, dest, "/", dr->d_name);
                    copy_dir((void*) str);
                }
            }
            else{
                str->src = concat(3, src, "/", dr->d_name);
                str->dest = concat(3, dest, "/", dr->d_name);
                copy_file((void*) str);
            }
        }
    }
    
    closedir(source_dir);
    closedir(destination_dir);

    time_t end = time(0);

    // double calc_time = (end - start)  * 1000 / CLOCKS_PER_SEC;
    printf("Copied Successfull! [Time taken to copy file = %lds]\n", end - start);


    return 0;
}

//------------------------------------------------------



int  main(int argc , char* argv[]){ 
    if (argc < 4){
        perror("Needs 3 Arguments for copy commands\nEx: exeFile source_file/source_dir dest_file_1/dest_dir_1 dest_file_2/dest_dir_2");
        return 0;
    }

    int n = 2;

    pthread_t tid[n]; 

    for (int i = 0; i < n; i++){
        struct thread_args * t_arg = (struct thread_args * ) malloc(sizeof (struct thread_args));
        t_arg->src = argv[1];
        t_arg->dest = argv[i+2];

        if(is_regular_file(argv[1])){
            pthread_create(&tid[i], NULL, copy_file, (void *) t_arg); 
        }else if(isDirectory(argv[1])){
            pthread_create(&tid[i], NULL, copy_dir, (void *) t_arg); 
        }else{
            perror("File format is not regular or directory!");
        }

    }

    for (int i = 0; i < n; i++){
        pthread_join(tid[i], NULL);
    }

    printf("\n[!] Thread copy process completed!!!\n");

    return 0;
}
