#include "hash_original.hpp"

int main(){
    //test

    int arr[3];
    int count = 100;

    ihash::hash_table set = ihash::hash_table(SIZE,3);

    for (int i = 0; i < count; i++)
    {
        arr[0] = i;
        for (int j = 0; j < count; j++)
        {
            arr[1] = j;
            for (int k = 0; k < count; k++)
            {
              arr[2] = k;
                if(set.insert(arr)){
                    std::cout <<  "inserted: " << i << ", " << j << ", " << k << std::endl; 
                }  
                else{
                    std::cout << "cannot inserted: " << i << ", " << j << ", " << k << std::endl;
                }
                if(set.insert(arr)){
                    std::cout <<  "inserted: " << i << ", " << j << ", " << k << std::endl; 
                }  
                else{
                    std::cout << "cannot inserted: " << i << ", " << j << ", " << k << std::endl;
                }
            }
        }
        
    }
    
}