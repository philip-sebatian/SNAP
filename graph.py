import os

class Graph_node:
    def __init__(self,hash):
        self.hash=hash
        self.front=None
        self.back=None
class HistoryGraph:
    def __init__(self):
        self.head=None
        self.start=None
    def add_log(self,node):
        if self.head==None and self.start==None:
            self.head=node
            self.start=node
        self.head.front=node 
        node.back=self.head
        self.head=self.head.front
    def roll_back(self,hash):
        curr=self.head
        while curr.hash!=hash:
            curr=curr.back
        curr.front=None