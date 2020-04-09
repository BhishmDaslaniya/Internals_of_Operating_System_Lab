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

void list_dir(char *path) {
  struct dirent *entry;
  DIR *dir = opendir(path);
   
  if(dir!=NULL){
    while ((entry = readdir(dir)) != NULL) {
      cout<<"-> "<< entry->d_name << endl;
    }
    closedir(dir);
  }else{
    perror("could not open directory");
    return;
  }
}

int main(int argc, char *argv[]) {
   list_dir(argv[1]);
}
