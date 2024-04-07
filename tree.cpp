#include <bits/stdc++.h>
#include <fstream>
#include "diff.cpp"
#include "json.hpp"

using json = nlohmann::json;

class file_blob
{
public:
    fs::path filepath;
    std::string f_name;
    file_blob(std::string f_path, std::string f_name)
    {
        this->filepath = f_path;
        this->f_name = f_name;
    }
    bool operator<(const file_blob &other) const
    {
        return filepath < other.filepath;
    }
    json to_json() const
    {
        json j;
        // Serialize file_blob members into JSON
        j["filepath"] = filepath.string();
        j["f_name"] = f_name;
        return j;
    }
    static file_blob from_json(const json &j)
    {
        // Extract data from JSON and construct file_blob instance
        std::string filepath_str = j.at("filepath").get<std::string>();
        std::string f_name = j.at("f_name").get<std::string>();
        return file_blob(filepath_str, f_name);
    }
};

std::string removeExtension(const std::string& filename) {
    size_t lastDotPos = filename.find_last_of(".");
    if (lastDotPos != std::string::npos) {
        return filename.substr(0, lastDotPos);
    }
    // If no extension found, return the original filename
    return filename;
}


std::string get_delta_fname(std::string path)
{
    std::string str = "";
    for (auto i : path)
    {
        if (i == '/')
        {
            continue;
        }
        str += i;
    }
    return str;
}

class Treeobject
{
public:
    std::set<file_blob> fb;
    std::string commit_name;
    std::shared_ptr<Treeobject> next = nullptr;
    std::shared_ptr<Treeobject> prev = nullptr;

    Treeobject(std::string commit_name)
    {

        this->commit_name = commit_name;
        this->next = nullptr;
        this->prev = nullptr;
    }
    Treeobject() = default;
    json to_json() const
    {
        json j;
        // Serialize Treeobject members into JSON
        j["commit_name"] = commit_name;
        // Serialize set of file_blob objects into JSON array
        for (const auto &blob : fb)
        {
            j["fb"].push_back(blob.to_json());
        }
        // Serialize next if it exists
        if (next != nullptr)
        {
            j["next"] = (next)->to_json();
        }
        // Serialize prev if it exists
        if (prev != nullptr)
        {
            j["prev"] = prev->to_json();
        }
        return j;
    }

    // Deserialization from JSON
    static Treeobject from_json(const json &j)
    {
        // Extract data from JSON and construct Treeobject instance
        std::string commit_name = j.at("commit_name").get<std::string>();
        Treeobject tree(commit_name);

        // Deserialize set of file_blob objects from JSON array
        for (const auto &blob_json : j.at("fb"))
        {
            tree.fb.insert(file_blob::from_json(blob_json));
        }

        // Deserialize next if it exists
        if (j.find("next") != j.end())
        {
            tree.next = std::make_shared<Treeobject>(Treeobject::from_json(j.at("next")));
        }

        // Deserialize prev if it exists
        if (j.find("prev") != j.end())
        {
            tree.prev = std::make_shared<Treeobject>(Treeobject::from_json(j.at("prev")));
        }

        return tree;
    }

    // f_path path to relative
    std::vector<std::string> get_vector_from_delta(file_blob ff)
    {
        fs::path stage_base = fs::current_path() / fs::path(".pgit") / fs::path("stage");
        auto content = get_content(stage_base / ff.filepath);
        fs::path diff_base = fs::current_path() / fs::path(".pgit") / fs::path("diff") / fs::path(this->commit_name);
        auto delta = get_content(ff.f_name);
        for (auto i : delta)
        {
            int j = 1;
            std::string temp = "";
            while (i[j] != '$')
            {
                temp += i[j];
                j++;
            }
            j++;
            if (i[0] == '+')
            {

                content.insert(content.begin() + std::stoi(temp), i.substr(j));
            }
            else
            {
                content.erase(content.begin() + std::stoi(temp));
            }
        }

        return content;
    }
    void write_vector(std::vector<std::string> cont, fs::path f_path)
    {

        std::fstream ff;

        ff.open(f_path, std::ios::out | std::ios::trunc);
        for (auto i : cont)
        {
            ff << (i + "\n");
        }
        ff.close();
    }
    void insert_blob(fs::path f_path)
    {
        std::string stage_path = fs::current_path() / fs::path(".pgit") / fs::path("stage"); // this acts as base to the stage file change it

        std::string stage_file = stage_path / fs::relative(f_path, fs::current_path());

        fs::path diff_base = fs::current_path() / fs::path(".pgit") / fs::path("diff") / fs::path(this->commit_name);
        if (!fs::exists(diff_base))
        {
            fs::create_directories(diff_base);
        }
        fs::path file_struct_path = fs::current_path() / fs::path(".pgit") / fs::path("file");
        write_fs_delta(file_struct_path / fs::path(this->commit_name + ".txt"));

        write_delta_from_file_name(stage_file, f_path, diff_base / fs::path("" + get_delta_fname(f_path.string()) + ""
                                                                                                                    ".txt"));

        fb.insert(file_blob(fs::relative(f_path, fs::current_path()), diff_base / fs::path(get_delta_fname(f_path.string()) + ".txt")));
    }
    void make_fs()
    {
        auto fs_set = get_fs(fs::current_path() / fs::path(".pgit") / fs::path("file") / fs::path(this->commit_name + ".txt"));
        create_change(fs_set, fs::current_path() / fs::path(".pgit") / fs::path("stage"));
        for (auto i : fb)
        {
            auto vec = get_vector_from_delta(i);
            write_vector(vec, fs::current_path() / fs::path(".pgit") / fs::path("stage") / i.filepath);
        }
    }
    void make_fs_cc(fs::path base)
    {
        auto fs_set = get_fs(fs::current_path() / fs::path(".pgit") / fs::path("file") / fs::path(this->commit_name + ".txt"));
        create_change(fs_set, base);
        create_change(fs_set, fs::current_path() / fs::path(".pgit") / fs::path("stage"));
        for (auto i : fb)
        {

            auto vec = get_vector_from_delta(i);

            write_vector(vec, fs::current_path() / fs::path(".pgit") / fs::path("stage") / i.filepath);
            write_vector(vec, base / i.filepath);
        }
    }

    void traverse_make_tree_obj(const fs::path &root)
    {

        for (const auto &entry : fs::directory_iterator(root))
        {
            std::cout << entry.path() << std::endl;
            if (fs::is_regular_file(entry))

                insert_blob(entry.path());
            if (fs::is_directory(entry))
            {
                if (entry.path().filename().string()[0] != '.')
                {
                    traverse_make_tree_obj(entry.path());
                }
            }
        }
    }
    void snapshot(std::string name)
    {
        auto curr = std::make_shared<Treeobject>(*this);
        auto head = curr;
        while (curr->next != nullptr)
        {
            curr->make_fs();
            if (curr->commit_name == name)
            {
                std::cout << "snapshot not taken . snap with same name exits" << std::endl;
                return;
            }
            curr = curr->next;
        }
        curr->make_fs();
        if (curr->commit_name == name)
        {
            std::cout << "snapshot not taken . snap with same name exits" << std::endl;
            return;
        }
        curr->next = std::make_shared<Treeobject>(Treeobject(name));
        curr->next->traverse_make_tree_obj(fs::current_path());

        auto tree_json = head->to_json();
        auto path = (fs::current_path() / fs::path(".pgit") / fs::path("tree.json")).string();
        std::ofstream file(path);
        file << tree_json.dump(4); // Write formatted JSON to file
        file.close();
        for (const auto &entry : fs::directory_iterator(fs::current_path() / fs::path(".pgit") / fs::path("stage")))
        {
            fs::remove_all(entry);
        }
    }

    void roll_back(std::string name)
    {
        std::set<std::string> inclusion_set;
        auto curr = std::make_shared<Treeobject>(*this);
        auto head = curr;
        for (const auto &entry : fs::directory_iterator(fs::current_path()))
        {
            if (entry.path().filename().string()[0] != '.')
            {
                fs::remove_all(entry);
            }
        }
        while (curr->commit_name != name && curr != nullptr)
        {
            curr->make_fs_cc(fs::current_path());
            inclusion_set.insert(curr->commit_name);
            curr = curr->next;
        }
        if (curr != nullptr)
        {
            curr->make_fs_cc(fs::current_path());
            inclusion_set.insert(curr->commit_name);
            curr->next = nullptr;
        }
        auto tree_json = head->to_json();
        auto path = (fs::current_path() / fs::path(".pgit") / fs::path("tree.json")).string();
        std::ofstream file(path);
        file << tree_json.dump(4); // Write formatted JSON to file
        file.close();
        for (const auto &entry : fs::directory_iterator(fs::current_path() / fs::path(".pgit") / fs::path("stage")))
        {
            fs::remove_all(entry);
        }
        for(const auto &entry : fs::directory_iterator(fs::current_path()/fs::path(".pgit")/fs::path("diff"))){
            if(inclusion_set.find(entry.path().filename().string())==inclusion_set.end()){
                fs::remove_all(entry);
            }
        }
        for(const auto &entry : fs::directory_iterator(fs::current_path()/fs::path(".pgit")/fs::path("file"))){
            if(inclusion_set.find(removeExtension(entry.path().filename().string()))==inclusion_set.end()){
                fs::remove_all(entry);
            }
        }
    }
};

Treeobject loadTreeobject(std::string commit)
{
    json tree_json;
    std::ifstream file((fs::current_path() / fs::path(".pgit") / fs::path("tree.json")).string());

    file.seekg(0, std::ios::beg);
    Treeobject tree;

    file >> tree_json;

    // Close the file
    file.close();
    tree = Treeobject::from_json(tree_json);
    return tree;
}
