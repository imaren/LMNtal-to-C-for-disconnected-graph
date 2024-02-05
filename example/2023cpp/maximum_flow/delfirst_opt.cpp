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

void print_i3(list<tuple<int,int,int>> in, string name){
    for(auto i: in){
        cout << name << "(" << get<0>(i) << "," << get<1>(i) << "," 
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

void print_s(list<tuple<int,int,int,list<int>,int,int>> in){
    for(auto i: in){
        cout << " between(" << get<0>(i) << "," << get<1>(i) << "," 
             << get<2>(i) ;
        print_ilist(get<3>(i));     
        cout << ")."; 
    }
}

// 削除される可能性があるルールについては全てのリスト(p, al の両方)を走査（効果なし）
// opt として、backward を forward よりも先に実行するように変更（機械的にできないので困りどころ）
// 若干削減されたがあまり効果はないか・・・？

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
    int n, n_cap;
    // 解析情報．今回はガードに比較演算がないので HL の id の数だけ
    int n_hl;
    i_file >> n >> n_cap; 
    n_hl = n;
 
    //cap
    list<list<tuple<int,int,int>>::iterator> vec_cap1[n_hl];
    list<list<tuple<int,int,int>>::iterator> vec_cap2[n_hl];
    list<tuple<int,int,int>> p_cap;
    list<tuple<int,int,int>> al_cap;
    //flow
    list<list<tuple<int,int,int>>::iterator> vec_flow1[n_hl];
    list<list<tuple<int,int,int>>::iterator> vec_flow2[n_hl];
    list<tuple<int,int,int>> p_flow;
    list<tuple<int,int,int>> al_flow;
    //start
    list<int> p_start;
    list<int> al_start;
    //goal
    list<int> p_goal;
    list<int> al_goal;
    //step
    vector<list<list<int>::iterator>> vec_step(n_hl);
    vector<list<list<int>::iterator>> vec_p_step(n_hl);
    list<int> p_step;
    list<int> al_step;
    //do
    vector<list<list<int>::iterator>> vec_do(n_hl);
    vector<list<list<int>::iterator>> vec_p_do(n_hl);
    list<int> p_do;
    list<int> al_do;
    //path
    vector<list<list<tuple<list<int>,int,int>>::iterator>> vec_path1(n_hl);
    vector<list<list<tuple<list<int>,int,int>>::iterator>> vec_path3(n_hl);
    list<tuple<list<int>,int,int>> p_path;
    list<tuple<list<int>,int,int>> al_path;
    vector<list<list<tuple<list<int>,int,int>>::iterator>> vec_p_path(n_hl);
    //inc
    vector<list<list<tuple<list<int>,int,int>>::iterator>> vec_inc1(n_hl);
    vector<list<list<tuple<list<int>,int,int>>::iterator>> vec_inc3(n_hl);
    list<tuple<list<int>,int,int>> p_inc;
    list<tuple<list<int>,int,int>> al_inc;
    bool flag = true;

    int link1, link2, link3, link4;
    string tmp1,tmp2;
    // start, goal を input
    i_file >> link1 >> link2;
    p_start.push_back(link1); p_goal.push_back(link2);
    for (size_t i = 0; i < n_cap; i++)
    {
        i_file >> link1 >> link2 >> link3;
        p_cap.push_back({link1,link2,link3});
        // cout << link1 << "," << link2 << "," << link3 << endl;
    }
    //start calculate transitive closure
    auto start = std::chrono::steady_clock::now();
    i_pair path_tmp;

    using ht1 = std::tuple<int,int>;
    using ht2 = std::tuple<int,int,int>;
    const auto hash1 = [](const ht1 &key){
        return (get<0>(key)*4027+get<1>(key));
    };
    const auto hash2 = [](const ht2 &key){
        return (get<0>(key)*4027+get<1>(key)*4049+get<2>(key));
    };

    ska::flat_hash_set<ht1, decltype(hash1)> history_rule1(1024,hash1);
    ska::flat_hash_set<ht1, decltype(hash1)> history_rule2(1024,hash1); 
    ska::flat_hash_set<ht2, decltype(hash2)> history_rule3(1024,hash2);  
    ska::flat_hash_set<ht2, decltype(hash2)> history_rule4(1024,hash2);  
    // :- uniq | step(!H), do(!H).
    vec_p_step.resize(n_hl+2);
    vec_p_do.resize(n_hl+2);
    vec_p_path.resize(n_hl+2);
    p_step.push_back(n_hl);
    auto sitr = prev(p_step.end());
    vec_p_step[n_hl].push_back(sitr);
    p_do.push_back(n_hl);
    auto ditr = prev(p_do.end());
    vec_p_do[n_hl].push_back(ditr);
    n_hl++;
    // cap
    // cap(!F,!T,C) \:- int(C), uniq(!F,!T) | flow(!F,!T,0).
    while(!p_cap.empty()){
        auto tmp = p_cap.back();
        p_cap.pop_back();
        auto F = get<0>(tmp);
        auto T = get<1>(tmp);
        //rule
        if(history_rule1.insert({F,T}).second){
            p_flow.push_back({F,T,0});
        }
        //never reach other rules なのでサボる

        //push
        al_cap.push_back(tmp);
        auto itr = prev(al_cap.end());
        vec_cap1[F].push_back(itr);
        vec_cap2[T].push_back(itr);
    }

    //start
    while(!p_start.empty()){
        // rule never fire なのでサボる
        auto tmp = p_start.back();
        p_start.pop_back();
        al_start.push_back(tmp);
    }
    //goal
    while(!p_goal.empty()){
        // rule never fire なのでサボる
        auto tmp = p_goal.back();
        p_goal.pop_back();
        al_goal.push_back(tmp);
    }
    //順番
    //inc,step,do,path,flow,
    while(true){
        if(!p_inc.empty()){
            auto tmp = p_inc.back();
            p_inc.pop_back();
            auto arg1 = get<0>(tmp);
            auto arg2 = get<1>(tmp);
            auto arg3 = get<2>(tmp);

            // cout << "push: inc[" ; 
            // for(auto l: arg1){
            //     cout << l << "," ;
            // }
            // cout << "], " << arg2 << "," << arg3 << endl;

            // inc([!B,!A|L], F, !ST), flow(!A,!B,N) 
            //  :- M = N+F | inc([!A|L], F, !ST), flow(!A,!B,M).
            //inc([!B,!A|L], F, !ST), flow(!B,!A,N) :- M = N-F | inc([!A|L], F, !ST), flow(!B,!A,M).
            if(arg1.size() > 1){
                auto itr = arg1.begin();
                int B = *itr;
                ++itr;
                int A = *itr;
                bool flag = false;
                if(vec_flow2[B].size() > 0)
                for(auto i = vec_flow2[B].begin(); i != vec_flow2[B].end(); ++i){
                    if(get<0>(**i)==A){
                        int M = arg2 + get<2>(**i);
                        // cout << "  update: " << A << "," << B << "->" << arg2 <<"+"<< get<2>(**i) <<"="<< M << endl;
                        arg1.pop_front();
                        p_inc.push_back({arg1,arg2,arg3});
                        p_flow.push_back({A,B,M});

                        if(vec_flow1[A].size() > 0)
                        for(auto j = vec_flow1[A].begin(); j != vec_flow1[A].end(); ++j){
                            if(*j == *i){
                                vec_flow1[A].erase(j);
                                break;
                            }
                        }
                        al_flow.erase(*i);
                        vec_flow2[B].erase(i);
                        flag = true;
                        break;
                    }
                }
                if(flag){
                    continue;
                }
                if(vec_flow1[B].size() > 0)
                for(auto i = vec_flow1[B].begin(); i != vec_flow1[B].end(); ++i){
                    if(get<1>(**i)==A){
                        int M = get<2>(**i)-arg2;
                        arg1.pop_front();
                        p_inc.push_back({arg1,arg2,arg3});
                        p_flow.push_back({B,A,M});
                        // cout << "  update back: " << B << "," << A << "->" << get<2>(**i) <<"-"<< arg2 <<"="<< M << endl;
                        
                        if(vec_flow2[A].size() > 0)
                        for(auto j = vec_flow2[A].begin(); j != vec_flow2[A].end(); ++j){
                            if(*j == *i){
                                vec_flow2[A].erase(j);
                                break;
                            }
                        }
                        al_flow.erase(*i);
                        vec_flow1[B].erase(i);
                        flag = true;
                        break;
                    }
                }
                if(flag){
                    continue;
                }
            }
            // inc([!S], F, !ST), step(!ST) :- int(F) | step(!New), do(!New).
            if(arg1.size()==1){
                if(n_hl > vec_step.size()){
                    vec_step.resize(n_hl+1);
                }
                bool flag = false;
                if(vec_step[arg3].size() > 0)
                for(auto i = vec_step[arg3].begin(); i != vec_step[arg3].end();++i){
                    p_step.push_back(n_hl);
                    auto sitr = prev(p_step.end());
                    vec_p_step[n_hl].push_back(sitr);
                    p_do.push_back(n_hl);
                    auto ditr = prev(p_do.end());
                    vec_p_do[n_hl].push_back(ditr);
                    n_hl++;
                    vec_p_step.resize(n_hl+1);
                    vec_p_do.resize(n_hl+1);
                    vec_p_path.resize(n_hl+1);
                    flag = true;
                    al_step.erase(*i);
                    vec_step[arg3].erase(i);
                    break;
                }
                if(flag){
                    continue;
                }
            }

            al_inc.push_back({arg1,arg2,arg3});
            auto nitr = prev(al_inc.end());
            vec_inc1[arg1.front()].push_back(nitr);
            vec_inc3[arg3].push_back(nitr);
            continue;
        }
        /*
        next_step@@
        inc([!S], F, !ST), step(!ST) :- int(F) | step(!New), do(!New).
        check_path@@
        step(!ST), goal(!G) \ path([!G|L], F, !ST), do(!ST)
            :-  
            |  inc([!G|L], F, !ST).

        find_startpath@@
        start(!A),cap(!A,!B,M),flow(!A,!B,N),step(!ST),do(!ST)\
            :- R = M-N, R > 0, uniq(!ST,!B)
            |path([!B,!A], R, !ST).
        propagate_path@@
        step(!ST), do(!ST), path([!A|L], F, !ST),cap(!A,!B,M),flow(!A,!B,N) \
            :- int(F), ground(L), R = M-N, uniq(!ST,!A,!B)
            | path([!B,!A|L], min(F,R), !ST).
        */
        if(!p_step.empty()){
            auto tmp = p_step.back();
            p_step.pop_back();
            vec_p_step[tmp].pop_back();
            //inc([!S], F, !ST), step(!ST) :- int(F) | step(!New), do(!New).
            bool flag = false;
            if(vec_inc3.size() < n_hl){
                vec_inc3.resize(n_hl+1);
            }
            if(vec_inc3[tmp].size() > 0)
            for(auto i = vec_inc3[tmp].begin(); i != vec_inc3[tmp].end(); ++i){
                auto S = get<0>(**i);
                if(S.size() == 1){
                    for(auto j = vec_inc1[S.front()].begin(); j != vec_inc1[S.front()].end(); ++j){
                        if(*j == *i){
                            vec_inc1[S.front()].erase(j);
                            break;
                        }
                    }
                    al_inc.erase(*i);
                    vec_inc3[tmp].erase(i);
                    flag = true;
                    p_step.push_back(n_hl);
                    auto sitr = prev(p_step.end());
                    vec_p_step[n_hl].push_back(sitr);
                    p_do.push_back(n_hl);
                    auto ditr = prev(p_do.end());
                    vec_p_do[n_hl].push_back(ditr);
                    n_hl++;
                    vec_p_step.resize(n_hl+1);
                    vec_p_do.resize(n_hl+1);
                    vec_p_path.resize(n_hl+1);
                    break;
                }
            }
            if(flag){
                continue;
            }

            //step(!ST), goal(!G) \ path([!G|L], F, !ST), do(!ST)
            // :-  inc([!G|L], F, !ST).
            if(vec_do.size() < n_hl)vec_do.resize(n_hl+1);
            if(vec_path3.size() < n_hl){
                vec_path1.resize(n_hl+1);
                vec_path3.resize(n_hl+1);
            }
            if(vec_do[tmp].size() > 0)
            for(auto j = vec_do[tmp].begin(); j!=vec_do[tmp].end(); ){
                bool fin = true;
                if(vec_path3[tmp].size() > 0)
                for(auto i = vec_path3[tmp].begin(); i != vec_path3[tmp].end(); ++i){
                    auto L = get<0>(**i);
                    auto G = L.front();
                    auto G2 = al_goal.back();
                    if(G == G2){
                        auto F = get<1>(**i);
                        for(auto j = vec_path1[G].begin(); j != vec_path1[G].end(); ++j){
                            if(*j == *i){
                                vec_path1[G].erase(j);
                                break;
                            }
                        }
                        al_do.erase(*j);
                        j = vec_do[tmp].erase(j);
                        al_path.erase(*i);
                        vec_path3[tmp].erase(i);
                        p_inc.push_back({L,F,tmp});
                        fin = false;
                        break;
                    }
                }
                if(fin)break;
                ++j;
            }

            // start(!A),cap(!A,!B,M),flow(!A,!B,N),step(!ST),do(!ST)\
            // :- R = M-N, R > 0, uniq(!ST,!B)
            // |path([!B,!A], R, !ST).
            if(!vec_do[tmp].empty()){
                for(auto i = al_start.begin(); i != al_start.end(); ++i){
                    for(auto j = vec_cap1[*i].begin(); j != vec_cap1[*i].end(); ++j){
                        auto B = get<1>(**j);
                        for(auto k = vec_flow1[*i].begin(); k != vec_flow1[*i].end(); ++k){
                            if(get<1>(**k) == B){
                                auto M = get<2>(**j);
                                auto N = get<2>(**k);
                                auto R = M-N;
                                if(R > 0){
                                    if(history_rule2.insert({tmp,B}).second){
                                        int A = *i;
                                        list<int> nlist = {B,A};
                                        // cout << "newstart: [" << B << "," << A << "], " << R << "," << tmp <<endl;
                                        p_path.push_back({nlist,R,tmp});
                                        auto pitr = prev(p_path.end());
                                        vec_p_path[tmp].push_back(pitr);
                                    }
                                }
                            }
                        }
                    }
                }
                // step(!ST), do(!ST), path([!A|L], F, !ST),cap(!A,!B,M),flow(!A,!B,N) \
                // :- int(F), ground(L), R = M-N, R > 0 uniq(!ST,!A,!B)
                // | path([!B,!A|L], min(F,R), !ST).
                for(auto i = vec_path3[tmp].begin(); i != vec_path3[tmp].end(); ++i){
                    auto L = get<0>(**i);
                    auto A = L.front();
                    //backward
                    for(auto j = vec_cap2[A].begin(); j != vec_cap2[A].end(); ++j){
                        auto B = get<0>(**j);
                        for(auto k = vec_flow2[A].begin(); k != vec_flow2[A].end(); ++k){
                            if(get<0>(**k) == B){
                                auto N = get<2>(**k);
                                if(N > 0){
                                    if(history_rule4.insert({tmp,A,B}).second){
                                        int F = get<1>(**i);
                                        int min = (F<N)?F:N;
                                        // cout << "min: " << F << "," << N << "=" << min << endl;
                                        auto nlist = L;
                                        nlist.push_front(A);
                                        // cout << "match backward: [";
                                        // for(auto l : L ){
                                        //     cout << l << "," ;
                                        // }
                                        // cout << "]" << endl;
                                        p_path.push_back({nlist,min,tmp});
                                        auto pitr = prev(p_path.end());
                                        vec_p_path[tmp].push_back(pitr);
                                    }
                                }
                            }
                        }
                    }
                    for(auto j = vec_cap1[A].begin(); j != vec_cap1[A].end(); ++j){
                        auto B = get<1>(**j);
                        for(auto k = vec_flow1[A].begin(); k != vec_flow1[A].end(); ++k){
                            if(get<1>(**k) == B){
                                auto M = get<2>(**j);
                                auto N = get<2>(**k);
                                auto R = M-N;
                                // cout << "cap:" << get<0>(**j) << "," << get<1>(**j) << "," << get<2>(**j) << "vs"
                                //      << "flow:" << get<0>(**k) << "," << get<1>(**k) << "," << get<2>(**k) << endl;
                                // cout << "!!!cap: " << M << "-" << N << "=" << R << endl;
                                if(R > 0){
                                    if(history_rule3.insert({tmp,A,B}).second){
                                        int F = get<1>(**i);
                                        int min = (F<R)?F:R;
                                        // cout << "min: " << F << "," << R << "=" << min << endl;
                                        auto nlist = L;
                                        nlist.push_front(B);
                                        //  cout << "match forward: [";
                                        // for(auto l : L ){
                                        //     cout << l << "," ;
                                        // }
                                        // cout << "]" << endl;
                                        p_path.push_back({nlist,min,tmp});
                                        auto pitr = prev(p_path.end());
                                        vec_p_path[tmp].push_back(pitr);
                                    }
                                }
                            }
                        }
                    }
                    
                }
            }

            if(vec_step.size() < n_hl){
                vec_step.resize(n_hl+1);
            }
            al_step.push_back({tmp});
            auto nitr = prev(al_step.end());
            vec_step[tmp].push_back(nitr);
            continue;
        }
        if(!p_do.empty()){
            auto tmp = p_do.back();
            p_do.pop_back();
            vec_p_do[tmp].pop_back();
            //step(!ST), goal(!G) \ path([!G|L], F, !ST), do(!ST)
            // :-  inc([!G|L], F, !ST).
            if(vec_step.size() < n_hl)vec_step.resize(n_hl+1);
            if(vec_path3.size() < n_hl){
                vec_path1.resize(n_hl+1);
                vec_path3.resize(n_hl+1);
            }
            if(!vec_step[tmp].empty() || !vec_p_step[tmp].empty()){
                bool flag = false;
                if(vec_path3[tmp].size() > 0)
                for(auto i = vec_path3[tmp].begin(); i != vec_path3[tmp].end(); ++i){
                    auto L = get<0>(**i);
                    auto G = L.front();
                    auto G2 = al_goal.back();
                    if(G == G2){
                        auto F = get<1>(**i);
                        for(auto j = vec_path1[G].begin(); j != vec_path1[G].end(); ++j){
                            if(*j == *i){
                                vec_path1[G].erase(j);
                                break;
                            }
                        }
                        al_path.erase(*i);
                        vec_path3[tmp].erase(i);
                        p_inc.push_back({L,F,tmp});
                        flag = true;
                        break;
                    }
                }
                if(flag)continue;
                if(vec_p_path[tmp].size() > 0)
                for(auto i = vec_p_path[tmp].begin(); i != vec_p_path[tmp].end(); ++i){
                    auto L = get<0>(**i);
                    auto G = L.front();
                    auto G2 = al_goal.back();
                    if(G == G2){
                        auto F = get<1>(**i);
                        al_path.erase(*i);
                        vec_path3[tmp].erase(i);
                        p_inc.push_back({L,F,tmp});
                        flag = true;
                        break;
                    }
                }
                if(flag)continue;
            }

            // start(!A),cap(!A,!B,M),flow(!A,!B,N),step(!ST),do(!ST)\
            //     :- R = M-N, R > 0, uniq(!ST,!B)
            //     |path([!B,!A], R, !ST).
            if(!vec_step[tmp].empty()){
                for(auto i = al_start.begin(); i != al_start.end(); ++i){
                    for(auto j = vec_cap1[*i].begin(); j != vec_cap1[*i].end(); ++j){
                        auto A = get<0>(**j);
                        auto B = get<1>(**j);
                        for(auto k = vec_flow1[*i].begin(); k != vec_flow1[*i].end(); ++k){
                            if(get<1>(**k) == B){
                                auto M = get<2>(**j);
                                auto N = get<2>(**k);
                                auto R = M-N;
                                if(R > 0){
                                    if(history_rule2.insert({tmp,B}).second){
                                        list<int> nlist = {B,A};
                                        // cout << "newstart: [" << A << "," << B << "], " << R << "," << tmp <<endl;
                                        p_path.push_back({nlist,R,tmp});
                                        auto pitr = prev(p_path.end());
                                        vec_p_path[tmp].push_back(pitr);
                                    }
                                }
                            }
                        }
                    }
                }
                // step(!ST), do(!ST), path([!A|L], F, !ST),cap(!A,!B,M),flow(!A,!B,N) \
                // :- int(F), ground(L), R = M-N, R > 0 uniq(!ST,!A,!B)
                // | path([!B,!A|L], min(F,R), !ST).
                for(auto i = vec_path3[tmp].begin(); i != vec_path3[tmp].end(); ++i){
                    auto L = get<0>(**i);
                    auto A = L.front();
                    //backward
                    for(auto j = vec_cap2[A].begin(); j != vec_cap2[A].end(); ++j){
                        auto B = get<0>(**j);
                        for(auto k = vec_flow2[A].begin(); k != vec_flow2[A].end(); ++k){
                            if(get<0>(**k) == B){
                                auto N = get<2>(**k);
                                if(N > 0){
                                    if(history_rule4.insert({tmp,A,B}).second){
                                        int F = get<1>(**i);
                                        int min = (F<N)?F:N;
                                        // cout << "min: " << F << "," << N << "=" << min << endl;
                                        auto nlist = L;
                                        nlist.push_front(A);
                                        // cout << "match backward: [";
                                        // for(auto l : L ){
                                        //     cout << l << "," ;
                                        // }
                                        // cout << "]" << endl;
                                        p_path.push_back({nlist,min,tmp});
                                        auto pitr = prev(p_path.end());
                                        vec_p_path[tmp].push_back(pitr);
                                    }
                                }
                            }
                        }
                    }
                    for(auto j = vec_cap1[A].begin(); j != vec_cap1[A].end(); ++j){
                        auto B = get<1>(**j);
                        for(auto k = vec_flow1[A].begin(); k != vec_flow1[A].end(); ++k){
                            if(get<1>(**k) == B){
                                auto M = get<2>(**j);
                                auto N = get<2>(**k);
                                auto R = M-N;
                                // cout << "cap:" << get<0>(**j) << "," << get<1>(**j) << "," << get<2>(**j) << "vs"
                                //      << "flow:" << get<0>(**k) << "," << get<1>(**k) << "," << get<2>(**k) << endl;
                                // cout << "!!cap: " << M << "-" << N << "=" << R << endl;
                                if(R > 0){
                                    if(history_rule3.insert({tmp,A,B}).second){
                                        int F = get<1>(**i);
                                        int min = (F<R)?F:R;
                                        // cout << "min: " << F << "," << R << "=" << min << endl;
                                        auto nlist = L;
                                        nlist.push_front(B);
                                        //  cout << "match forward: [";
                                        // for(auto l : L ){
                                        //     cout << l << "," ;
                                        // }
                                        // cout << "]" << endl;
                                        p_path.push_back({nlist,min,tmp});
                                        auto pitr = prev(p_path.end());
                                        vec_p_path[tmp].push_back(pitr);
                                    }
                                }
                            }
                        }
                    }
                    
                }
            }


            if(vec_do.size() < n_hl){
                vec_do.resize(n_hl+1);
            }
            al_do.push_back(tmp);
            auto nitr = prev(al_do.end());
            vec_do[tmp].push_back(nitr);
            continue;
        }
        if(!p_path.empty()){
            auto tmp = p_path.back();
            p_path.pop_back();
            auto arg1 = get<0>(tmp);
            auto  H = arg1.front(); 
            auto arg2 = get<1>(tmp);
            auto arg3 = get<2>(tmp);
            vec_p_path[arg3].pop_back();
            // cout << "\tpush path[" ;
            // for(auto l: arg1){
            //     cout << l << "," ;
            // }
            // cout << "]," << arg2 << "," << arg3 << endl;

            // step(!ST), goal(!G) \ path([!G|L], F, !ST), do(!ST)
            //     :-  inc([!G|L], F, !ST).
            if((!vec_step[arg3].empty()) || (!vec_p_step[arg3].empty())){
                if(al_goal.front() == H ){
                    // cout << "else" << endl;
                    if(!vec_do[arg3].empty()){
                        auto i = vec_do[arg3].front();
                        vec_do[arg3].pop_front();
                        al_do.erase(i);
                        p_inc.push_back(tmp);
                        continue;
                    }
                    else if(!vec_p_do[arg3].empty()){
                        auto i = vec_p_do[arg3].front();
                        vec_p_do[arg3].pop_front();
                        p_do.erase(i);
                        p_inc.push_back(tmp);
                        continue;
                    }
                }
            }

            // step(!ST), do(!ST), path([!A|L], F, !ST),cap(!A,!B,M),flow(!A,!B,N) \
            //     :- int(F), ground(L), R = M-N, R > 0, uniq(!ST,!A,!B)
            //     | path([!B,!A|L], min(F,R), !ST).
            if(!vec_step[arg3].empty()){
                if(!vec_do[arg3].empty()){
                    //backward
                    if(vec_cap2[H].size() > 0)
                    for(auto i = vec_cap2[H].begin(); i != vec_cap2[H].end(); ++i){
                        auto B = get<0>(**i);
                        if(vec_flow2[H].size() > 0)
                        for(auto j = vec_flow2[H].begin(); j != vec_flow2[H].end(); ++j){
                            if(get<0>(**j) == B){
                                int N = get<2>(**j);
                                if(N > 0){
                                    if(history_rule4.insert({arg3,H,B}).second){
                                        auto nlist = arg1;
                                        nlist.push_front(B);
                                        // cout << "match backward: [";
                                        // for(auto l : arg1 ){
                                        //     cout << l << "," ;
                                        // }
                                        // cout << "]" << endl;
                                        int min = (arg2<N)?arg2:N;
                                        p_path.push_back({nlist,min,arg3});
                                        auto pitr = prev(p_path.end());
                                        vec_p_path[arg3].push_back(pitr);
                                    }
                                }
                            }
                        }
                    }   
                    if(vec_cap1[H].size() > 0)
                    for(auto i = vec_cap1[H].begin(); i != vec_cap1[H].end(); ++i){
                        auto B = get<1>(**i);
                        if(vec_flow1[H].size() > 0)
                        for(auto j = vec_flow1[H].begin(); j != vec_flow1[H].end(); ++j){
                            if(get<1>(**j) == B){
                                int R = get<2>(**i) - get<2>(**j);
                                // cout << "cap:" << get<0>(**i) << "," << get<1>(**i) << "," << get<2>(**i) << "vs"
                                //      << "flow:" << get<0>(**j) << "," << get<1>(**j) << "," << get<2>(**j) << endl;
                                // cout << "!!!!cap: " << get<2>(**i) << "-" << get<2>(**j) << "=" << R << endl;
                                if(R > 0){
                                    if(history_rule3.insert({arg3,H,B}).second){
                                        auto nlist = arg1;
                                        nlist.push_front(B);
                                        //  cout << "match forward: [";
                                        // for(auto l : arg1 ){
                                        //     cout << l << "," ;
                                        // }
                                        // cout << "]" << endl;
                                        
                                        int min = (arg2<R)?arg2:R;
                                        // cout << "min: " << arg2 << "," << R << "=" << min << endl;
                                        p_path.push_back({nlist,min,arg3});
                                        auto pitr = prev(p_path.end());
                                        vec_p_path[arg3].push_back(pitr);
                                    }
                                }
                            }
                        }
                    }   
                }
            }

            if(vec_path1.size() < n_hl){
                vec_path1.resize(n_hl+1);
                vec_path3.resize(n_hl+1);
            }
            al_path.push_back(tmp);
            auto nitr = prev(al_path.end());
            vec_path1[H].push_back(nitr);
            vec_path3[arg3].push_back(nitr);
            continue;
        }
        if(!p_flow.empty()){
            auto tmp = p_flow.back();
            p_flow.pop_back();
            auto A = get<0>(tmp);
            auto B = get<1>(tmp);
            auto N = get<2>(tmp);
            
            //inc([!B,!A|L], F, !ST), flow(!A,!B,N) 
            //  :- M = N+F | inc([!A|L], F, !ST), flow(!A,!B,M).
            bool flag = false;
            if(!vec_inc1[B].empty())
            for(auto i = vec_inc1[B].begin(); i != vec_inc1[B].end(); ++i){
                auto L = get<0>(**i);
                if(L.size() > 1){
                    auto itr = L.begin();
                    auto fst = *itr;
                    ++itr;
                    auto snd = *itr;
                    if(snd == A){
                        auto F = get<1>(**i);
                        auto ST = get<2>(**i);
                        auto M = N+F;
                        // cout << "  update: " << A << "," << B << "->" << N <<"+"<<F<<"="<< M << endl;
                        L.pop_front();
                        p_inc.push_back({L,F,ST});
                        p_flow.push_back({A,B,M});
                        
                        for(auto j = vec_inc3[ST].begin(); j != vec_inc3[ST].end(); ++j){
                            if(*j == *i){
                                vec_inc3[ST].erase(j);
                                break;
                            }
                        }
                        al_inc.erase(*i);
                        vec_inc1[B].erase(i);
                        flag = true;
                        break;
                    }
                }

            }
            if(flag) continue;
            //decrease
            if(!vec_inc1[A].empty())
            for(auto i = vec_inc1[A].begin(); i != vec_inc1[A].end(); ++i){
                auto L = get<0>(**i);
                if(L.size() > 1){
                    auto itr = L.begin();
                    auto fst = *itr;
                    ++itr;
                    auto snd = *itr;
                    if(snd == B){
                        auto F = get<1>(**i);
                        auto ST = get<2>(**i);
                        auto M = N-F;
                        L.pop_front();
                        p_inc.push_back({L,F,ST});
                        p_flow.push_back({A,B,M});
                        // cout << "  update: " << A << "," << B << "->" << N <<"-"<< F <<"="<< M << endl;
                        
                        for(auto j = vec_inc3[ST].begin(); j != vec_inc3[ST].end(); ++j){
                            if(*j == *i){
                                vec_inc3[ST].erase(j);
                                break;
                            }
                        }
                        al_inc.erase(*i);
                        vec_inc1[A].erase(i);
                        flag = true;
                        break;
                    }
                }

            }
            if(flag) continue;

            // start(!A),cap(!A,!B,M),flow(!A,!B,N),step(!ST),do(!ST)\
            //  :- R = M-N, R > 0, uniq(!ST,!B)
            //  |path([!B,!A], R, !ST).
            // step(!ST), do(!ST), path([!A|L], F, !ST),cap(!A,!B,M),flow(!A,!B,N) \
            //  :- int(F), ground(L), R = M-N, R > 0, uniq(!ST,!A,!B)
            //  | path([!B,!A|L], min(F,R), !ST).
            auto sA = al_start.front();
            for(auto s = al_step.begin(); s != al_step.end(); ++s){
                auto ST = *s;
                if(vec_do[ST].size() > 0){
                    //backward
                    if(vec_cap2[B].size() > 0)
                    for(auto i = vec_cap2[B].begin(); i != vec_cap2[B].end(); ++i){
                        if(get<0>(**i) == A){
                            if(vec_path1[B].size() > 0)
                            for(auto j = vec_path1[B].begin(); j != vec_path1[B].end(); ++j){
                                if(ST == get<2>(**j)){
                                    if(N>0){
                                        if(history_rule4.insert({ST,B,A}).second){
                                            auto L = get<0>(**j);
                                            int F = get<1>(**j);
                                            L.push_front(A);
                                            int min = (F<N)?F:N;
                                            // cout << "min: " << F << "," << N << "=" << min << endl;
                                            p_path.push_back({L,min,ST});
                                            auto pitr = prev(p_path.end());
                                            vec_p_path[ST].push_back(pitr);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if(vec_cap1[A].size() > 0)
                    for(auto i = vec_cap1[A].begin(); i != vec_cap1[A].end(); ++i){
                        if(sA == A){
                            if(get<1>(**i) == B){
                                int M = get<2>(**i);
                                int R = M-N;
                                if(R>0){
                                    if(history_rule2.insert({ST,B}).second){
                                        list<int> nlist = {B,A};
                                        // cout << "newstart: [" << A << "," << B << "], " << R << "," << ST <<endl;
                                        p_path.push_back({nlist,R,ST});
                                        auto pitr = prev(p_path.end());
                                        vec_p_path[ST].push_back(pitr);
                                    }
                                } 
                            }
                        }
                        if(get<1>(**i) == B){
                            if(vec_path1[A].size() > 0)
                            for(auto j = vec_path1[A].begin(); j != vec_path1[A].end(); ++j){
                                if(ST == get<2>(**j)){
                                    int M = get<2>(**i);
                                    int R = M-N;
                                    // cout << "cap:" << get<0>(**i) << "," << get<1>(**i) << "," << get<2>(**i) << "vs"
                                    //     << "flow:" << A << "," << B << "," << N << endl;
                                    // cout << "!!!!!!!!cap: " << M << "-" << N << "=" << R << endl;
                                    if(R>0){
                                        if(history_rule3.insert({ST,A,B}).second){
                                            auto L = get<0>(**j);
                                            int F = get<1>(**j);
                                            L.push_front(B);
                                            // cout << "match forward: [";
                                            // for(auto l : L ){
                                            //     cout << l << "," ;
                                            // }
                                            // cout << "]" << endl;
                                            int min = (F<R)?F:R;
                                            // cout << "min: " << F << "," << R << "=" << min << endl;
                                            p_path.push_back({L,min,ST});
                                            auto pitr = prev(p_path.end());
                                            vec_p_path[ST].push_back(pitr);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;    
                }
            }
            al_flow.push_back(tmp);
            auto nitr = prev(al_flow.end());
            vec_flow1[A].push_back(nitr);
            vec_flow2[B].push_back(nitr);
            continue;
        }
        
        break;
    }

    auto end = std::chrono::steady_clock::now();
    long duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    ofstream o_file;
    o_file.open(filename2, ios::app);

    o_file << duration << ",";

    cout << "history sizse:" << endl;
    cout << history_rule1.size() << ", " << history_rule2.size() << ", " 
         << history_rule3.size() << ", " << history_rule4.size() << endl;
    int maxflow = 0;
    int G = al_goal.front();
    for(auto l : vec_flow2[G]){
        maxflow += get<2>(*l);
    }
    cout << "maxflow(" << maxflow << ").";

    // cout << "goal(" << al_goal.back() << ").";
    // string name = "flow";
    // print_i3(al_flow,name);
    // name = "cap";
    // print_i3(al_cap, name);
    cout << "@4." << endl;

    // cout << try1 << "," << app1 << "," << try2 << "," << app2 << endl;

    // delete[] distance_ihhi1;
    // delete[] distance_ihhi2;
    // delete[] between_ihi;

    return 0;
}