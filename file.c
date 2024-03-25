#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int get_count(char *);
char buffer [1024];
char **  get_files(char * f_name){
    FILE * f = fopen(f_name,"r");
    int count=get_count(f_name);
   
    char ** arr = (char **)malloc(sizeof(char *)*count);
    
    char *temp;
    int j=0;
    while(fgets(buffer,sizeof(buffer),f)){
        arr[j]=malloc(sizeof(buffer)+1);
        strcpy(arr[j],buffer);
        j++;
    }  
    fseek(f,0,SEEK_SET);
    return arr;
    
}


int get_count(char * f_name){
    FILE * f = fopen(f_name,"r");
    int count=0;
    while(fgets(buffer,sizeof(buffer),f)){
        count++;
    }   
    fseek(f,0,SEEK_SET);
    return count;
}
