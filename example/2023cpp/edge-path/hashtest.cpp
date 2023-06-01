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
#include "flat_hash_map.hpp"

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

int h1(int a, int b, int c){
    return a+b+c;
}

int h2(int a, int b, int c){
    return a*2+b*3+c*5;
}

int h3(int a, int b, int c){
    return a*11+b*23+c*53;
}

int h4_sub(int a){
    return a % 10000000;
}

int h4(int a, int b, int c){
    int tmp = (h4_sub(a) xor 0x123410) * 3.14159 ;
    tmp = (h4_sub(a) xor tmp) * 3.14159;
    return h4_sub((h4_sub(a) xor tmp) *3.14159);
}

int main(int argc, char *argv[]){

    string filename = "input.txt";
    if(argc >= 2){
        filename = argv[1];
    }
    string filename2 = "hashtest.csv";
    ifstream i_file(filename);
    if(!i_file.is_open()){
        cerr << "Could not open input file - " << filename << endl;
        return 1;
    }

    int S;

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
    ska::flat_hash_set<string> history; 

    i_pair path_tmp;
    ofstream o_file;
    o_file.open(filename2);
    o_file << "arg1,arg2,arg3,hash value" << endl;

    // std::cout << history.max_size() << endl;

    while (true)
    {
        if(push_atom_path.empty()) break;

        path_tmp = push_atom_path.front();
        push_atom_path.pop_front();
        path_list.push_back(path_tmp);
        
        for(i_pair i : edge_list[path_tmp.first]){
            string nhist = to_string(i.first) + ":" + to_string(i.second) + ":" + to_string(path_tmp.second);
            if(history.insert(nhist).second){
                //hash 関数のテスト. マッチングが成功したとき（=key が違う時）value がどれくらい衝突するか確かめるためこのブロック．
                o_file << i.first << "," << i.second << "," << path_tmp.second << "," 
                        << h1(i.first,i.second,path_tmp.second) << ","
                        << h2(i.first,i.second,path_tmp.second) << ","
                        << h3(i.first,i.second,path_tmp.second) << ","
                        << h4(i.first,i.second,path_tmp.second) << ","
                        << endl;

                i_pair ins = {i.first, path_tmp.second};
                push_atom_path.push_back(ins);
            }
        }
    }
    // std::cout << history.max_size() << endl;



    // for(int i : used_edge_list){
    //     print_list(edge_list[i], "edge");
    // }
    // print_list(path_list, "path");
    // cout << "@4." << endl;

    return 0;
}