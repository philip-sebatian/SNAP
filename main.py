from script import Foldernode
from graph import Graph_node,HistoryGraph
import pickle 
import time

graph=HistoryGraph()
Project=Foldernode("./")
Project_hash = Project.hash
print(Project_hash)
current_hash=Project_hash

count=0
while True:
    current=Foldernode("./")
    if current.hash!=Project.hash:
        node=Graph_node(current.hash)
        graph.add_log(node)
        print("file has changed")
        Project=current
        if count==2:
               break 
        count+=1
        print(count)
    time.sleep(0.25)

with open("histgraph/graph.pkl","wb") as f:
            pickle.dump(graph,f)



