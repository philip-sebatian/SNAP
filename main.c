    
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char  action_stack[1024];
int top=0;
int main(int argc, char * argv[]){
  char * word1 = argv[1];
  char * word2 = argv[2];
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
      printf("%d\t",dp[i][j]);
    }
    printf("\n");
  }
  for(int i=0 ; i<l1+1;i++){
    dp[i][0]=i;
  }
  for(int i =0; i <l2+1;i++){
    dp[0][i]=i;
  }
  printf("\n");
  //prints the dp table for debugging
  for(int i =0 ; i < l1+1;i++){
    for(int j=0 ; j < l2+1;j++){
      printf("%d\t",dp[i][j]);
    }
    printf("\n");
  }
  //the dp loop for calculating distance of the strings
  for(int i =1; i<l1+1;i++){
    for(int j=1 ; j<l2+1;j++){
      if(word1[i]==word2[j]){

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

  printf("\n\n");
  for(int i =0 ; i < l1+1;i++){
    for(int j=0 ; j < l2+1;j++){
      printf("%d\t",dp[i][j]);
    }
    printf("\n");
  }
  /*this is for backtracking the actions that were perforemed to obtain the string the s1*/
  int i =strlen(word1)+1;

  int j =strlen(word2)+1;
  while(i>0 && j>0){
    if(word1[i-1]==word2[j-1]){
      i--;
      j--;
    }
    else{
      if(dp[i-1][j]+1==dp[i][j] && i!=0){
        action_stack[top]='r';
        printf("%s-%c\n","remove",word1[i-1]);
        i--;
        top++;
      }
      else{
        if(j==0){
          continue;
        }
          action_stack[top]='a';
          printf("%s-%c\n","add",word2[j-1]);
        
          j--;
          top++;
      }
    }
  }

  while(i>0){
    action_stack[top]='r';
    printf("%s-%c\n","remove",word1[i-1]);
    top++;
    i--;
  }
  while(j>0){
    action_stack[top]='a';
    printf("%s-%c\n","add",word2[j-1]);
    top++;
    j--;
  }


 int curr =0 ;
 while(curr<=top){
   printf("%c\n",action_stack[curr]);
   curr++;
 }
 free(dp);
}
