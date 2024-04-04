#include<bits/stdc++.h>
#include <fstream>
#include "diff.cpp"
#include "json.hpp"

using json = nlohmann::json;

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
        json to_json() const {
        json j;
        // Serialize file_blob members into JSON
        j["filepath"] = filepath.string();
        j["f_name"] = f_name;
        return j;
    }
     static file_blob from_json(const json& j) {
        // Extract data from JSON and construct file_blob instance
        std::string filepath_str = j.at("filepath").get<std::string>();
        std::string f_name = j.at("f_name").get<std::string>();
        return file_blob(filepath_str, f_name);
    }

};

class Treeobject{
    public: 
       
        std::set<file_blob> fb;
        std::string commit_name;
        std:: shared_ptr<Treeobject> next;
        std:: shared_ptr<Treeobject> prev;

        Treeobject(std::string commit_name ){
            
            this->commit_name=commit_name;
            this->next =nullptr;
            this->prev=nullptr;

        }
        json to_json() const {
        json j;
        // Serialize Treeobject members into JSON
        j["commit_name"] = commit_name;
        // Serialize set of file_blob objects into JSON array
        for (const auto& blob : fb) {
            j["fb"].push_back(blob.to_json());
        }
        return j;
    }

    // Deserialization from JSON
    static Treeobject from_json(const json& j) {
        // Extract data from JSON and construct Treeobject instance
        std::string commit_name = j.at("commit_name").get<std::string>();
        Treeobject tree(commit_name);
        // Deserialize set of file_blob objects from JSON array
        for (const auto& blob_json : j.at("fb")) {
            tree.fb.insert(file_blob::from_json(blob_json));
        }
        return tree;
    }
        //f_path path to relative
        std::vector<std::string> get_vector_from_delta(fs::path f_path){
            fs::path stage_base=fs::current_path()/fs::path(".pgit")/fs::path("stage");
            auto content=get_content(stage_base/f_path);
            std::cout<<"////////////////////////////"<<content.size()<<std::endl;
            std::cout<<stage_base/f_path<<std::endl;
            fs::path diff_base=fs::current_path()/fs::path(".pgit")/fs::path("diff")/fs::path(this->commit_name);
            auto delta=get_content(diff_base/fs::path(f_path.string()+".txt"));
            std::cout<<"cccccc "<<diff_base/fs::path(f_path.string()+".txt")<<std::endl;
            std::cout<<content.size()<<std::endl;
            for(auto i : delta){
                int j =1;
                std::string temp="";
                while(i[j]!='$'){
                    temp+=i[j];
                    j++;
                }
                j++;
                if(i[0]=='+'){

                    content.insert(content.begin()+std::stoi(temp),i.substr(j));
                }
                else{
                    content.erase(content.begin()+std::stoi(temp));
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
        void insert_blob(fs::path f_path){
            std::string stage_path=fs::current_path()/fs::path(".pgit")/fs::path("stage");//this acts as base to the stage file change it 
            std::cout<<"biasbiabsdasdnskad=------"<<stage_path /fs::relative(f_path, fs::current_path())<<std::endl;
            std::string stage_file= stage_path /fs::relative(f_path,fs::current_path());
            
            
            std::cout<<"ssssssssssssssssssss =="<<stage_file<<std::endl;
            fs::path diff_base=fs::current_path()/fs::path(".pgit")/fs::path("diff")/fs::path(this->commit_name);
            if(!fs::exists(diff_base)){
                fs::create_directories(diff_base);
            }
            fs::path file_struct_path=fs::current_path()/fs::path(".pgit")/fs::path("file");
            write_fs_delta(file_struct_path/fs::path(this->commit_name+".txt"));
            
            write_delta_from_file_name(stage_file,f_path,diff_base/fs::path(""+f_path.filename().string()+""".txt"));
            
            fb.insert(file_blob(fs::relative(f_path,fs::current_path()),diff_base/fs::path(f_path.filename().string()+".txt")));
        }
        void make_fs(){
            auto fs_set =get_fs(fs::current_path()/fs::path(".pgit")/ fs::path("file")/fs::path(this->commit_name+".txt"));
            create_change(fs_set,fs::current_path()/fs::path(".pgit")/fs::path("stage"));
            for(auto i : fb){
                std::cout<<i.filepath<<std::endl;
                auto vec =get_vector_from_delta(i.filepath.filename());
                std::cout<<"size="<<vec.size()<<std::endl;
                write_vector(vec,fs::current_path()/fs::path(".pgit")/fs::path("stage")/i.filepath);
                
                
            }
        }

    void traverse_make_tree_obj(const fs::path& root) {
    
    for (const auto& entry : fs::directory_iterator(root)) {
        std::cout<<entry.path()<<std::endl;
        if(fs::is_regular_file(entry))

        insert_blob(entry.path());
        if(fs::is_directory(entry)){
            if(entry.path().filename().string()[0]!='.'){
                traverse_make_tree_obj(entry.path());
            }
        }
    }
    
    
}


};

void traverse_make_tree_obj(const fs::path& root,Treeobject &tee) {
    
    for (const auto& entry : fs::directory_iterator(root)) {
        std::cout<<entry.path()<<std::endl;
        if(fs::is_regular_file(entry))

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
    std::string current_branch="master";

    void snapshot(std::string snap_name){
        if(head==nullptr){
             head = std::make_shared<Treeobject>(snap_name);
             branches[current_branch]=head;
        }
        else{
            std::shared_ptr<Treeobject> new_node = std::make_shared<Treeobject>(snap_name);
            auto old = branches[current_branch];
            old->next=new_node;
            new_node->prev=old;
            
        }
        
    }


};



int main(){
    Treeobject tee = Treeobject("hello");
    Treeobject tee2 = Treeobject("hi");
   tee.traverse_make_tree_obj(fs::current_path());
   // auto  i =get_content("/home/ubuntu/cppgit/v-control/.diff/noob/diff.cpp.txt");
   tee.make_fs();
   tee2.traverse_make_tree_obj(fs::current_path());
   tee2.make_fs();
   for(auto i :tee.fb){
    std::cout<<i.filepath<<std::endl;
   }
    json tree_json = tee.to_json();
    // Output JSON string
    std::cout << tree_json.dump(4) << std::endl;
    // Deserialize JSON string to Treeobject
    Treeobject new_tree = Treeobject::from_json(tree_json);
    return 0;
    //sadjh
    //esrtdfyguhlkj;,
   
}
