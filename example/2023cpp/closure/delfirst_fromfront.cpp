#include <iostream>
#include <unordered_set>
#include <string>
#include <iterator>
#include <fstream>
#include <chrono>
#include <list>
#include <vector>

using namespace std;

void print_path(list<tuple<int,int>> in){
    for(auto i: in){
        cout << " path(" << get<0>(i) << "," << get<1>(i) << ")."; 
    }
}

int main(int argc, char *argv[]){

    string filename = "input.txt";
    if(argc >= 2){
        filename = argv[1];
    }

    string filename2 = "result_cpp.csv";


    ifstream i_file(filename);
    if(!i_file.is_open()){
        cerr << "Could not open input file - " << filename << endl;
        return 1;
    }
    // 入力補助情報. test case では between, max, min は入力 0個 とする
    int n_v, n_e;
    // 解析情報．今回はガードに比較演算がないので HL の id の数だけ
   
    i_file >> n_v >> n_e; 
    int n_hl = n_v;
    typedef int used; 

    list<list<tuple<int,int>>::iterator> IS_path_hh_al_1[n_hl];
    list<list<tuple<int,int>>::iterator> IS_path_hh_al_2[n_hl];
    list<tuple<int,int>> D_path_hh_p;
    list<tuple<int,int>>  D_path_hh_al;
    bool flag = true;

    int link1, link2;
    string tmp1,tmp2;
    for (size_t i = 0; i < n_e; i++)
    {
        i_file >> link1 >> link2;
        D_path_hh_p.push_back({link1,link2});
    }

    //start calculate transitive closure
    auto start = std::chrono::steady_clock::now();

    using hist_type = std::tuple<int,int>;
    const auto hash = [](const hist_type &key){
        return (get<0>(key)*4027+get<1>(key)*4049);
    };

    std::unordered_set<hist_type, decltype(hash)> history_rule1(1024,hash);

    while (!(D_path_hh_p.empty())){
        tuple<int,int> tmp = D_path_hh_p.front();
        int arg1 = get<0>(tmp);
        int arg2 = get<1>(tmp);
        D_path_hh_p.pop_front();

        for(auto i: IS_path_hh_al_1[arg2]){
            if(history_rule1.insert({arg1,get<1>(*i)}).second){
                D_path_hh_p.push_back({arg1,get<1>(*i)});
            }  
        }
        for(auto i: IS_path_hh_al_2[arg1]){
            if(history_rule1.insert({get<0>(*i),arg2}).second){
                 D_path_hh_p.push_back({get<0>(*i),arg2});
            }
        }
        D_path_hh_al.push_back({arg1,arg2});
        auto itr = prev(D_path_hh_al.end());
        IS_path_hh_al_1[arg1].push_back({itr});
        IS_path_hh_al_2[arg2].push_back({itr});
    }

    auto end = std::chrono::steady_clock::now();
    long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    ofstream o_file;
    o_file.open(filename2, ios::app);

    o_file << duration << ",";

    // print_path(D_path_hh_al);
    // cout << " @4." << endl;
    return 0;
}