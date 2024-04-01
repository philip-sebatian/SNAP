#include<bits/stdc++.h>
#include <fstream>
#include "diff.cpp"
#include "sha.cpp"


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
        void insert_blob(fs::path f_path,std::string commit_name){
            std::string stage_path=f_path.parent_path()/fs::path(".stage");//this acts as base to the stage file change it 
            std::string stage_file= stage_path / f_path.filename();
            std::string hash = computeSHA256(f_path);
            this->commit_name=commit_name;
            fs::path diff_base=fs::current_path()/fs::path(".diff")/fs::path(commit_name);
            if(!fs::exists(diff_base)){
                fs::create_directories(diff_base);
            }
            fs::path file_struct_path=fs::current_path()/fs::path(".file");
            write_fs_delta(file_struct_path/fs::path(commit_name+".txt"));
            write_delta_from_file_name(stage_file,f_path,diff_base/fs::path(f_path.filename().string()+".txt"));
            
            fb.insert(file_blob(fs::relative(f_path,fs::current_path()),diff_base/fs::path(f_path.string()+".txt")));
        }
        void make_fs(){
            auto fs_set =get_fs(fs::current_path()/ fs::path(".file")/fs::path(commit_name+".txt"));
            create_change(fs_set,fs::current_path()/fs::path(".stage"));
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
    for(auto i : tee.fb){
        std::cout<<i.filepath<<std::endl;
    }
    tee.make_fs();
}
