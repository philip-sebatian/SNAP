#include<bits/stdc++.h>
#include <fstream>
//class for the actions


class actions{
    
    public:
    std::string letter ;
    int idx ;
    std::string action;
        actions(std::string letter,int idx,std::string action){
            this->letter=letter;
            this->idx=idx;
            this->action=action;
        }
};

//deque to store the actions
std::deque<actions> stk;
std::vector<std::string> add_line(std::vector<std::string>&,std::string ,int );
std::vector<std::string> delete_line(std::vector<std::string>&,int);
//calculates the dp for diff
std::vector<std::vector<int>> calc(std::vector<std::string> s1,std::vector <std::string> s2){
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


//gets the actions needed to be performed

void get_actions(std::vector<std::vector<int>>&dp,std::vector<std::string> &s1,std::vector<std::string> &s2){
    int i =s1.size();
    int j =s2.size();
    while(i>0 && j>0){
        if(s1[i-1]==s2[j-1]){
            i--;
            j--;
        }
        else if(dp[i-1][j]+1==dp[i][j]){
            
            stk.push_back(actions(s1[i-1],i-1,"remove"));
            i--;
        }
        else{
            stk.push_back(actions(s2[j-1],i,"add"));
            j--;
        }
        
    }
    while(i>0){
        stk.push_back(actions(s1[i-1],i-1,"remove"));
        i--;
    }
    while(j>0){
        stk.push_back(actions(s2[j-1],i,"add"));
        j--;
    }
    while(!stk.empty()){
        std::cout<<stk.front().action<<" "<<stk.front().letter<<" at index-"<<stk.front().idx<<std::endl;
        if(stk.front().action=="remove"){
            delete_line(s1,stk.front().idx);
        }
        else{
            add_line(s1,stk.front().letter,stk.front().idx);
        }
        stk.pop_front();
    }
}


//gets the content of the files of a file as array of strings
std::vector<std::string> get_content(std::string fname){
    std:: fstream ff ;
    ff.open(fname,std::ios::out|std::ios::in);
    std::string str ;
    std::vector <std::string>lines;
    while(getline(ff,str)){
    
        std::cout<<str<<std::endl;
        lines.push_back(str);
   }
   return lines;

}
std::vector<std::string> add_line(std::vector<std::string>&file_content,std::string s1,int idx){
    std::vector<std::string> temp;
    bool flg=false;
    for(int i =0 ; i < file_content.size();i++){
        if(i==idx){
            
            if(!flg){
                temp.push_back(s1);
                i--;
                }
            flg=true;
            continue;
        }
        temp.push_back(file_content[i]);
    }
    file_content=temp;
    return file_content;
}

std::vector<std::string> delete_line(std::vector<std::string>&file_content,int idx){
    std::vector<std::string> temp;
    for(int i =0 ; i < file_content.size();i++){
        if(i==idx){
            continue;
        }
        temp.push_back(file_content[i]);
    }
    file_content=temp;
    return file_content;
}

int main()
{
   
   
   std ::string str ;
   std::vector<std::string> lines1;
   std::vector<std::string> lines2;
   lines1=get_content("hello.txt");
   lines2=get_content("new.txt");
   std::vector<std::vector<int>> dp =calc(lines1,lines2);
   get_actions(dp,lines1,lines2);
   for(auto i : lines1){
    std::cout<<i<<std::endl;
   }
   
   


}