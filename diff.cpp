#include<bits/stdc++.h>
#include <fstream>
#include "file.cpp"
//class for the actions
namespace fs = std::filesystem;

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
//s1 is the line that is to be changed and s2 is the line that is to be searched to 
std::deque<actions> get_actions(std::vector<std::vector<int>>&dp,std::vector<std::string> &s1,std::vector<std::string> &s2){
    std::deque<actions> stk;
    int i =s1.size();
    int j =s2.size();
    while(i>0 && j>0){
        if(s1[i-1]==s2[j-1]){
            i--;
            j--;
        }
        else if(dp[i-1][j]+1==dp[i][j]){
            
            stk.push_back(actions(s1[i-1],i-1,"-"));
            i--;
        }
        else{
            stk.push_back(actions(s2[j-1],i,"+"));
            j--;
        }
        
    }
    while(i>0){
        stk.push_back(actions(s1[i-1],i-1,"-"));
        i--;
    }
    while(j>0){
        stk.push_back(actions(s2[j-1],i,"+"));
        j--;
    }
    return stk;
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
//adds line from content vector
std::vector<std::string> add_line(std::vector<std::string>&file_content,std::string s1,int idx){
    std::vector<std::string> temp;
    bool flg=false;
    for(int i =0 ; i < file_content.size();i++){
        if(i==idx){
            
            if(!flg){
                temp.push_back(s1);
                
                flg=true;
                
                }
            
            
        }
        temp.push_back(file_content[i]);
    }
    if (!flg) {
        temp.push_back(s1); 
    }
    file_content=temp;
    return file_content;
}
//deltes line from content vector 
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
//recontructs the the action deque from the delta file with name equals fname 
std::set<fs::path> get_fs(std::string delta){
    std:: fstream ff ;
    ff.open(delta,std::ios::out|std::ios::in);
    std::string strbuffer;
    getline(ff,strbuffer);
    std::string str =strbuffer.substr(2);
    std::set<fs::path> tokens;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, ',')) {
        tokens.insert(fs::path(token));
    }
    return tokens;

}
std::deque<actions> reconstruct_delta_file(std::string fname){
    std::deque<actions> stk;
    std:: fstream ff ;
    ff.open(fname,std::ios::out|std::ios::in);
    std::string str ;
    std::string temp_line;
    int idx;
    std::string action;
    
    while(getline(ff,str)){
        temp_line=str.substr(2);
        action=str[0];
        idx=str[1];
        
        stk.push_back(actions(temp_line,idx-'0',action));
   }
   return stk;
}
void write_fs_delta(std::string delta){
    std::set<std::filesystem::path> filestruct ;
    traverse(std::filesystem::current_path(),filestruct);
    std::string fss="$";
    for(auto i :filestruct){
        fss+=","+i.string();
    }
    fss+="\n";
    std:: fstream ff ;
    ff.open(delta,std::ios::out|std::ios::trunc);
    std::string str ;
    ff<<fss;
    ff.seekg(0);
}
//takes in the action dequeue and writes delta file with the name passed in the parameter fname
void write_delta_from_actionstk(std::string fname,std::deque<actions>stk){
    std::set<std::filesystem::path> filestruct ;
    
    std:: fstream ff ;
    ff.open(fname,std::ios::out|std::ios::trunc);
    std::string str ;
    
    while(!stk.empty()){
        str=stk.front().action+std::to_string(stk.front().idx)+stk.front().letter+"\n";
        stk.pop_front();
        ff<<str;
    }
    ff.close();
    std::cout<<"finished writing"<<std::endl;
}
void write_delta_from_file_name(std::string f1,std::string f2,std::string delta){
    std::vector<std::string> lines1;
    std::vector<std::string> lines2;
    lines1=get_content(f1);
    lines2=get_content(f2);
    std::vector<std::vector<int>> dp =calc(lines1,lines2);
    if(dp[lines1.size()][lines2.size()]==0){
        return;
    }
    std::deque<actions> ssk=get_actions(dp,lines1,lines2);
    write_delta_from_actionstk(delta,ssk);
}

void create_change(std::set<fs::path> file_s){
    std::set<fs::path> c_flst;
    traverse(fs::current_path(),c_flst);
    for(const auto &i :file_s){
        std::cout<<"contents "<<i<<"\n";
        if(c_flst.find(i)==c_flst.end()){
            if(fs::is_directory(fs::current_path()/i)){
                if(!fs::exists(fs::current_path()/i)){
                    std::cout<<"add "<<i<<"\n";
                    fs::create_directories(fs::current_path() / i);
                }
            }
        }
    }

    for(const auto &i : c_flst){
        std::cout<<"conetnt B "<<i<<"\n";
        if(file_s.find(i)==file_s.end()){
                std::cout<<"remove "<<i<<"\n";
                fs::remove_all(fs::current_path()/i);
            
        }
    }

}
/*int main()
{
   //write_delta_from_file_name("he;lo.txt","new.txt","delta3.txt");
   //write_fs_delta("file.txt");
   //auto i = get_fs("file.txt");
    //create_change(i);
    auto i = reconstruct_delta_file("delta3.txt");
    for(auto j :i){
        std::cout<<j.letter<<"\n";
    }
}*/