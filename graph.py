import os
import copy
import hashlib
#structure of each nodes
class Graph_node:
    def __init__(self,hash):
        self.hash=hash
        self.front=None
        self.back=None
        
        
#this the a DAG that stores the logs the commits
class HistoryGraph:
    def __init__(self,name):
        self.name=name
        self.start=None
        self.branches={}#contains heads of each branch
        self.current=None
        self.curr_branch="master"
    
    #adds the log to the current branch
    def add_log(self,node):
        if "master" not in self.branches:
            
            self.start=node
            self.current=node
            self.branches["master"]=self.start
        self.branches[self.curr_branch].front=node
        node.back=self.branches[self.curr_branch]
        self.branches[self.curr_branch]=node
    
    
    
    #creates new branches in the graph ie adds the pointer of head of each branch to  self.braches 
    def branch(self,branch_name):
        if branch_name in self.branches:
            print(f"branch {branch_name} already exists")
            return
        
        self.branches[branch_name]=copy.deepcopy(self.current)
        print(f"branch {branch_name} was created")
    
    
    #switches the branches
    def switch_brach(self,branch_name):
        if branch_name not in self.branches:
            print(f"branch {branch_name} does not exits")
        else:
            self.current=self.branches[branch_name]
            self.curr_branch=branch_name
            print(f"switched to branch {branch_name}")
    


#strucrure of each of the foldernodes
#these are responsilbe for keeping and calculating the hashes recursively
class Foldernode:
    def __init__(self,path):
        self.folder_path=path
        self.files,self.folders = None,None
        self.hash=''
        self.get_file_folder()
        self.get_hash_folder()
    
    #gets hash of each folder inside the folder node including itself
    def get_hash_folder(self):
        hash_f=hashlib.new("sha256")
        
        concar_s=''
        for i in self.files:
            f_p=os.path.join(self.folder_path,i)
            encryption=self.calculate_file_sha(f_p)
            concar_s+=encryption
        for ffs in self.folders:
            concar_s+=ffs.hash
        hash_f.update(concar_s.encode('utf-8'))
        self.hash=hash_f.hexdigest()
        return self.hash
    
    
    #gets file and folder inside the folder of folder node 
    def get_file_folder(self):
        file_list=[]
        folder_list=[]
        
        for i in os.listdir(self.folder_path):
            if not os.path.isdir(os.path.join(self.folder_path,i)) and i[0]!=".":
                file_list.append(i)
            elif i[0]!='.' and i!="__pycache__":
                folder_list.append(Foldernode(os.path.join(self.folder_path,i)))
        self.files=file_list
        self.folders=folder_list
        return file_list,folder_list
        


    #calculates hash of a file
    def calculate_file_sha(self,file_path, algorithm='sha256'):
        hash_function = hashlib.new(algorithm)
        with open(file_path, 'rb') as f:
            while chunk := f.read(4096):
                hash_function.update(chunk)
        return hash_function.hexdigest()