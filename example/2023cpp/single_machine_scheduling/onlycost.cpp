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

void print_job(list<tuple<int,int,int,int>> in){
    for(auto i: in){
        cout << " job(" << get<0>(i) << "," << get<1>(i) << ",!H" 
             << get<2>(i) << ")."; 
    }
}

void print_ilist(list<int> lst){
    for(auto itr = lst.begin(); itr != lst.end(); ++itr){
        if(itr == lst.begin()){
            cout << "[" << *itr;
        }else
        cout << "," << *itr ;
    }
    cout << "]";
}

void print_s(list<tuple<int,int,int,int,int>> in){
    for(auto i: in){
        cout << " s(" << get<0>(i) << "," << get<1>(i) << "," 
             << get<2>(i) << ",!H" << get<3>(i) ;
        // print_ilist(get<3>(i));     
        cout << ")."; 
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
    string filename2 = "result_delfirst.csv";


    ifstream i_file(filename);
    if(!i_file.is_open()){
        cerr << "Could not open input file - " << filename << endl;
        return 1;
    }
    // 入力補助情報. test case では between, max, min は入力 0個 とする
    int n, n_job;
    // 解析情報．今回はガードに比較演算がないので HL の id の数だけ
    int n_hl;
    i_file >> n >> n_job; 
    n_hl = n_job;
    typedef int used;

    //job
    list<list<tuple<int,int,int,used>>::iterator> vec_job[n_hl];
    list<tuple<int,int,int>> p_job;
    list<tuple<int,int,int,used>> al_job;
    //job_s
    vector<list<list<tuple<int,int,int,int,used>>::iterator>> vec_job_s(n_hl);
    list<tuple<int,int,int,int>> p_job_s;
    list<tuple<int,int,int,int,used>> al_job_s;
    //n
    list<int> p_n;
    list<int> al_n;
    //f
    list<int> p_f;
    list<int> al_f;
    //nn
    list<int> p_nn;
    list<int> al_nn;
    //m
    list<int> p_m;
    list<int> al_m;
    //s
    vector<list<list<tuple<int,int,int,int,used>>::iterator>> vec_s;
    list<tuple<int,int,int,int>> p_s;
    list<tuple<int,int,int,int,used>> al_s;
    bool flag = true;

    int link1, link2, link3, link4;
    string tmp1,tmp2;
    for (size_t i = 0; i < n_job; i++)
    {
        i_file >> link1 >> link2 >> link3;
        p_job.push_back({link1,link2,link3});
        cout << link1 << "," << link2 << "," << link3 << endl;
    }
    p_n.push_back(n);

    //start calculate transitive closure
    auto start = std::chrono::steady_clock::now();
    i_pair path_tmp;

    using ht1 = std::tuple<int,int>;
    using ht2 = std::tuple<int,int,int,int>;
    const auto hash1 = [](const ht1 &key){
        return (get<0>(key)*4027+get<1>(key));
    };
    const auto hash2 = [](const ht2 &key){
        return (get<0>(key)*4027+get<1>(key)*4049+get<2>(key)*4057
                +get<3>(key));
    };

    ska::flat_hash_set<ht1, decltype(hash1)> history_rule1(1024,hash1);
    ska::flat_hash_set<ht2, decltype(hash2)> history_rule2(1024,hash2); 
    

    // :- uniq | f(1).
    p_f.push_back(1);
    // n(I) \:- int(I), uniq | nn(I). 
    if(!p_n.empty()){
        auto tmp = p_n.back();
        p_n.pop_back();
        p_nn.push_back(tmp);
        al_n.push_back(tmp);
       
    }

    // nn(I), f(A) :- J = I-1, B=A*I, I > 0 | nn(J),f(B).
    // nn(0), f(A) :- m(A).
    while(true){
        if(!p_nn.empty()){
            auto I = p_nn.back();
            p_nn.pop_back();
            if(!al_f.empty()){
                auto A = al_f.back();
                if(I > 0){
                    int J = I-1;
                    int B = A*I;
                    p_nn.push_back(J);
                    p_f.push_back(B);
                    al_f.pop_back();
                    continue;
                }
                if(I == 0){
                    p_m.push_back(A);
                    al_f.pop_back();
                    continue;
                }
            }else{
                al_nn.push_back(I);
            }
            continue;
        }
        if(!p_f.empty()){
            auto A = p_f.back();
            p_f.pop_back();
            if(!al_nn.empty()){
                auto I = al_nn.back();
                if(I > 0){
                    int J = I-1;
                    int B = A*I;
                    p_nn.push_back(J);
                    p_f.push_back(B);
                    al_nn.pop_back();
                    continue; 
                }
                if(I == 0){
                    p_m.push_back(A);
                    al_nn.pop_back();
                    continue;
                }
            }else{
                al_f.push_back(A);
            }
            continue;
        }
        break;
    }


    // m(N) :- M = N-1, N > 0 | m(M),s(0,1,0,[],!New).
    while(!p_m.empty()){
        auto N = p_m.back();
        p_m.pop_back();
        if(N > 0){
            int M = N-1;
            n_hl++;
            p_m.push_back(M);
            p_s.push_back({0,1,0,n_hl});
            continue;
        }
        al_m.push_back(N);
    }

    // job
    // propagate@@
    // job(W, P, !H), s(S, T, J, L, !ID) \ :-int(W), int(P), uniq(!H,!ID) | job_s(W,P,!H,!ID).
    while(!p_job.empty()){
        auto tmpjob = p_job.back();
        p_job.pop_back();
        if(!al_s.empty()){
            auto tmps = al_s.back();
            al_s.pop_back();
            // never reach here. サボる
        }
        int H = get<2>(tmpjob);
        int hlnum = vec_job[H].size()+1;
        al_job.push_back({get<0>(tmpjob),get<1>(tmpjob),H,hlnum});
        auto itr = prev(al_job.end());
        vec_job[H].push_back(itr);
    }

    // s(int,int,int,list,hl)
    // job_s
    //minimize@@
    // n(N), s(S, T, J, L, !ID1) \ s(S2, T2, J2, L2, !ID2) :- J =:= N, S < S2, int(J2), int(T2)| g(S2,L2, !ID2).
    //pertialpermution@@
    // job_s(W, P, !H, !ID), s(S, T, J, L, !ID) :- 
    // Time = T + P, Weight = T * W, Sum = Weight + S, J1 = J+1, uniq(S,J,!ID,!H) | 
    // s(Sum,Time,J1,[!H|L],!ID).
    // propagate@@
    // job(W, P, !H), s(S, T, J, L, !ID) \ :-int(W), int(P), uniq(!H,!ID) | job_s(W,P,!H,!ID).
    while(true){
        if(!p_s.empty()){
            auto tmps = p_s.back();
            int S = get<0>(tmps);
            int T = get<1>(tmps);
            int J = get<2>(tmps);
            int arg4 = get<3>(tmps);
            p_s.pop_back();

            // //rule
            //     // ひとまずこのルール無しバージョン
            //     //minimize@@
            //     // n(N), s(S, T, J, L, !ID1) \ s(S2, T2, J2, L2, !ID2) :- J =:= N, S < S2, int(J2), int(T2)| g(S2,L2, !ID2).


                //pertialpermution@@
                // job_s(W, P, !H, !ID), s(S, T, J, L, !ID) :- 
                // Time = T + P, Weight = T * W, Sum = Weight + S, J1 = J+1, uniq(S,J,!ID,!H) | 
                // s(Sum,Time,J1,[!H|L],!ID).
                bool flag = false;
                if(vec_job_s.size() > arg4)
                if(!vec_job_s[arg4].empty()){
                    for(auto itr = vec_job_s[arg4].begin(); itr != vec_job_s[arg4].end();++itr){
                        if(history_rule2.insert({S,J,arg4,get<2>(**itr)}).second){
                            int W = get<0>(**itr);
                            int P = get<1>(**itr);
                            int Time = T + P;
                            int Weight = T*W;
                            int Sum = Weight + S;
                            int J1 = J+1;
                            p_s.push_back({Sum,Time,J1,arg4});
                            cout << "uniq: " << S << "," << J << "," << arg4 << "," << get<2>(**itr) << endl;
                            cout << "\tnew s: " << Sum << "," << Time << "," << J1 << ",!H" << arg4 << endl;
                            al_job_s.erase(*itr);
                            vec_job_s[arg4].erase(itr);
                            flag = true;
                            break;
                        }
                    }
                    if(flag)continue;
                }


                // propagate@@
                // job(W, P, !H), s(S, T, J, L, !ID) \ :-int(W), int(P), uniq(!H,!ID) | job_s(W,P,!H,!ID).
                if(!al_job.empty()){
                    for(auto itr = al_job.begin(); itr != al_job.end(); ++itr){
                        int W = get<0>(*itr);
                        int P = get<1>(*itr);
                        int H = get<2>(*itr);
                        if(history_rule1.insert({H,arg4}).second){
                            p_job_s.push_back({W,P,H,arg4});
                        }
                    }
                }

            // //push to atomlist
            if(vec_s.size() < n_hl){
                vec_s.resize(n_hl+1);
            }
            int hlnum = vec_s[arg4].size()+1;
            al_s.push_back({get<0>(tmps),get<1>(tmps),get<2>(tmps),arg4,hlnum});
            auto itr = prev(al_s.end());
            vec_s[arg4].push_back(itr);
            continue;
        }
        if(!p_job_s.empty()){
            auto tmpjobs = p_job_s.back();
            p_job_s.pop_back();
            int W = get<0>(tmpjobs);
            int P = get<1>(tmpjobs);
            int arg2 = get<2>(tmpjobs);
            int arg3 = get<3>(tmpjobs);
            //rule
                //pertialpermution@@
                // job_s(W, P, !H, !ID), s(S, T, J, L, !ID) :- 
                // Time = T + P, Weight = T * W, Sum = Weight + S, J1 = J+1, uniq(S,J,!ID,!H) | 
                // s(Sum,Time,J1,[!H|L],!ID).
            if(!vec_s[arg3].empty()){
                bool flag = false;
                for(auto itr = vec_s[arg3].begin(); itr != vec_s[arg3].end(); ++itr){
                    int S = get<0>(**itr);
                    int T = get<1>(**itr);
                    int J = get<2>(**itr);
                    if(history_rule2.insert({S,J,arg3,arg2}).second){
                        int Time = T + P;
                        int Weight = T * W;
                        int Sum = Weight + S;
                        int J1 = J+1;
                        al_s.erase(*itr);
                        vec_s[arg3].erase(itr);
                        p_s.push_back({Sum,Time,J1,arg3});
                        cout << "uniq: " << S << "," << J << "," << arg3 << "," << arg2 << endl;
                        cout << "\tnew s: " << Sum << "," << Time << "," << J1 << ",!H" << arg3 << endl;
                        flag=true;
                        break;
                    }
                }
                if(flag)continue;
            }
            //rule failed
            //push to atomlist
            if(vec_job_s.size() < n_hl){
                vec_job_s.resize(n_hl+1);
            }
            int hlnum = vec_job_s[arg3].size()+1;
            al_job_s.push_back({get<0>(tmpjobs),get<1>(tmpjobs),get<2>(tmpjobs),arg3,hlnum});
            auto itr = prev(al_job_s.end());
            vec_job_s[arg3].push_back(itr);
            continue;
        }
        break;
    }
    

    auto end = std::chrono::steady_clock::now();
    long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    ofstream o_file;
    o_file.open(filename2, ios::app);

    // o_file << duration << ",";

    print_job(al_job);
    print_s(al_s);
    cout << "@4." << endl;

    // cout << try1 << "," << app1 << "," << try2 << "," << app2 << endl;

    // delete[] distance_ihhi1;
    // delete[] distance_ihhi2;
    // delete[] between_ihi;

    return 0;
}