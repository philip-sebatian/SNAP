#include<bits/stdc++.h>


class actions{
    
    public:
    char letter ;
    int idx ;
    std::string action;
        actions(char letter,int idx,std::string action){
            this->letter=letter;
            this->idx=idx;
            this->action=action;
        }
};


std::stack<actions> stk;
std::vector<std::vector<int>> calc(std::string s1,std::string s2){
    std::vector<std::vector<int>>dp(s1.size()+1,std::vector<int>(s2.size()+1,0));
    for(int i=0 ; i < s1.size()+1;i++){
        dp[i][0]=i;
    }
    for(int i =0 ; i<s2.size()+1;i++){
        dp[0][i]=i;
    }

    for(int i =1 ; i<s1.size()+1;i++){
        for(int j =1 ; j < s2.size()+1;j++){
            if(s1[i-1]==s2[j-1]){
                dp[i][j]=dp[i-1][j-1];
            }
            else{
                dp[i][j]=std::min(dp[i-1][j],dp[i][j-1])+1;
            }
        }
    }


    for(int i=0 ; i<s1.size()+1;i++){
        for(int j=0 ; j<s2.size()+1;j++){
            std::cout<<dp[i][j]<<"\t";
        }
        std::cout<<std::endl;
    }
    return dp;


    
}

void get_actions(std::vector<std::vector<int>>&dp,std::string s1,std::string s2){
    int i =s1.size()+1;
    int j =s2.size()+1;
    while(i>0 && j>0){
        if(s1[i-1]==s2[j-1]){
            i--;
            j--;
        }
        else if(dp[i-1][j]+1==dp[i][j]){
            
            stk.push(actions(s1[i-1],i-1,"remove"));
            i--;
        }
        else{
            stk.push(actions(s2[j-1],i,"add"));
            j--;
        }
    }
    while(i>0){
        stk.push(actions(s1[i-1],i-1,"remove"));
        i--;
    }
    while(j>0){
        stk.push(actions(s2[j-1],i,"add"));
        j--;
    }
    while(!stk.empty()){
        std::cout<<stk.top().action<<" "<<stk.top().letter<<" at index-"<<stk.top().idx<<std::endl;
        stk.pop();
    }
}

int main()
{
   std :: vector<std::vector<int>> dp = calc("hello","hi");
   get_actions(dp,"hello","hi");

}