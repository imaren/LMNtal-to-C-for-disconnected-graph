#include <iostream>
#include <unordered_set>
#include <string>
#include <iterator>
#include <fstream>
#include <list>
#include <vector>

using namespace std;

void print_i3(list<tuple<int,int,int>> in, string name){
    for(auto i: in){
        cout << name << "(" << get<0>(i) << "," << get<1>(i) << "," 
             << get<2>(i) << ")."; 
    }
}

int main(int argc, char *argv[]){

    string filename = "input.txt";
    if(argc >= 2){
        filename = argv[1];
    }
    ifstream i_file(filename);
    if(!i_file.is_open()){
        cerr << "Could not open input file - " << filename << endl;
        return 1;
    }
    int n, n_cap;
    int n_hl;
    i_file >> n >> n_cap; 
    n_hl = n;
 
    //cap
    list<list<tuple<int,int,int>>::iterator> IS_cap_hhi_al_1[n_hl];
    list<list<tuple<int,int,int>>::iterator> IS_cap_hhi_al_2[n_hl];
    list<tuple<int,int,int>> D_cap_hhi_p;
    list<tuple<int,int,int>> D_cap_hhi_al;
    //flow
    list<list<tuple<int,int,int>>::iterator> IS_flow_hhi_al_1[n_hl];
    list<list<tuple<int,int,int>>::iterator> IS_flow_hhi_al_2[n_hl];
    list<tuple<int,int,int>> D_flow_hhi_p;
    list<tuple<int,int,int>> D_flow_hhi_al;
    //start
    list<int> D_start_h_p;
    list<int> D_start_h_al;
    //goal
    list<int> D_goal_h_p;
    list<int> D_goal_h_al;
    //step
    vector<list<list<int>::iterator>> IS_step_h_al_1(n_hl);
    vector<list<list<int>::iterator>> IS_step_h_p_1(n_hl);
    list<int> D_step_h_p;
    list<int> D_step_h_al;
    //do
    vector<list<list<int>::iterator>> IS_do_h_al_1(n_hl);
    vector<list<list<int>::iterator>> IS_do_h_p_1(n_hl);
    list<int> D_do_h_p;
    list<int> D_do_h_al;
    //path
    vector<list<list<tuple<list<int>,int,int>>::iterator>> IS_path_lih_al_1(n_hl);
    vector<list<list<tuple<list<int>,int,int>>::iterator>> IS_path_lih_al_3(n_hl);
    list<tuple<list<int>,int,int>> D_path_lih_p;
    list<tuple<list<int>,int,int>> D_path_lih_al;
    vector<list<list<tuple<list<int>,int,int>>::iterator>> IS_path_lih_p_1(n_hl);
    //inc
    vector<list<list<tuple<list<int>,int,int>>::iterator>> IS_inc_lih_al_1(n_hl);
    vector<list<list<tuple<list<int>,int,int>>::iterator>> IS_inc_lih_al_3(n_hl);
    list<tuple<list<int>,int,int>> D_inc_lih_p;
    list<tuple<list<int>,int,int>> D_inc_lih_al;
    bool flag = true;

    int link1, link2, link3, link4;
    string tmp1,tmp2;
    i_file >> link1 >> link2;
    D_start_h_p.push_back(link1); D_goal_h_p.push_back(link2);
    for (size_t i = 0; i < n_cap; i++)
    {
        i_file >> link1 >> link2 >> link3;
        D_cap_hhi_p.push_back({link1,link2,link3});
    }

    using ht1 = std::tuple<int,int>;
    using ht2 = std::tuple<int,int,int>;
    const auto hash1 = [](const ht1 &key){
        return (get<0>(key)*4027+get<1>(key));
    };
    const auto hash2 = [](const ht2 &key){
        return (get<0>(key)*4027+get<1>(key)*4049+get<2>(key));
    };

    std::unordered_set<ht1, decltype(hash1)> history_rule1(1024,hash1);
    std::unordered_set<ht1, decltype(hash1)> history_rule2(1024,hash1); 
    std::unordered_set<ht2, decltype(hash2)> history_rule3(1024,hash2);  
    std::unordered_set<ht2, decltype(hash2)> history_rule4(1024,hash2);  
    // :- uniq | step(!H), do(!H).
    IS_step_h_p_1.resize(n_hl+2);
    IS_do_h_p_1.resize(n_hl+2);
    IS_path_lih_p_1.resize(n_hl+2);
    D_step_h_p.push_back(n_hl);
    auto sitr = prev(D_step_h_p.end());
    IS_step_h_p_1[n_hl].push_back(sitr);
    D_do_h_p.push_back(n_hl);
    auto ditr = prev(D_do_h_p.end());
    IS_do_h_p_1[n_hl].push_back(ditr);
    n_hl++;
    // cap
    // cap(!F,!T,C) \:- int(C), uniq(!F,!T) | flow(!F,!T,0).
    while(!D_cap_hhi_p.empty()){
        auto tmp = D_cap_hhi_p.back();
        D_cap_hhi_p.pop_back();
        auto F = get<0>(tmp);
        auto T = get<1>(tmp);
        //rule
        if(history_rule1.insert({F,T}).second){
            D_flow_hhi_p.push_back({F,T,0});
        }
        //never reach other rules なのでサボる

        //push
        D_cap_hhi_al.push_back(tmp);
        auto itr = prev(D_cap_hhi_al.end());
        IS_cap_hhi_al_1[F].push_back(itr);
        IS_cap_hhi_al_2[T].push_back(itr);
    }

    //start
    while(!D_start_h_p.empty()){
        // rule never fire なのでサボる
        auto tmp = D_start_h_p.back();
        D_start_h_p.pop_back();
        D_start_h_al.push_back(tmp);
    }
    //goal
    while(!D_goal_h_p.empty()){
        // rule never fire なのでサボる
        auto tmp = D_goal_h_p.back();
        D_goal_h_p.pop_back();
        D_goal_h_al.push_back(tmp);
    }
    //順番
    //inc,step,do,path,flow,
    while(true){
        if(!D_inc_lih_p.empty()){
            auto tmp = D_inc_lih_p.front();
            D_inc_lih_p.pop_front();
            auto arg1 = get<0>(tmp);
            auto arg2 = get<1>(tmp);
            auto arg3 = get<2>(tmp);

            if(arg1.size() > 1){
                auto itr = arg1.begin();
                int B = *itr;
                ++itr;
                int A = *itr;
                bool flag = false;
                if(IS_flow_hhi_al_2[B].size() > 0)
                for(auto i = IS_flow_hhi_al_2[B].begin(); i != IS_flow_hhi_al_2[B].end(); ++i){
                    if(get<0>(**i)==A){
                        int M = arg2 + get<2>(**i);
                        arg1.pop_front();
                        D_inc_lih_p.push_back({arg1,arg2,arg3});
                        D_flow_hhi_p.push_back({A,B,M});

                        if(IS_flow_hhi_al_1[A].size() > 0)
                        for(auto j = IS_flow_hhi_al_1[A].begin(); j != IS_flow_hhi_al_1[A].end(); ++j){
                            if(*j == *i){
                                IS_flow_hhi_al_1[A].erase(j);
                                break;
                            }
                        }
                        D_flow_hhi_al.erase(*i);
                        IS_flow_hhi_al_2[B].erase(i);
                        flag = true;
                        break;
                    }
                }
                if(flag){
                    continue;
                }
                if(IS_flow_hhi_al_1[B].size() > 0)
                for(auto i = IS_flow_hhi_al_1[B].begin(); i != IS_flow_hhi_al_1[B].end(); ++i){
                    if(get<1>(**i)==A){
                        int M = get<2>(**i)-arg2;
                        arg1.pop_front();
                        D_inc_lih_p.push_back({arg1,arg2,arg3});
                        D_flow_hhi_p.push_back({B,A,M});
                        if(IS_flow_hhi_al_2[A].size() > 0)
                        for(auto j = IS_flow_hhi_al_2[A].begin(); j != IS_flow_hhi_al_2[A].end(); ++j){
                            if(*j == *i){
                                IS_flow_hhi_al_2[A].erase(j);
                                break;
                            }
                        }
                        D_flow_hhi_al.erase(*i);
                        IS_flow_hhi_al_1[B].erase(i);
                        flag = true;
                        break;
                    }
                }
                if(flag){
                    continue;
                }
            }
            if(arg1.size()==1){
                if(n_hl > IS_step_h_al_1.size()){
                    IS_step_h_al_1.resize(n_hl+1);
                }
                bool flag = false;
                if(IS_step_h_al_1[arg3].size() > 0)
                for(auto i = IS_step_h_al_1[arg3].begin(); i != IS_step_h_al_1[arg3].end();++i){
                    D_step_h_p.push_back(n_hl);
                    auto sitr = prev(D_step_h_p.end());
                    IS_step_h_p_1[n_hl].push_back(sitr);
                    D_do_h_p.push_back(n_hl);
                    auto ditr = prev(D_do_h_p.end());
                    IS_do_h_p_1[n_hl].push_back(ditr);
                    n_hl++;
                    IS_step_h_p_1.resize(n_hl+1);
                    IS_do_h_p_1.resize(n_hl+1);
                    IS_path_lih_p_1.resize(n_hl+1);
                    flag = true;
                    D_step_h_al.erase(*i);
                    IS_step_h_al_1[arg3].erase(i);
                    break;
                }
                if(flag){
                    continue;
                }
            }

            D_inc_lih_al.push_back({arg1,arg2,arg3});
            auto nitr = prev(D_inc_lih_al.end());
            IS_inc_lih_al_1[arg1.front()].push_back(nitr);
            IS_inc_lih_al_3[arg3].push_back(nitr);
            continue;
        }
        if(!D_step_h_p.empty()){
            auto tmp = D_step_h_p.front();
            D_step_h_p.pop_front();
            IS_step_h_p_1[tmp].pop_front();
            bool flag = false;
            if(IS_inc_lih_al_3.size() < n_hl){
                IS_inc_lih_al_3.resize(n_hl+1);
            }
            if(IS_inc_lih_al_3[tmp].size() > 0)
            for(auto i = IS_inc_lih_al_3[tmp].begin(); i != IS_inc_lih_al_3[tmp].end(); ++i){
                auto S = get<0>(**i);
                if(S.size() == 1){
                    for(auto j = IS_inc_lih_al_1[S.front()].begin(); j != IS_inc_lih_al_1[S.front()].end(); ++j){
                        if(*j == *i){
                            IS_inc_lih_al_1[S.front()].erase(j);
                            break;
                        }
                    }
                    D_inc_lih_al.erase(*i);
                    IS_inc_lih_al_3[tmp].erase(i);
                    flag = true;
                    D_step_h_p.push_back(n_hl);
                    auto sitr = prev(D_step_h_p.end());
                    IS_step_h_p_1[n_hl].push_back(sitr);
                    D_do_h_p.push_back(n_hl);
                    auto ditr = prev(D_do_h_p.end());
                    IS_do_h_p_1[n_hl].push_back(ditr);
                    n_hl++;
                    IS_step_h_p_1.resize(n_hl+1);
                    IS_do_h_p_1.resize(n_hl+1);
                    IS_path_lih_p_1.resize(n_hl+1);
                    break;
                }
            }
            if(flag){
                continue;
            }

            if(IS_do_h_al_1.size() < n_hl)IS_do_h_al_1.resize(n_hl+1);
            if(IS_path_lih_al_3.size() < n_hl){
                IS_path_lih_al_1.resize(n_hl+1);
                IS_path_lih_al_3.resize(n_hl+1);
            }
            if(IS_do_h_al_1[tmp].size() > 0)
            for(auto j = IS_do_h_al_1[tmp].begin(); j!=IS_do_h_al_1[tmp].end(); ){
                bool fin = true;
                if(IS_path_lih_al_3[tmp].size() > 0)
                for(auto i = IS_path_lih_al_3[tmp].begin(); i != IS_path_lih_al_3[tmp].end(); ++i){
                    auto L = get<0>(**i);
                    auto G = L.front();
                    auto G2 = D_goal_h_al.back();
                    if(G == G2){
                        auto F = get<1>(**i);
                        for(auto j = IS_path_lih_al_1[G].begin(); j != IS_path_lih_al_1[G].end(); ++j){
                            if(*j == *i){
                                IS_path_lih_al_1[G].erase(j);
                                break;
                            }
                        }
                        D_do_h_al.erase(*j);
                        j = IS_do_h_al_1[tmp].erase(j);
                        D_path_lih_al.erase(*i);
                        IS_path_lih_al_3[tmp].erase(i);
                        D_inc_lih_p.push_back({L,F,tmp});
                        fin = false;
                        break;
                    }
                }
                if(fin)break;
                ++j;
            }
            if(!IS_do_h_al_1[tmp].empty()){
                for(auto i = D_start_h_al.begin(); i != D_start_h_al.end(); ++i){
                    for(auto j = IS_cap_hhi_al_1[*i].begin(); j != IS_cap_hhi_al_1[*i].end(); ++j){
                        auto B = get<1>(**j);
                        for(auto k = IS_flow_hhi_al_1[*i].begin(); k != IS_flow_hhi_al_1[*i].end(); ++k){
                            if(get<1>(**k) == B){
                                auto M = get<2>(**j);
                                auto N = get<2>(**k);
                                auto R = M-N;
                                if(R > 0){
                                    if(history_rule2.insert({tmp,B}).second){
                                        int A = *i;
                                        list<int> nlist = {B,A};
                                        D_path_lih_p.push_back({nlist,R,tmp});
                                        auto pitr = prev(D_path_lih_p.end());
                                        IS_path_lih_p_1[tmp].push_back(pitr);
                                    }
                                }
                            }
                        }
                    }
                }
                for(auto i = IS_path_lih_al_3[tmp].begin(); i != IS_path_lih_al_3[tmp].end(); ++i){
                    auto L = get<0>(**i);
                    auto A = L.front();
                    for(auto j = IS_cap_hhi_al_2[A].begin(); j != IS_cap_hhi_al_2[A].end(); ++j){
                        auto B = get<0>(**j);
                        for(auto k = IS_flow_hhi_al_2[A].begin(); k != IS_flow_hhi_al_2[A].end(); ++k){
                            if(get<0>(**k) == B){
                                auto N = get<2>(**k);
                                if(N > 0){
                                    if(history_rule4.insert({tmp,A,B}).second){
                                        int F = get<1>(**i);
                                        int min = (F<N)?F:N;
                                        auto nlist = L;
                                        nlist.push_front(A);
                                        D_path_lih_p.push_back({nlist,min,tmp});
                                        auto pitr = prev(D_path_lih_p.end());
                                        IS_path_lih_p_1[tmp].push_back(pitr);
                                    }
                                }
                            }
                        }
                    }
                    for(auto j = IS_cap_hhi_al_1[A].begin(); j != IS_cap_hhi_al_1[A].end(); ++j){
                        auto B = get<1>(**j);
                        for(auto k = IS_flow_hhi_al_1[A].begin(); k != IS_flow_hhi_al_1[A].end(); ++k){
                            if(get<1>(**k) == B){
                                auto M = get<2>(**j);
                                auto N = get<2>(**k);
                                auto R = M-N;
                                if(R > 0){
                                    if(history_rule3.insert({tmp,A,B}).second){
                                        int F = get<1>(**i);
                                        int min = (F<R)?F:R;
                                        auto nlist = L;
                                        nlist.push_front(B);
                                        D_path_lih_p.push_back({nlist,min,tmp});
                                        auto pitr = prev(D_path_lih_p.end());
                                        IS_path_lih_p_1[tmp].push_back(pitr);
                                    }
                                }
                            }
                        }
                    }
                    
                }
            }

            if(IS_step_h_al_1.size() < n_hl){
                IS_step_h_al_1.resize(n_hl+1);
            }
            D_step_h_al.push_back({tmp});
            auto nitr = prev(D_step_h_al.end());
            IS_step_h_al_1[tmp].push_back(nitr);
            continue;
        }
        if(!D_do_h_p.empty()){
            auto tmp = D_do_h_p.front();
            D_do_h_p.pop_front();
            IS_do_h_p_1[tmp].pop_front();
            if(IS_step_h_al_1.size() < n_hl)IS_step_h_al_1.resize(n_hl+1);
            if(IS_path_lih_al_3.size() < n_hl){
                IS_path_lih_al_1.resize(n_hl+1);
                IS_path_lih_al_3.resize(n_hl+1);
            }
            if(!IS_step_h_al_1[tmp].empty() || !IS_step_h_p_1[tmp].empty()){
                bool flag = false;
                if(IS_path_lih_al_3[tmp].size() > 0)
                for(auto i = IS_path_lih_al_3[tmp].begin(); i != IS_path_lih_al_3[tmp].end(); ++i){
                    auto L = get<0>(**i);
                    auto G = L.front();
                    auto G2 = D_goal_h_al.back();
                    if(G == G2){
                        auto F = get<1>(**i);
                        for(auto j = IS_path_lih_al_1[G].begin(); j != IS_path_lih_al_1[G].end(); ++j){
                            if(*j == *i){
                                IS_path_lih_al_1[G].erase(j);
                                break;
                            }
                        }
                        D_path_lih_al.erase(*i);
                        IS_path_lih_al_3[tmp].erase(i);
                        D_inc_lih_p.push_back({L,F,tmp});
                        flag = true;
                        break;
                    }
                }
                if(flag)continue;
                if(IS_path_lih_p_1[tmp].size() > 0)
                for(auto i = IS_path_lih_p_1[tmp].begin(); i != IS_path_lih_p_1[tmp].end(); ++i){
                    auto L = get<0>(**i);
                    auto G = L.front();
                    auto G2 = D_goal_h_al.back();
                    if(G == G2){
                        auto F = get<1>(**i);
                        D_path_lih_al.erase(*i);
                        IS_path_lih_al_3[tmp].erase(i);
                        D_inc_lih_p.push_back({L,F,tmp});
                        flag = true;
                        break;
                    }
                }
                if(flag)continue;
            }
            if(!IS_step_h_al_1[tmp].empty()){
                for(auto i = D_start_h_al.begin(); i != D_start_h_al.end(); ++i){
                    for(auto j = IS_cap_hhi_al_1[*i].begin(); j != IS_cap_hhi_al_1[*i].end(); ++j){
                        auto A = get<0>(**j);
                        auto B = get<1>(**j);
                        for(auto k = IS_flow_hhi_al_1[*i].begin(); k != IS_flow_hhi_al_1[*i].end(); ++k){
                            if(get<1>(**k) == B){
                                auto M = get<2>(**j);
                                auto N = get<2>(**k);
                                auto R = M-N;
                                if(R > 0){
                                    if(history_rule2.insert({tmp,B}).second){
                                        list<int> nlist = {B,A};
                                        D_path_lih_p.push_back({nlist,R,tmp});
                                        auto pitr = prev(D_path_lih_p.end());
                                        IS_path_lih_p_1[tmp].push_back(pitr);
                                    }
                                }
                            }
                        }
                    }
                }
                for(auto i = IS_path_lih_al_3[tmp].begin(); i != IS_path_lih_al_3[tmp].end(); ++i){
                    auto L = get<0>(**i);
                    auto A = L.front();
                    for(auto j = IS_cap_hhi_al_2[A].begin(); j != IS_cap_hhi_al_2[A].end(); ++j){
                        auto B = get<0>(**j);
                        for(auto k = IS_flow_hhi_al_2[A].begin(); k != IS_flow_hhi_al_2[A].end(); ++k){
                            if(get<0>(**k) == B){
                                auto N = get<2>(**k);
                                if(N > 0){
                                    if(history_rule4.insert({tmp,A,B}).second){
                                        int F = get<1>(**i);
                                        int min = (F<N)?F:N;
                                        auto nlist = L;
                                        nlist.push_front(A);
                                        D_path_lih_p.push_back({nlist,min,tmp});
                                        auto pitr = prev(D_path_lih_p.end());
                                        IS_path_lih_p_1[tmp].push_back(pitr);
                                    }
                                }
                            }
                        }
                    }
                    for(auto j = IS_cap_hhi_al_1[A].begin(); j != IS_cap_hhi_al_1[A].end(); ++j){
                        auto B = get<1>(**j);
                        for(auto k = IS_flow_hhi_al_1[A].begin(); k != IS_flow_hhi_al_1[A].end(); ++k){
                            if(get<1>(**k) == B){
                                auto M = get<2>(**j);
                                auto N = get<2>(**k);
                                auto R = M-N;
                                if(R > 0){
                                    if(history_rule3.insert({tmp,A,B}).second){
                                        int F = get<1>(**i);
                                        int min = (F<R)?F:R;
                                        auto nlist = L;
                                        nlist.push_front(B);
                                        D_path_lih_p.push_back({nlist,min,tmp});
                                        auto pitr = prev(D_path_lih_p.end());
                                        IS_path_lih_p_1[tmp].push_back(pitr);
                                    }
                                }
                            }
                        }
                    }
                    
                }
            }
            if(IS_do_h_al_1.size() < n_hl){
                IS_do_h_al_1.resize(n_hl+1);
            }
            D_do_h_al.push_back(tmp);
            auto nitr = prev(D_do_h_al.end());
            IS_do_h_al_1[tmp].push_back(nitr);
            continue;
        }
        if(!D_path_lih_p.empty()){
            auto tmp = D_path_lih_p.front();
            D_path_lih_p.pop_front();
            auto arg1 = get<0>(tmp);
            auto  H = arg1.front(); 
            auto arg2 = get<1>(tmp);
            auto arg3 = get<2>(tmp);
            IS_path_lih_p_1[arg3].pop_front();
            if((!IS_step_h_al_1[arg3].empty()) || (!IS_step_h_p_1[arg3].empty())){
                if(D_goal_h_al.front() == H ){
                    if(!IS_do_h_al_1[arg3].empty()){
                        auto i = IS_do_h_al_1[arg3].front();
                        IS_do_h_al_1[arg3].pop_front();
                        D_do_h_al.erase(i);
                        D_inc_lih_p.push_back(tmp);
                        continue;
                    }
                    else if(!IS_do_h_p_1[arg3].empty()){
                        auto i = IS_do_h_p_1[arg3].front();
                        IS_do_h_p_1[arg3].pop_front();
                        D_do_h_p.erase(i);
                        D_inc_lih_p.push_back(tmp);
                        continue;
                    }
                }
            }
            if(!IS_step_h_al_1[arg3].empty()){
                if(!IS_do_h_al_1[arg3].empty()){
                    if(IS_cap_hhi_al_2[H].size() > 0)
                    for(auto i = IS_cap_hhi_al_2[H].begin(); i != IS_cap_hhi_al_2[H].end(); ++i){
                        auto B = get<0>(**i);
                        if(IS_flow_hhi_al_2[H].size() > 0)
                        for(auto j = IS_flow_hhi_al_2[H].begin(); j != IS_flow_hhi_al_2[H].end(); ++j){
                            if(get<0>(**j) == B){
                                int N = get<2>(**j);
                                if(N > 0){
                                    if(history_rule4.insert({arg3,H,B}).second){
                                        auto nlist = arg1;
                                        nlist.push_front(B);
                                        int min = (arg2<N)?arg2:N;
                                        D_path_lih_p.push_back({nlist,min,arg3});
                                        auto pitr = prev(D_path_lih_p.end());
                                        IS_path_lih_p_1[arg3].push_back(pitr);
                                    }
                                }
                            }
                        }
                    }   
                    if(IS_cap_hhi_al_1[H].size() > 0)
                    for(auto i = IS_cap_hhi_al_1[H].begin(); i != IS_cap_hhi_al_1[H].end(); ++i){
                        auto B = get<1>(**i);
                        if(IS_flow_hhi_al_1[H].size() > 0)
                        for(auto j = IS_flow_hhi_al_1[H].begin(); j != IS_flow_hhi_al_1[H].end(); ++j){
                            if(get<1>(**j) == B){
                                int R = get<2>(**i) - get<2>(**j);
                                if(R > 0){
                                    if(history_rule3.insert({arg3,H,B}).second){
                                        auto nlist = arg1;
                                        nlist.push_front(B);
                                        int min = (arg2<R)?arg2:R;
                                        D_path_lih_p.push_back({nlist,min,arg3});
                                        auto pitr = prev(D_path_lih_p.end());
                                        IS_path_lih_p_1[arg3].push_back(pitr);
                                    }
                                }
                            }
                        }
                    }   
                }
            }

            if(IS_path_lih_al_1.size() < n_hl){
                IS_path_lih_al_1.resize(n_hl+1);
                IS_path_lih_al_3.resize(n_hl+1);
            }
            D_path_lih_al.push_back(tmp);
            auto nitr = prev(D_path_lih_al.end());
            IS_path_lih_al_1[H].push_back(nitr);
            IS_path_lih_al_3[arg3].push_back(nitr);
            continue;
        }
        if(!D_flow_hhi_p.empty()){
            auto tmp = D_flow_hhi_p.front();
            D_flow_hhi_p.pop_front();
            auto A = get<0>(tmp);
            auto B = get<1>(tmp);
            auto N = get<2>(tmp);
            bool flag = false;
            if(!IS_inc_lih_al_1[B].empty())
            for(auto i = IS_inc_lih_al_1[B].begin(); i != IS_inc_lih_al_1[B].end(); ++i){
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
                        L.pop_front();
                        D_inc_lih_p.push_back({L,F,ST});
                        D_flow_hhi_p.push_back({A,B,M});
                        
                        for(auto j = IS_inc_lih_al_3[ST].begin(); j != IS_inc_lih_al_3[ST].end(); ++j){
                            if(*j == *i){
                                IS_inc_lih_al_3[ST].erase(j);
                                break;
                            }
                        }
                        D_inc_lih_al.erase(*i);
                        IS_inc_lih_al_1[B].erase(i);
                        flag = true;
                        break;
                    }
                }

            }
            if(flag) continue;
            if(!IS_inc_lih_al_1[A].empty())
            for(auto i = IS_inc_lih_al_1[A].begin(); i != IS_inc_lih_al_1[A].end(); ++i){
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
                        D_inc_lih_p.push_back({L,F,ST});
                        D_flow_hhi_p.push_back({A,B,M});
                        for(auto j = IS_inc_lih_al_3[ST].begin(); j != IS_inc_lih_al_3[ST].end(); ++j){
                            if(*j == *i){
                                IS_inc_lih_al_3[ST].erase(j);
                                break;
                            }
                        }
                        D_inc_lih_al.erase(*i);
                        IS_inc_lih_al_1[A].erase(i);
                        flag = true;
                        break;
                    }
                }

            }
            if(flag) continue;

            auto sA = D_start_h_al.front();
            for(auto s = D_step_h_al.begin(); s != D_step_h_al.end(); ++s){
                auto ST = *s;
                if(IS_do_h_al_1[ST].size() > 0){
                    if(IS_cap_hhi_al_2[B].size() > 0)
                    for(auto i = IS_cap_hhi_al_2[B].begin(); i != IS_cap_hhi_al_2[B].end(); ++i){
                        if(get<0>(**i) == A){
                            if(IS_path_lih_al_1[B].size() > 0)
                            for(auto j = IS_path_lih_al_1[B].begin(); j != IS_path_lih_al_1[B].end(); ++j){
                                if(ST == get<2>(**j)){
                                    if(N>0){
                                        if(history_rule4.insert({ST,B,A}).second){
                                            auto L = get<0>(**j);
                                            int F = get<1>(**j);
                                            L.push_front(A);
                                            int min = (F<N)?F:N;
                                            D_path_lih_p.push_back({L,min,ST});
                                            auto pitr = prev(D_path_lih_p.end());
                                            IS_path_lih_p_1[ST].push_back(pitr);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if(IS_cap_hhi_al_1[A].size() > 0)
                    for(auto i = IS_cap_hhi_al_1[A].begin(); i != IS_cap_hhi_al_1[A].end(); ++i){
                        if(sA == A){
                            if(get<1>(**i) == B){
                                int M = get<2>(**i);
                                int R = M-N;
                                if(R>0){
                                    if(history_rule2.insert({ST,B}).second){
                                        list<int> nlist = {B,A};
                                        D_path_lih_p.push_back({nlist,R,ST});
                                        auto pitr = prev(D_path_lih_p.end());
                                        IS_path_lih_p_1[ST].push_back(pitr);
                                    }
                                } 
                            }
                        }
                        if(get<1>(**i) == B){
                            if(IS_path_lih_al_1[A].size() > 0)
                            for(auto j = IS_path_lih_al_1[A].begin(); j != IS_path_lih_al_1[A].end(); ++j){
                                if(ST == get<2>(**j)){
                                    int M = get<2>(**i);
                                    int R = M-N;
                                    if(R>0){
                                        if(history_rule3.insert({ST,A,B}).second){
                                            auto L = get<0>(**j);
                                            int F = get<1>(**j);
                                            L.push_front(B);
                                            int min = (F<R)?F:R;
                                            D_path_lih_p.push_back({L,min,ST});
                                            auto pitr = prev(D_path_lih_p.end());
                                            IS_path_lih_p_1[ST].push_back(pitr);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;    
                }
            }
            D_flow_hhi_al.push_back(tmp);
            auto nitr = prev(D_flow_hhi_al.end());
            IS_flow_hhi_al_1[A].push_back(nitr);
            IS_flow_hhi_al_2[B].push_back(nitr);
            continue;
        }
        break;
    }

    string name = "flow";
    print_i3(D_flow_hhi_al,name);
    name = "cap";
    print_i3(D_cap_hhi_al, name);
    cout << "@4." << endl;

    return 0;
}