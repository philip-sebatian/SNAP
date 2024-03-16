import os
import copy

class Graph_node:
    def __init__(self,hash):
        self.hash=hash
        self.front=None
        self.back=None
        
        

class HistoryGraph:
    def __init__(self,name):
        self.name=name
        self.start=None
        self.branches={}#contains heads of each branch
        self.current=None
        self.curr_branch="master"
    
    
    def add_log(self,node):
        if "master" not in self.branches:
            
            self.start=node
            self.current=node
            self.branches["master"]=self.start
        self.branches[self.curr_branch].front=node
        node.back=self.branches[self.curr_branch]
        self.branches[self.curr_branch]=node
    
    
    
    
    def branch(self,branch_name):
        if branch_name in self.branches:
            print(f"branch {branch_name} already exists")
            return
        
        self.branches[branch_name]=copy.deepcopy(self.current)
        print(f"branch {branch_name} was created")
    
    def switch_brach(self,branch_name):
        if branch_name not in self.branches:
            print(f"branch {branch_name} does not exits")
        else:
            self.current=self.branches[branch_name]
            self.curr_branch=branch_name
            print(f"switched to branch {branch_name}")
    
    