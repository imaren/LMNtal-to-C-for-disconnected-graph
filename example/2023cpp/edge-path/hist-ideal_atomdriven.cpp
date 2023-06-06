// 閉包計算
// atom 主導と配列を組み合わせる

// edge-path のやつ
// edge(A,B), path(C,D) \:- B=:=C, ... | path(A,D).

#include <iostream>
#include <unordered_set>
#include <string>
#include <iterator>
#include <fstream>
#include <chrono>
#include <list>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// #define S 4
using namespace std;


struct i_pair{
    int first;
    int second;
};

void print_list(list<i_pair> ls, string name){
    for(i_pair i : ls){
        cout << name << "(" << i.first << "," << i.second << "). ";
    }
}

int main(int argc, char *argv[]){

    /* 
    * 入力めんどくさいからとりあえず配列の形で渡すことにする
    * めんどくさいから頂点番号の最大値もわかってることにする
    * 
    * 入力形式
    *   <頂点番号の最大値>
    *   <edge_0> <edge_1>
    *   ...
    *   <edge_0> <edge_1>
    */
    string filename = "input.txt";
    if(argc >= 2){
        filename = argv[1];
    }

    // 出力用ファイル 
    string filename2 = "result_hist-ideal.csv";


    ifstream i_file(filename);
    if(!i_file.is_open()){
        cerr << "Could not open input file - " << filename << endl;
        return 1;
    }
    unsigned int S;

    i_file >> S; 

    list<i_pair>* edge_list = new list<i_pair>[S]; 
    list<i_pair> push_atom_path;
    list<i_pair> path_list;

    int link1, link2;
    // edge list の中で1つ以上要素が登録されているリストの配列番号
    list<int> used_edge_list;
    while (i_file >> link1)
    {
        i_file >> link2;
        i_pair in;
        in.first = link1;
        in.second = link2;
        edge_list[link2].push_back(in);
        used_edge_list.push_back(link2);
        push_atom_path.push_back(in);
    }

    //必要な byte 数の計算
    uint8_t b_needed=sizeof(unsigned int);
    unsigned int tmp = S;
    unsigned int dummy = 0xFF << sizeof(unsigned int)*8-8;

    while (!(tmp & dummy))
    {
        dummy = dummy >> 8;
        b_needed--;
    }
    
    
    

    //start calculate transitive closure
    auto start = std::chrono::steady_clock::now();
    i_pair path_tmp;

    uint8_t *history = (uint8_t *)calloc(100000000000, sizeof(uint8_t));

    // cout << "calloc " << (int)b_needed << endl;

    if(history == NULL){
        cout << "calloc error" << endl;
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        if(push_atom_path.empty()) break;

        path_tmp = push_atom_path.front();
        push_atom_path.pop_front();
        path_list.push_back(path_tmp);
        
        for(i_pair i : edge_list[path_tmp.first]){
            // unsigned long long nhist = ((i.first << b_needed*16) + (i.second << b_needed*8) + (path_tmp.second));
            // 履歴の追加に成功（＝マッチングに成功）したら
            if(!history[((i.first*S*S) + (i.second*S) + (path_tmp.second))]){
                history[((i.first*S*S) + (i.second*S) + (path_tmp.second))]=1;
                i_pair ins = {i.first, path_tmp.second};
                push_atom_path.push_back(ins);
            }
        }
    }
    // std::cout << "a" << endl;

    auto end = std::chrono::steady_clock::now();
    long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    ofstream o_file;
    o_file.open(filename2, ios::app);

    o_file << duration << endl;

    free(history);

    // for(int i : used_edge_list){
    //     print_list(edge_list[i], "edge");
    // }
    // print_list(path_list, "path");
    // cout << "@4." << endl;

    return 0;
}