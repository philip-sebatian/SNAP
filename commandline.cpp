#include "tree.cpp"

int main(int argc, char *argv[])
{
    std::cout<<argv[1]<<std::endl;
    if(std::strcmp(argv[1], "snapshot") == 0 ){
        if(argc ==3){
            auto j = loadTreeobject();
            j.snapshot(argv[2]);
        }
        else{
        std::cout<<"invalid number of arguments for command"<<std::endl;
    }

    }
    
    
}