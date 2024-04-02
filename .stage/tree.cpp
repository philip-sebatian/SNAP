#include<bits/stdc++.h>
46    //esrtdfyguhlkj;,
#include <fstream>
#include "diff.cpp"



class file_blob{
    public:
        fs::path filepath;
        std:: string f_name ;
        file_blob(std::string f_path,std::string f_name){
            this->filepath=f_path;
            this->f_name=f_name;
            
        }
        bool operator<(const file_blob& other) const {
        return filepath < other.filepath;
    }
};

class Treeobject{
    public: 
       
        std::set<file_blob> fb;
        std::string commit_name;
        std:: shared_ptr<Treeobject> next;

        Treeobject(){
            
            
            this->next =nullptr;

        }
        //f_path path to relative
        std::vector<std::string> get_vector_from_delta(fs::path f_path){
            fs::path stage_base=fs::current_path()/fs::path(".stage");
            auto content=get_content(stage_base/f_path);
            std::cout<<"////////////////////////////"<<content.size()<<std::endl;
            std::cout<<stage_base/f_path<<std::endl;
            fs::path diff_base=fs::current_path()/fs::path(".diff")/fs::path(commit_name);
            auto delta=get_content(diff_base/fs::path(f_path.string()+".txt"));
            std::cout<<"cccccc "<<diff_base/fs::path(f_path.string()+".txt")<<std::endl;
            std::cout<<content.size()<<std::endl;
            for(auto i : delta){
                
                if(i[0]=='+'){
                    content.insert(content.begin()+i[1]-'0',i.substr(2));
                }
                else{
                    content.erase(content.begin()+i[1]-'0');
                }
            }
            std::cout<<"////////////////////////////"<<content.size()<<"poooop"<<std::endl;
            return content;
            
            
        }
        void write_vector(std::vector<std::string> cont,fs::path f_path){

            std:: fstream ff ;
            
            ff.open(f_path,std::ios::out|std::ios::trunc);
            for(auto i :cont){
                ff<<(i+"\n");
            }
            ff.close();
        }
        void insert_blob(fs::path f_path,std::string commit_name){
            std::string stage_path=f_path.parent_path()/fs::path(".stage");//this acts as base to the stage file change it 
            std::cout<<"biasbiabsdasdnskad=------"<<stage_path /fs::relative(f_path, fs::current_path())<<std::endl;
            std::string stage_file= stage_path /fs::relative(f_path,fs::current_path());
            
            this->commit_name=commit_name;
            std::cout<<"ssssssssssssssssssss =="<<stage_file<<std::endl;
            fs::path diff_base=fs::current_path()/fs::path(".diff")/fs::path(commit_name);
            if(!fs::exists(diff_base)){
                fs::create_directories(diff_base);
            }
            fs::path file_struct_path=fs::current_path()/fs::path(".file");
            write_fs_delta(file_struct_path/fs::path(commit_name+".txt"));
            
            write_delta_from_file_name(stage_file,f_path,diff_base/fs::path(""+f_path.filename().string()+""".txt"));
            
            fb.insert(file_blob(fs::relative(f_path,fs::current_path()),diff_base/fs::path(f_path.filename().string()+".txt")));
        }
        void make_fs(){
            auto fs_set =get_fs(fs::current_path()/ fs::path(".file")/fs::path(commit_name+".txt"));
            create_change(fs_set,fs::current_path()/fs::path(".stage"));
            for(auto i : fb){
                std::cout<<i.filepath<<std::endl;
                auto vec =get_vector_from_delta(i.filepath.filename());
                std::cout<<"size="<<vec.size()<<std::endl;
                write_vector(vec,fs::current_path()/fs::path(".stage")/i.filepath);
                
                
            }
        }


};

void traverse_make_tree_obj(const fs::path& root,Treeobject &tee) {
    
    for (const auto& entry : fs::directory_iterator(root)) {
        std::cout<<entry.path()<<std::endl;
        if(fs::is_regular_file(entry))

        tee.insert_blob(entry.path(),"noob");
        if(fs::is_directory(entry)){
            if(entry.path().filename().string()[0]!='.'){
                traverse_make_tree_obj(entry.path(),tee);
            }
        }
    }
    
    
}

class Tree{
    std::shared_ptr<Treeobject> head;
    std::unordered_map <std::string,std::shared_ptr<Treeobject>> branches;
    std::string current_branch="master";

    void snapshot(std::string snap_name){
        if(head==nullptr){
             head = std::make_shared<Treeobject>();
             branches[current_branch]=head;


        }
        
    }


};



int main(){
    Treeobject tee = Treeobject();
   traverse_make_tree_obj(fs::current_path(),tee);
   // auto  i =get_content("/home/ubuntu/cppgit/v-control/.diff/noob/diff.cpp.txt");
    tee.make_fs();
    
      //  std::cout<<i.size()<<std::endl;
    //sadjh
   
}
