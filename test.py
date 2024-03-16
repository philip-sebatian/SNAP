import pickle

with open('histgraph/graph.pkl', 'rb') as f:
    obj = pickle.load(f)
curr=obj.start
while(curr!=None):
    print(curr.hash)
    curr=curr.front

print("***************")
obj.roll_back("acffb0d041a7ba3a21627b7ccc1809c1c7a2b6d2bcc8d38b4ea275e8c56959bc1960bb7f7accfc584b1bb8bfaa7b9e355595c25ae572c355d2162f7b2fec2b7b")
curr=obj.start
while(curr!=None):
    print(curr.hash)
    curr=curr.front
