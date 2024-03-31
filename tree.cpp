#include<bits/stdc++.h>
#include <fstream>
#include "diff.cpp"
#include "sha.cpp"


class file_blob{
    public:
        fs::path filepath;
        std::string hash;
        file_blob(std::string f_path,std::string hash){
            this->filepath=f_path;
            this->hash = hash;
        }
        bool operator<(const file_blob& other) const {
        return filepath < other.filepath;
    }
};

class Treeobject{
    public: 
       
        std::set<file_blob> fb;
        std::string message;
        std:: shared_ptr<Treeobject> next;

        Treeobject(std:: string message){
            
            this->message= message;
            this->next =nullptr;

        }
        void insert_blob(fs::path f_path){
            std::string stage_path=f_path.parent_path()/fs::path(".stage");//this acts as base to the stage file change it 
            std::string stage_file= stage_path / f_path.filename();
            std::string hash = computeSHA256(f_path);
            fs::path diff_base=fs::current_path()/fs::path(".diff");
            write_delta_from_file_name(stage_file,f_path,diff_base/fs::path(f_path.filename().string()+".txt"));
            
            fb.insert(file_blob(diff_base/fs::path(hash+".txt"),hash));
        }

};

void traverse_make_tree_obj(const fs::path& root,Treeobject &tee) {
    
    for (const auto& entry : fs::directory_iterator(root)) {
        std::cout<<entry.path()<<std::endl;
        tee.insert_blob(entry.path());
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


};



int main(){
    Treeobject tee = Treeobject("hello");
    traverse_make_tree_obj(fs::current_path(),tee);
}
