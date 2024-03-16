import pickle

with open('histgraph/graph.pkl', 'rb') as f:
    obj = pickle.load(f)
curr=obj.start
while(curr!=None):
    print(curr.hash)
    curr=curr.front

print("***************")
obj.roll_back("4760358ac696cdf19b27a7b6c47f62e81dab9c0364da643517df4893b7f6383e")
curr=obj.start
while(curr!=None):
    print(curr.hash)
    curr=curr.front
