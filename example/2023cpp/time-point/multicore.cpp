#include <iostream>
#include <unordered_set>
#include <string>
#include <iterator>
#include <fstream>
#include <chrono>
#include <list>
#include <algorithm>
#include <vector>
#include <mutex>
#include <thread>
#include "flat_hash_map.hpp"

// https://contentsviewer.work/Master/software/cpp/how-to-implement-a-thread-pool/article
// https://github.com/nodec-project/nodec
#include "thread_pool_executor.hpp"

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

typedef int used; 
using hist_type = std::tuple<int,int,int,int,int,int>;
const auto myhash = [](const hist_type &key){
    return (get<0>(key)*4027+get<1>(key)*4049+get<2>(key)*4057
            +get<3>(key)*4073+get<4>(key)*4073+get<5>(key)*4079);
};

class LMNTAL{
    vector<list<tuple<int,int,int,int,used,used>>::iterator>* distance_ihhi1;
    vector<list<tuple<int,int,int,int,used,used>>::iterator>* distance_ihhi2;
    list<tuple<int,int,int,int>> push_distance_ihhi;
    list<tuple<int,int,int,int,used,used>> al_distance_ihhi;
    vector<list<tuple<int,int,int,used>>::iterator>* between_ihi;
    list<tuple<int,int,int>> push_between_ihi;
    list<tuple<int,int,int,used>> al_between_ihi;
    list<tuple<int,int,string>> push_min_iil;
    list<tuple<int,int,string>> push_max_iil;
    list<int> push_start_h;
    list<string> other;
    bool flag = true;

    mutex mtx;

    ska::flat_hash_set<hist_type, decltype(myhash)>* history_rule3;
    ska::flat_hash_set<hist_type, decltype(myhash)>* history_rule4; 

public:
    LMNTAL(int n_hl){
        distance_ihhi1 = new vector<list<tuple<int,int,int,int,used,used>>::iterator>[n_hl];
        distance_ihhi2 = new vector<list<tuple<int,int,int,int,used,used>>::iterator>[n_hl];
        between_ihi = new  vector<list<tuple<int,int,int,used>>::iterator>[n_hl];
        history_rule3 = new ska::flat_hash_set<hist_type, decltype(myhash)>(1024,myhash);
        history_rule4 = new ska::flat_hash_set<hist_type, decltype(myhash)>(1024,myhash);
    }
    ~LMNTAL(){
        // delete(distance_ihhi1);
        // delete(distance_ihhi2);
        // delete(between_ihi);
        delete(history_rule3);
        delete(history_rule4);
    }

    void init_pushstart_h(list<int> l){
        this->push_start_h = l;
    }

    void init_push_distance_ihhi(list<tuple<int,int,int,int>> l){
        this->push_distance_ihhi = l;
    }

    void loop_start(){
        while(!push_start_h.empty()){
        // start(!X) :- between(0,!X,0).
        int tmp = push_start_h.back();
        push_start_h.pop_back();
        push_between_ihi.push_back({0,tmp,0});
        }
        return;
    }

    void loop_distance(){
        while(!push_distance_ihhi.empty()){
            tuple<int,int,int,int> tmp = push_distance_ihhi.back();
            push_distance_ihhi.pop_back();
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
                cout << get<0>(*i) << endl;
                if(history_rule3->insert({get<0>(*i),arg2,get<2>(*i),arg1,arg3,arg4}).second){
                    int AC = get<0>(*i) + arg1;
                    int BD = get<2>(*i) + arg4; 
                    cout << "forward" << AC << ", " << BD << endl;
                    push_between_ihi.push_back({AC,arg3,BD});
                }   
                if(history_rule4->insert({get<0>(*i),arg2,get<2>(*i),arg1,arg3,arg4}).second){
                    int AC = get<0>(*i) - arg4;
                    int BD = get<2>(*i) - arg1; 
                    cout << "backward" << AC << ", " << BD << endl;
                    push_between_ihi.push_back({AC,arg3,BD});
                }
            }
            al_distance_ihhi.push_back({arg1,arg2,arg3,arg4,distance_ihhi1[arg2].size(),distance_ihhi2[arg3].size()});
            auto itr = al_distance_ihhi.end();
            --itr;
            distance_ihhi1[arg2].push_back({itr});
            distance_ihhi2[arg3].push_back({itr});
        }
    }

    static void thr_distance(tuple<int,int,int,int> target, void* param){
        LMNTAL* const that = (LMNTAL* const)param;

        int arg1 = get<0>(target);
        int arg2 = get<1>(target);
        int arg3 = get<2>(target);
        int arg4 = get<3>(target);

        for(auto i: that->between_ihi[arg2]){
            cout << get<0>(*i) << endl;
            if(that->history_rule3->find({get<0>(*i),arg2,get<2>(*i),arg1,arg3,arg4}) != that->history_rule3->end()){
                int AC = get<0>(*i) + arg1;
                int BD = get<2>(*i) + arg4; 
                that->mtx.lock();
                that->history_rule3->insert({get<0>(*i),arg2,get<2>(*i),arg1,arg3,arg4});
                that->push_between_ihi.push_back({AC,arg3,BD});
                that->mtx.unlock();
            }   
            if(that->history_rule4->find({get<0>(*i),arg2,get<2>(*i),arg1,arg3,arg4}) != that->history_rule4->end() ){
                int AC = get<0>(*i) - arg4;
                int BD = get<2>(*i) - arg1; 
                that->mtx.lock();
                that->history_rule4->insert({get<0>(*i),arg2,get<2>(*i),arg1,arg3,arg4});
                that->push_between_ihi.push_back({AC,arg3,BD});
                that->mtx.unlock();
            }
        }
        that->mtx.lock();
        that->al_distance_ihhi.push_back({arg1,arg2,arg3,arg4,that->distance_ihhi1[arg2].size(),that->distance_ihhi2[arg3].size()});
        auto itr = that->al_distance_ihhi.end();
        --itr;
        that->distance_ihhi1[arg2].push_back({itr});
        that->distance_ihhi2[arg3].push_back({itr});
        that->mtx.unlock();
    }

    void multicore_between(){
        nodec::concurrent::ThreadPoolExecutor exector(4);

        auto future_test = exector.submit([&](auto number){ return this->thread_between(); }, 1);

        while (!(push_between_ihi.empty())){
            tuple<int,int,int> tmp = push_between_ihi.back();
            int arg1 = get<0>(tmp);
            int arg2 = get<1>(tmp);
            int arg3 = get<2>(tmp);
            push_between_ihi.pop_back();



            
        }
    }

    void thread_between(){
        // cout << "target: " << arg1 << ", " << arg2 << ", " << arg3 << endl;

            // cout << "list: ";
            // print_between(al_between_ihi);
            // cout << endl;

            // between(A,!X,B) :- A > B | .
            if(arg1 > arg3)continue;

            // between(A,!Y,B), between(C,!Y,D) :- between(max(A,C),!Y,min(B,D)).
            // max, min が消えるか消えないかを予め解析
            // 消えない場合は, between(int,int,int,int) になりえない
            if(!between_ihi[arg2].empty()){
                // 必ず消えるので pop back
                auto tmp2 = between_ihi[arg2].back();
                between_ihi[arg2].pop_back();
                int arg21 = get<0>(*tmp2);
                int arg22 = get<1>(*tmp2);
                int arg23 = get<2>(*tmp2);
                int arg24 = get<3>(*tmp2);

                // max@@ R = max(A,B) :- A > B | R = A.
                // max@@ R = max(A,B) :- A =< B | R = B.
                int max;
                if(arg1 > arg21){
                    max = arg1;
                }else if(arg1 <= arg21){
                    max = arg21;
                }
                // min@@ R = min(A,B) :- A > B | R = B.
                // min@@ R = min(A,B) :- A =< B | R = A. 
                int min;
                if(arg3 > arg23){
                    min = arg23;
                }else if(arg3 <= arg23){
                    min = arg3;
                }
                push_between_ihi.push_back({max,arg2,min});

                al_between_ihi.erase(tmp2);
                
                continue;
            }


            // between(A,!Y,B), distance(C,!Y,!Z,D) \ 
            //     :- AC=A+C, BD=B+D, uniq(A,!Y,B,C,!Z,D) 
            //     | between(AC,!Z,BD).
            // between(A,!Y,B), distance(C,!Y,!Z,D) \ 
            //     :- AC=A-D, BD=B-C, uniq(A,!Y,B,C,!Z,D) 
            //     | between(AC,!Z,BD).
            for(auto i: distance_ihhi1[arg2]){
                if(history_rule3->insert({arg1,arg2,arg3,get<0>(*i),get<2>(*i),get<3>(*i)}).second){
                    int AC = arg1 + get<0>(*i);
                    int BD = arg3 + get<3>(*i);
                    // cout << "forward" << AC << ", " << get<2>(*i) << ", " <<BD <<  endl; 
                    push_between_ihi.push_back({AC,get<2>(*i),BD});
                }      
            }
            for(auto i: distance_ihhi2[arg2]){
                if(history_rule4->insert({arg1,arg2,arg3,get<0>(*i),get<1>(*i),get<3>(*i)}).second){
                    int AC = arg1 - get<3>(*i);
                    int BD = arg3 - get<0>(*i); 
                    // cout << "backward" << AC << ", " << BD << endl; 
                    push_between_ihi.push_back({AC,get<1>(*i),BD});
                } 
            }
            al_between_ihi.push_back({arg1,arg2,arg3,between_ihi[arg2].size()});
            auto itr = al_between_ihi.end();
            --itr;
            between_ihi[arg2].push_back({itr});
    }

    void print_list(list<i_pair> ls, string name){
        for(i_pair i : ls){
            cout << " " << name << "(" << i.first << "," << i.second << ").";
        }
    }

    void loop_between(){
        while (!(push_between_ihi.empty())){
            tuple<int,int,int> tmp = push_between_ihi.back();
            int arg1 = get<0>(tmp);
            int arg2 = get<1>(tmp);
            int arg3 = get<2>(tmp);
            push_between_ihi.pop_back();

            // cout << "target: " << arg1 << ", " << arg2 << ", " << arg3 << endl;

            // cout << "list: ";
            // print_between(al_between_ihi);
            // cout << endl;

            // between(A,!X,B) :- A > B | .
            if(arg1 > arg3)continue;

            // between(A,!Y,B), between(C,!Y,D) :- between(max(A,C),!Y,min(B,D)).
            // max, min が消えるか消えないかを予め解析
            // 消えない場合は, between(int,int,int,int) になりえない
            if(!between_ihi[arg2].empty()){
                // 必ず消えるので pop back
                auto tmp2 = between_ihi[arg2].back();
                between_ihi[arg2].pop_back();
                int arg21 = get<0>(*tmp2);
                int arg22 = get<1>(*tmp2);
                int arg23 = get<2>(*tmp2);
                int arg24 = get<3>(*tmp2);

                // max@@ R = max(A,B) :- A > B | R = A.
                // max@@ R = max(A,B) :- A =< B | R = B.
                int max;
                if(arg1 > arg21){
                    max = arg1;
                }else if(arg1 <= arg21){
                    max = arg21;
                }
                // min@@ R = min(A,B) :- A > B | R = B.
                // min@@ R = min(A,B) :- A =< B | R = A. 
                int min;
                if(arg3 > arg23){
                    min = arg23;
                }else if(arg3 <= arg23){
                    min = arg3;
                }
                push_between_ihi.push_back({max,arg2,min});

                al_between_ihi.erase(tmp2);
                
                continue;
            }


            // between(A,!Y,B), distance(C,!Y,!Z,D) \ 
            //     :- AC=A+C, BD=B+D, uniq(A,!Y,B,C,!Z,D) 
            //     | between(AC,!Z,BD).
            // between(A,!Y,B), distance(C,!Y,!Z,D) \ 
            //     :- AC=A-D, BD=B-C, uniq(A,!Y,B,C,!Z,D) 
            //     | between(AC,!Z,BD).
            for(auto i: distance_ihhi1[arg2]){
                if(history_rule3->insert({arg1,arg2,arg3,get<0>(*i),get<2>(*i),get<3>(*i)}).second){
                    int AC = arg1 + get<0>(*i);
                    int BD = arg3 + get<3>(*i);
                    // cout << "forward" << AC << ", " << get<2>(*i) << ", " <<BD <<  endl; 
                    push_between_ihi.push_back({AC,get<2>(*i),BD});
                }      
            }
            for(auto i: distance_ihhi2[arg2]){
                if(history_rule4->insert({arg1,arg2,arg3,get<0>(*i),get<1>(*i),get<3>(*i)}).second){
                    int AC = arg1 - get<3>(*i);
                    int BD = arg3 - get<0>(*i); 
                    // cout << "backward" << AC << ", " << BD << endl; 
                    push_between_ihi.push_back({AC,get<1>(*i),BD});
                } 
            }
            al_between_ihi.push_back({arg1,arg2,arg3,between_ihi[arg2].size()});
            auto itr = al_between_ihi.end();
            --itr;
            between_ihi[arg2].push_back({itr});
        }
    }

    void print_list(list<i_pair> ls, string name){
        for(i_pair i : ls){
            cout << " " << name << "(" << i.first << "," << i.second << ").";
        }
    }

    void print_distance(){
        for(auto i: this->al_distance_ihhi){
            cout << " distance(" << get<0>(i) << "," << get<1>(i) << "," 
                << get<2>(i) << "," << get<3>(i) << ")."; 
        }
    }

    void print_between(){
        for(auto i: this->al_between_ihi){
            cout << " between(" << get<0>(i) << "," << get<1>(i) << "," 
                << get<2>(i) << ")."; 
        }
    }

};

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
    string filename2 = "result_flathash_myhashfunc.csv";


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

    LMNTAL lmn(n_hl);
    
    list<tuple<int,int,int,int>> push_distance_ihhi;
    list<tuple<int,int,int>> push_between_ihi;
    list<int> push_start_h;

    int link1, link2, link3, link4;
    string tmp1,tmp2;
    for(size_t i = 0; i < n_start; i++){
        i_file >> link1;
        push_start_h.push_back(link1);
    }
    for (size_t i = 0; i < n_distance; i++){
        i_file >> link1 >> link2 >> link3 >> link4;
        push_distance_ihhi.push_back({link1,link2,link3,link4});
    }

    lmn.init_pushstart_h(push_start_h);
    lmn.init_push_distance_ihhi(push_distance_ihhi);


    //start calculate transitive closure
    auto start = std::chrono::steady_clock::now();
    i_pair path_tmp;

    lmn.loop_start();
    lmn.loop_distance();
    lmn.loop_between();

    auto end = std::chrono::steady_clock::now();
    long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    ofstream o_file;
    o_file.open(filename2, ios::app);

    o_file << duration << ",";

    lmn.print_distance();
    lmn.print_between();
    cout << "@4." << endl;

    return 0;
}