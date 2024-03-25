    
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file.c"

int top=0;
int get_count(char *);
char **get_files(char *);

int **get_dp_file(char *, char *);
char *delete_letter(char *, int);
char *add_letter(char *, int, char);
typedef struct act{
  char action;
  char *letter ;
  int idx;
}actions;
actions action_stack[1024];
actions * get_action_stack(int size){
  actions * action_stack = (actions *)malloc(size*sizeof(actions));
  return action_stack;
}
char * delete_letter(char * word ,int idx){
  int len = strlen(word);
  char *temp=(char *)malloc((len-1)*sizeof(char));
  int j=0;
  for(int i =0; i < len;i++){
      if(i==idx)continue;
      temp[j]=word[i];
      j++;
  }
  
  return temp;
}

char * add_letter(char * word,int idx,char c ){
  int len = strlen(word);
  char*temp=(char *)malloc((len+2)*sizeof(char));
  strcpy(temp,word);
  for(int i =len;i>idx;i--){
    temp[i]=temp[i-1];
  }
  temp[idx]=c;
  temp[len+1]='\0';
  return temp;

}
//function for getting  the dp pointer
int ** get_dp(char * word1,char *word2){
  int l1 = strlen(word1);
  int l2 = strlen(word2);
  //dynamically allocates a 2d array for computation of dp
  int **dp=(int**)malloc((l1+1)*sizeof(int *));
  for(int i =0 ; i< l1+1;i++){
    dp[i]=(int*)malloc((l2+1)*sizeof(int *));
  }
  //fills the base cases in the dp table and also fills other values with zeroes
  for(int i =0 ; i < l1+1;i++){
    for(int j=0 ; j < l2+1;j++){
      dp[i][j]=0;
    }
  }
  for(int i=0 ; i<l1+1;i++){
    dp[i][0]=i;
  }
  for(int i =0; i <l2+1;i++){
    dp[0][i]=i;
  }
  
  //the dp loop for calculating distance of the strings
  for(int i =1; i<l1+1;i++){
    for(int j=1 ; j<l2+1;j++){
      if(word1[i-1]==word2[j-1]){

        dp[i][j]=dp[i-1][j-1];
      }
      else{
        if(dp[i-1][j]<dp[i][j-1]){
          dp[i][j]=dp[i-1][j]+1;
        }
        else{
          dp[i][j]=dp[i][j-1]+1;
        }
      }
    }
  }
  return dp;

}


int ** get_dp_file(char *word1,char *word2){
  int l1 = get_count(word1);
  int l2 = get_count(word2);


  char ** w1=get_files(word1);
  char ** w2=get_files(word2);
  //dynamically allocates a 2d array for computation of dp
  int **dp=(int**)malloc((l1+1)*sizeof(int *));
  for(int i =0 ; i< l1+1;i++){
    dp[i]=(int*)malloc((l2+1)*sizeof(int *));
  }
  //fills the base cases in the dp table and also fills other values with zeroes
  for(int i =0 ; i < l1+1;i++){
    for(int j=0 ; j < l2+1;j++){
      dp[i][j]=0;
    }
  }
  for(int i=0 ; i<l1+1;i++){
    dp[i][0]=i;
  }
  for(int i =0; i <l2+1;i++){
    dp[0][i]=i;
  }
  
  //the dp loop for calculating distance of the strings
  for(int i =1; i<l1+1;i++){
    for(int j=1 ; j<l2+1;j++){
      if(strcmp(w1[i-1],w2[j-1])==0){

        dp[i][j]=dp[i-1][j-1];
      }
      else{
        if(dp[i-1][j]<dp[i][j-1]){
          dp[i][j]=dp[i-1][j]+1;
        }
        else{
          dp[i][j]=dp[i][j-1]+1;
        }
      }
    }
  }
  return dp;

}

//for tracing actions on action stack array dynamically alocated by actions
actions * trace_actions(actions * action_stack,int **dp,char **word1,char **word2,char *v1,char *v2){
  int i =get_count(v1);
  actions temp_action ;
  int j =get_count(v2);
  int curr =i;
  while(i>0 && j>0){
    if(strcmp(word1[i-1],word2[j-1])==0){
      i--;
      j--;
    }
    else{
      if(dp[i-1][j]+1==dp[i][j] && i!=0){
        temp_action.action='r';
        temp_action.letter=word1[i-1];
        temp_action.idx=i-1;
        action_stack[top]=temp_action;
        printf("%s-%s\n","remove",word1[i-1]);
        i--;

        top++;
      }
      else{
        if(j==0){
          continue;
        }
          curr=i;
          temp_action.action='a';
          temp_action.letter=word2[j-1];
          temp_action.idx=i;
          action_stack[top]=temp_action;
          printf("%s-%s\n","add",word2[j-1]);
          
          j--;
          top++;
      }
    }
  }
  printf("overrrrrrrrr\n");
  while(i>0){
    temp_action.action='r';
    temp_action.idx=i-1;
    temp_action.letter=word1[i-1];
    action_stack[top]=temp_action;
    printf("%s-%s\n","remove",word1[i-1]);
    top++;
    
    i--;
  }
  while(j>0){
    
    temp_action.action='a';
    temp_action.idx=i;
    temp_action.letter=word2[j-1];
    action_stack[top]=temp_action;
    printf("%s-%s\n","add",word2[j-1]);
    top++;
    
    j--;
  }
  return action_stack;
}
//main point
int main(int argc, char * argv[]){
  actions *action_stack=get_action_stack(1024);
  char * word1 = argv[1];
  char * word2 = argv[2];
  char **f1= get_files(word1);
  char **f2=get_files(word2);
  int l1 = get_count(word1);
  int l2 = get_count(word2);
  int **dp=get_dp_file(word1,word2);
  
  printf("\n\n");
  for(int i =0 ; i < l1+1;i++){
    for(int j=0 ; j < l2+1;j++){
      printf("%d\t",dp[i][j]);
    }
    printf("\n");
  }
  
  trace_actions(action_stack,dp,f1,f2,word1,word2);
  printf("\n\n");
 int curr =0 ;
 /*while(curr<top){
  if(action_stack[curr].action=='r'){
   printf("%s %s %s %d\n","remove",action_stack[curr].letter,"at index",action_stack[curr].idx);
  
   word1=delete_letter(word1,action_stack[curr].idx);
   
   
  }else{
   printf("%s %s %s %d\n","add",action_stack[curr].letter,"at index",action_stack[curr].idx);
   
   word1=add_letter(word1,action_stack[curr].idx,action_stack[curr].letter);
   
  }curr++;
  printf("%s\n",word1);
  
 }*/
 
 
 

 free(dp);
}
