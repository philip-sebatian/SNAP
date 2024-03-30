#include <filesystem>
#include <bits/stdc++.h>
namespace fs = std::filesystem;

// Function to traverse a folder and construct its structure
void traverse(const fs::path& root,std::set<fs::path>& filestruct) {
    
    for (const auto& entry : fs::directory_iterator(root)) {
        if(entry.path().filename().string()[0]!='.')
        filestruct.insert(entry);
        if(fs::is_directory(entry)){
            if(entry.path().filename().string()[0]!='.'){
                traverse(entry,filestruct);
            }
        }
    }
    
    
}


