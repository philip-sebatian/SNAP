import os 
import hashlib



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





