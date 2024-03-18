#include <iostream>
#include <unordered_set>
#include <string>
#include <iterator>
#include <fstream>
#include <chrono>
#include <list>
#include <algorithm>
#include <vector>
#include "flat_hash_map.hpp"

using namespace std;


struct i_pair{
    int first;
    int second;
};

struct arg_iil{
    int first;
    int second;
    string third;
};

struct link{
    string str;
    void* atom;
    int arg;
};

void print_list(list<i_pair> ls, string name){
    for(i_pair i : ls){
        cout << " " << name << "(" << i.first << "," << i.second << ").";
    }
}

void print_distance(list<tuple<int,int,int,int,int,int>> in){
    for(auto i: in){
        cout << " distance(" << get<0>(i) << "," << get<1>(i) << "," 
             << get<2>(i) << "," << get<3>(i) << ")."; 
    }
}

void print_between(list<tuple<int,int,int,int>> in){
    for(auto i: in){
        cout << " between(" << get<0>(i) << "," << get<1>(i) << "," 
             << get<2>(i) << ")."; 
    }
}


int main(int argc, char *argv[]){

    /* 
    * 
    * 入力形式
    *   <start_1 の数>
    *   <distance_4 の数>
    *   <hl id の数>
    *   <start_1> ...
    *   <distance_4> ...
    */
    string filename = "input.txt";
    if(argc >= 2){
        filename = argv[1];
    }

    // 出力用ファイル 
    string filename2 = "result_delfirst_front.csv";


    ifstream i_file(filename);
    if(!i_file.is_open()){
        cerr << "Could not open input file - " << filename << endl;
        return 1;
    }
    // 入力補助情報. test case では between, max, min は入力 0個 とする
    int n_start, n_distance, n_between, n_max, n_min;
    // 解析情報．今回はガードに比較演算がないので HL の id の数だけ
    int n_hl;
    i_file >> n_start >> n_distance; 
    i_file >> n_hl;

    typedef int used; 


    // vector<list<tuple<int,int,int,int,used,used>>::iterator>* distance_ihhi1 = new vector<list<tuple<int,int,int,int,used,used>>::iterator>[n_hl];
    list<list<tuple<int,int,int,int,used,used>>::iterator> distance_ihhi1[n_hl];
    // vector<list<tuple<int,int,int,int,used,used>>::iterator>* distance_ihhi2 = new vector<list<tuple<int,int,int,int,used,used>>::iterator>[n_hl];
    list<list<tuple<int,int,int,int,used,used>>::iterator> distance_ihhi2[n_hl];
    list<tuple<int,int,int,int>> push_distance_ihhi;
    list<tuple<int,int,int,int,used,used>> al_distance_ihhi;
    // vector<list<tuple<int,int,int,used>>::iterator>* between_ihi = new  vector<list<tuple<int,int,int,used>>::iterator>[n_hl];
    list<list<tuple<int,int,int,used>>::iterator> between_ihi[n_hl];
    list<tuple<int,int,int>> push_between_ihi;
    list<list<tuple<int,int,int>>::iterator> between_p[n_hl];
    list<tuple<int,int,int,used>> al_between_ihi;
    // list<tuple<int,int,string>> push_min_iil;
    // list<tuple<int,int,string>> push_max_iil;
    list<int> push_start_h;
    // list<string> other;
    bool flag = true;

    int link1, link2, link3, link4;
    string tmp1,tmp2;
    for(size_t i = 0; i < n_start; i++){
        i_file >> link1;
        push_start_h.push_back(link1);
    }
    for (size_t i = 0; i < n_distance; i++)
    {
        i_file >> link1 >> link2 >> link3 >> link4;
        push_distance_ihhi.push_back({link1,link2,link3,link4});
    }

    //start calculate transitive closure
    auto start = std::chrono::steady_clock::now();
    i_pair path_tmp;

    using hist_type = std::tuple<int,int,int,int,int,int>;
    const auto hash = [](const hist_type &key){
        return (get<0>(key)*4027+get<1>(key)*4049+get<2>(key)*4057
                +get<3>(key)*4073+get<4>(key)*4073+get<5>(key)*4079);
    };

    // ska::flat_hash_set<hist_type, decltype(hash)> history_rule3(1024,hash);
    // ska::flat_hash_set<hist_type, decltype(hash)> history_rule4(1024,hash); 
    std::unordered_set<hist_type, decltype(hash)> history_rule3(1024,hash);
    std::unordered_set<hist_type, decltype(hash)> history_rule4(1024,hash); 
    
    
    // relates' of start atom 
    while(!push_start_h.empty()){
        // start(!X) :- between(0,!X,0).
        int tmp = push_start_h.back();
        push_start_h.pop_back();
        push_between_ihi.push_back({0,tmp,0});
        auto titr = prev(push_between_ihi.end());
        between_p[tmp].push_back(titr);
    }


    // distance は ルールによって増えないので下のループに入れない
    while(!push_distance_ihhi.empty()){
        // tuple<int,int,int,int> tmp = push_distance_ihhi.back();
        // push_distance_ihhi.pop_back();
        tuple<int,int,int,int> tmp = push_distance_ihhi.front();
        push_distance_ihhi.pop_front();
        int arg1 = get<0>(tmp);
        int arg2 = get<1>(tmp);
        int arg3 = get<2>(tmp);
        int arg4 = get<3>(tmp);

        // between(A,!Y,B), distance(C,!Y,!Z,D) \ 
        //     :- AC=A+C, BD=B+D, uniq(A,!Y,B,C,!Z,D) 
        //     | between(AC,!Z,BD).
        // between(A,!Y,B), distance(C,!Y,!Z,D) \ 
        //     :- AC=A-D, BD=B-C, uniq(A,!Y,B,C,!Z,D) 
        //     | between(AC,!Z,BD). 
        
        for(auto i: between_ihi[arg2]){
            if(history_rule3.insert({get<0>(*i),arg2,get<2>(*i),arg1,arg3,arg4}).second){
                int AC = get<0>(*i) + arg1;
                int BD = get<2>(*i) + arg4; 
                push_between_ihi.push_back({AC,arg3,BD});
                auto titr = prev(push_between_ihi.end());
                cout << "arg3: " << arg3 << endl; 
                between_p[arg3].push_back(titr);
            }   
            if(history_rule4.insert({get<0>(*i),arg2,get<2>(*i),arg1,arg3,arg4}).second){
                int AC = get<0>(*i) - arg4;
                int BD = get<2>(*i) - arg1; 
                push_between_ihi.push_back({AC,arg3,BD});
                auto titr = prev(push_between_ihi.end());
                between_p[arg3].push_back(titr);
            }
        }
        al_distance_ihhi.push_back({arg1,arg2,arg3,arg4,distance_ihhi1[arg2].size(),distance_ihhi2[arg3].size()});
        auto itr = al_distance_ihhi.end();
        --itr;
        distance_ihhi1[arg2].push_back({itr});
        distance_ihhi2[arg3].push_back({itr});
    }

    // int try1 = 0, app1 = 0, try2 = 0, app2 = 0;
    
    // relates' of between atom
    while (!(push_between_ihi.empty())){
        // if(try2 > 20){
        //     break;
        // }
        tuple<int,int,int> tmp = push_between_ihi.front();
        int arg1 = get<0>(tmp);
        int arg2 = get<1>(tmp);
        int arg3 = get<2>(tmp);
        push_between_ihi.pop_front();
        between_p[arg2].pop_front();


        // cout << "size: " << push_between_ihi.size() << endl;
        // cout << "  target: " << arg1 << ", " << arg2 << ", " << arg3 << endl;

        // cout << "list: ";
        // print_between(al_between_ihi);
        // cout << endl;

        // between(A,!X,B) :- A > B | .
        if(arg1 > arg3)continue;

        // between(A,!Y,B), between(C,!Y,D) :- between(max(A,C),!Y,min(B,D)).
        // max, min が消えるか消えないかを予め解析
        // 消えない場合は, between(int,int,int,int) になりえない
        if(between_p[arg2].size() > 0){
            auto itr = between_p[arg2].begin();
            int max = get<0>(**itr)>arg1?get<0>(**itr):arg1;
            int min = get<2>(**itr)<arg3?get<2>(**itr):arg3;
            push_between_ihi.erase(*itr);
            between_p[arg2].erase(itr);
            if(!(max==arg1 && min==arg3)){
                push_between_ihi.push_back({max,arg2,min});
                auto nitr = prev(push_between_ihi.end());
                between_p[arg2].push_back(nitr);
                continue;
            }
        }
        if(!between_ihi[arg2].empty()){           
            int max,min;
            // 必ず消えるので pop back
            auto tmp2 = between_ihi[arg2].back();
            // between_ihi[arg2].pop_back();
            int arg21 = get<0>(*tmp2);
            int arg22 = get<1>(*tmp2);
            int arg23 = get<2>(*tmp2);
            int arg24 = get<3>(*tmp2);

            // max@@ R = max(A,B) :- A > B | R = A.
            // max@@ R = max(A,B) :- A =< B | R = B.
            if(arg1 > arg21){
                max = arg1;
            }else if(arg1 <= arg21){
                max = arg21;
            }
            // min@@ R = min(A,B) :- A > B | R = B.
            // min@@ R = min(A,B) :- A =< B | R = A. 
            if(arg3 > arg23){
                min = arg23;
            }else if(arg3 <= arg23){
                min = arg3;
            }

            // specification
            if(arg21 != max || arg23 != min){
            push_between_ihi.push_back({max,arg2,min});
            auto titr = prev(push_between_ihi.end());
            between_p[arg2].push_back(titr);
            between_ihi[arg2].pop_back();
            al_between_ihi.erase(tmp2);
            }

            continue;
        }

        // between(A,!Y,B), distance(C,!Y,!Z,D) \ 
        //     :- AC=A+C, BD=B+D, uniq(A,!Y,B,C,!Z,D) 
        //     | between(AC,!Z,BD).
        // between(A,!Y,B), distance(C,!Y,!Z,D) \ 
        //     :- AC=A-D, BD=B-C, uniq(A,!Y,B,C,!Z,D) 
        //     | between(AC,!Z,BD).
        for(auto i: distance_ihhi1[arg2]){
            // try1++;
            if(history_rule3.insert({arg1,arg2,arg3,get<0>(*i),get<2>(*i),get<3>(*i)}).second){
                // app1++;
                int AC = arg1 + get<0>(*i);
                int BD = arg3 + get<3>(*i);
                // cout << "forward" << AC << ", " << get<2>(*i) << ", " <<BD <<  endl; 
                push_between_ihi.push_back({AC,get<2>(*i),BD});
                auto titr = prev(push_between_ihi.end());
                between_p[get<2>(*i)].push_back(titr);
            }  
        }
        for(auto i: distance_ihhi2[arg2]){
            // try2++;
            if(history_rule4.insert({arg1,arg2,arg3,get<0>(*i),get<1>(*i),get<3>(*i)}).second){
                // app2++;
                int AC = arg1 - get<3>(*i);
                int BD = arg3 - get<0>(*i); 
                // cout << "\tpair: " << get<3>(*i) << "," << get<1>(*i) << "," << get<2>(*i) << "," << get<3>(*i) << endl;
                // cout << "\tbackward args: " << AC << ", " << BD << endl;                 
                push_between_ihi.push_back({AC,get<1>(*i),BD});
                auto titr = prev(push_between_ihi.end());
                between_p[get<1>(*i)].push_back(titr);
            } 
        }
        al_between_ihi.push_back({arg1,arg2,arg3,between_ihi[arg2].size()});
        auto itr = al_between_ihi.end();
        --itr;
        between_ihi[arg2].push_back({itr});
    }

    auto end = std::chrono::steady_clock::now();
    long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    ofstream o_file;
    o_file.open(filename2, ios::app);

    o_file << duration << ",";

    print_distance(al_distance_ihhi);
    print_between(al_between_ihi);
    // cout << "@4." << endl;

    // cout << sizeof(tuple<int,int,int,int,int,int>) << " : "<< sizeof(int) << endl;
    // cout << history_rule3.size() << ", " << history_rule4.size() << endl;
    // cout << history_rule3.load_factor() << "," << history_rule3.max_load_factor() << endl;

    // cout << al_between_ihi.size() << ", " << al_distance_ihhi.size() << endl;

    // cout << try1 << "," << app1 << "," << try2 << "," << app2 << endl;


    return 0;
}