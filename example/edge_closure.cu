#include <cstdio>
#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include <sstream>
#include <chrono>
#include "cuda_runtime.h"

using namespace std;

#define Dim 512

struct edge_2{
    vector<int> arg1 = {};
    vector<int> arg2 = {};
    int prev;
    int curr;

    void init_diffatomlist(){
        prev = curr = arg1.size();
    }

    void move_diffatomlist(){
        prev = curr;
        curr = arg1.size();
    }
};

void input(edge_2 *al){
    string l,a_name, arg;
    cout << "called input" << endl;
    while(getline(std::cin, l)){
        istringstream stream(l);
        while(getline(stream, a_name, '(')){
            getline(stream, arg, ',');
            int tmp = atoi(arg.c_str());
            al->arg1.push_back(tmp);
            getline(stream, arg, ')');
            tmp = atoi(arg.c_str());
            al->arg2.push_back(atoi(arg.c_str()));
        }
    }
}

void output(edge_2 *al){
    int i;
    for (i = 0; i < al->arg1.size()-2; i++)
    {
        cout << "edge(" << al->arg1[i] << "," << 
                           al->arg2[i] << "), ";
    }
    cout << "edge(" << al->arg1[i] << "," << al->arg2[i] << ")." << endl;
}



__global__ 
void parallel_matching(){
    
}

int main(void){
    struct edge_2 atomlist;
    input(&atomlist);
    atomlist.init_diffatomlist();

    output(&atomlist);
    return 0;
}