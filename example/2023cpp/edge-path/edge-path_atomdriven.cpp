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
    string filename2 = "result_stl.csv";


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

    unordered_set<string> history; 

    std::cout << history.bucket_count() << " : ";
    //start calculate transitive closure
    auto start = std::chrono::steady_clock::now();
    i_pair path_tmp;

    while (true)
    {
        if(push_atom_path.empty()) break;

        path_tmp = push_atom_path.front();
        push_atom_path.pop_front();
        path_list.push_back(path_tmp);
        
        for(i_pair i : edge_list[path_tmp.first]){
            string nhist = to_string(i.first) + ":" + to_string(i.second) + ":" + to_string(path_tmp.second);
            // 履歴の追加に成功（＝マッチングに成功）したら
            if(history.insert(nhist).second){
                i_pair ins = {i.first, path_tmp.second};
                push_atom_path.push_back(ins);
            }
        }
    }

    auto end = std::chrono::steady_clock::now();
    long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << history.bucket_count() << endl;

    ofstream o_file;
    o_file.open(filename2, ios::app);

    o_file << duration << endl;
    // std::cout << history.max_size() << endl;

    // for(int i : used_edge_list){
    //     print_list(edge_list[i], "edge");
    // }
    // print_list(path_list, "path");
    // cout << "@4." << endl;

    return 0;
}