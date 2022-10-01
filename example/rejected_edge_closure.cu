#include <cstdio>
#include <vector>
#include <string>
#include <queue>
#include <sstream>
#include <chrono>
#include "cuda_runtime.h"

using namespace std;

#define Dim 512

/** 
 * - step 1 : 出現する可能性のあるアトムの構造体を定義する？
 * -- 一旦非連結用に作る（引数は全てデータアトム）
 * -- 連結にするにしてもポインタを引数分用意すればいいと思う
 * -- atomlist添え字アクセスや連続したメモリ領域の確保のためにベクタを使う
 * -- cchr と同様に freelist も用意する
 */ 
struct edge_2{
    int arg[2];
    bool live = true;
}

struct atomlist{
    vector<edge_2> atom;
    int prev;
    int curr;
    queue<edge_2> freelist;

    void move_diffatomlist(){
        prev = curr;
        curr = atom.size();
    }

    void del_all(){
        while(!(atom.back().live)){
            atom.pop_back();
        }
        while (!(freelist.empty()))
        {
            int adr = freelist.front();
            freelist.pop();
            if(adr < atom.size()){
                atom[adr] = atom.back();
                atom.pop_back();
            }
        } 
    }
}

void input(atomlist *al){
    string l,a_name, arg;
    edge_2 atom;
    while(getline(std::cin, l)){
        istringstream stream(l);
        while(getline(stream, a_name, '(')){
            getline(stream, arg, ',');
            atom.arg[0] = atoi(arg);
            getline(stream, arg, ')');
            atom.arg[1] = atoi(arg);
            al->atom.push_back()
        }
    }
}

void output(atomlist al){
    for (int i = 0; i < al.atom.size(); i++)
    {
        cout << "edge(" << al.atom[i].arg[0] << "," << 
                           al.atom[i].arg[1] << "), ";
    }
    cout << endl;
}

__global__ 
void parallel_matching(){
    
}