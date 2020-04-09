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

#define COPYMORE 0644

#define BUF_SIZE 1024
using namespace std;

struct thread_args {
    char *src;
    char *dest;
    int a;
};


/**
 * isFile(string: path)
 * Returns 1 if file, 0 if directory, -1 if invalid.    
 */
int isFile(char * path){
    struct stat path_stat;
    stat(path, &path_stat);
    int isFile = S_ISREG(path_stat.st_mode);
    
    if (isFile) return 1;

    int isDir = S_ISDIR(path_stat.st_mode);

    if (isDir) return 0;
    
    return -1;
}


void * copyFile(void * arg){
    time_t   start = time(0);
 
    struct thread_args * str = (struct thread_args *) arg;

    char * source = str->src;
    char * dest = str->dest;

    printf("[!] Copying \"%s\" to \"%s\"\n", source, dest);


    if (isFile(dest) == -1){
        printf("\n[!] Creating file. %d", COPYMORE);
        int z = creat(dest, COPYMORE); // permission
    } 
    
    if (access(source, R_OK)){
        printf("\n[-] Permission denied.\n");
        return 0;
    }

    int sourceFD = open(source, O_RDONLY); // returns -1 if invalid.
    int destFD = open(dest, O_CREAT | O_WRONLY, 0644);

    if (sourceFD == -1 || destFD == -1){
        printf("\n[-] Error.");
        return 0;
    }
    
    char * buf = (char*) calloc(1024, sizeof(char));
    size_t read_status = 1;

    while (read_status != -1 && read_status != 0){

        read_status = read(sourceFD, buf, 1023);        
        size_t write_status = write(destFD, buf, read_status);
        if (write_status == -1) { 
            printf("\n[-] Error.");
            return 0;
        }

        // printf("%ld ", read_status);
    }

    //printf("\nReading\n.. %ld bytes read. \n%s", read_status, buf);
    //printf(" -- FD= %d -- ", sourceFD);   
    time_t end = time(0);
    double datetime_diff_ms = (end - start)  * 1000 / CLOCKS_PER_SEC;
    printf("[!] Done [Time taken to copy file = %lds]\n", time(0) - start);
}

  


// struct thread_args t_arg1;
// struct thread_args t_arg2;

// void *copy_file(char *src, char *dest)
// {
//     int fdold, fdnew;
//     fdold = open(src, O_RDONLY);

//     if (fdold == -1){
//         perror("Cannot Open File!");
//         exit(EXIT_FAILURE);
//     }

//     fdnew = creat(dest, 0666);

//     if (fdnew == -1){
//         printf("Cannot Create File %s!", dest);
//         exit(EXIT_FAILURE);
//  }
//     char buffer[BUF_SIZE];
//     int count;
//     while ((count = read(fdold, buffer, sizeof(buffer) ))> 0)
//     {
//         write(fdnew, buffer, count);
//     }
//     printf("Copied Successfull\n");

//     exit(EXIT_SUCCESS);
// }
 

int  main(int argc , char** argv){
    if (argc != 4){
        printf("[!] Error, command requires two arguments");
        printf("\n\t\tcp source destination1 destination2");
        return 0;
    }



    // t_arg1.src = "aaa";
    // t_arg1.src = "aaa";
    // t_arg1.a = 1;

    int n = 2;
    pthread_t tid[n]; 

    for (int i = 0; i < n; i++){
        struct thread_args * t_arg1 = (struct thread_args * ) malloc(sizeof (struct thread_args));
        t_arg1->src = argv[1];
        t_arg1->dest = argv[i+2];

        pthread_create(&tid[i], NULL, copyFile, (void *) t_arg1); 

    }

    for (int i = 0; i < n; i++) pthread_join(tid[i], NULL);

    printf("\nzzz");

	// t_arg1.src = t_arg2.src = argv[1];
	// t_arg1.dest = argv[2];
	// t_arg2.dest = argv[3]; 
	// pthread_t t1,t2;
	// pthread_create(&t1,NULL, copy_file, &t_arg1);
    // t_arg1.src = "aaa";
	// pthread_create(&t2,NULL, copy_file, &t_arg2);
}
