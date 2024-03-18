#include <iostream>
#include <unordered_set>
#include <string>
#include <iterator>
#include <fstream>
#include <list>
#include <algorithm>
#include <vector>

using namespace std;

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
    string filename = "input.txt";
    if(argc >= 2){
        filename = argv[1];
    }

    ifstream i_file(filename);
    if(!i_file.is_open()){
        cerr << "Could not open input file - " << filename << endl;
        return 1;
    }
    int n_start, n_distance, n_between, n_max, n_min;
    int n_hl;
    i_file >> n_start >> n_distance; 
    i_file >> n_hl;

    typedef int used; 
    
    list<int> D_start_h_p;
    list<tuple<int,int,int,int>> D_distance_ihhi_p;
    list<tuple<int,int,int,int,used,used>> D_distance_ihhi_al;
    list<list<tuple<int,int,int,int,used,used>>::iterator> IS_distance_ihhi_al_2[n_hl];
    list<list<tuple<int,int,int,int,used,used>>::iterator> IS_distance_ihhi_al_3[n_hl];
    list<list<tuple<int,int,int,used>>::iterator> IS_between_ihi_al[n_hl];
    list<tuple<int,int,int>> D_between_ihi_p;
    list<list<tuple<int,int,int>>::iterator> IS_between_ihi_p[n_hl];
    list<tuple<int,int,int,used>> D_between_ihi_al;

    int link1, link2, link3, link4;
    string tmp1,tmp2;
    for(size_t i = 0; i < n_start; i++){
        i_file >> link1;
        D_start_h_p.push_back(link1);
    }
    for (size_t i = 0; i < n_distance; i++)
    {
        i_file >> link1 >> link2 >> link3 >> link4;
        D_distance_ihhi_p.push_back({link1,link2,link3,link4});
    }

    using hist_type = std::tuple<int,int,int,int,int,int>;
    const auto hash = [](const hist_type &key){
        return (get<0>(key)*4027+get<1>(key)*4049+get<2>(key)*4057
                +get<3>(key)*4073+get<4>(key)*4073+get<5>(key)*4079);
    };

    std::unordered_set<hist_type, decltype(hash)> history_rule3(1024,hash);
    std::unordered_set<hist_type, decltype(hash)> history_rule4(1024,hash); 
    
    // start_h
    while(!D_start_h_p.empty()){
        int tmp = D_start_h_p.back();
        D_start_h_p.pop_back();
        D_between_ihi_p.push_back({0,tmp,0});
        auto titr = prev(D_between_ihi_p.end());
        IS_between_ihi_p[tmp].push_back(titr);
    }

    // distance_ihhi
    while(!D_distance_ihhi_p.empty()){
        tuple<int,int,int,int> tmp = D_distance_ihhi_p.front();
        D_distance_ihhi_p.pop_front();
        int arg1 = get<0>(tmp);
        int arg2 = get<1>(tmp);
        int arg3 = get<2>(tmp);
        int arg4 = get<3>(tmp);
        
        for(auto i: IS_between_ihi_al[arg2]){
            if(history_rule3.insert({get<0>(*i),arg2,get<2>(*i),arg1,arg3,arg4}).second){
                int AC = get<0>(*i) + arg1;
                int BD = get<2>(*i) + arg4; 
                D_between_ihi_p.push_back({AC,arg3,BD});
                auto titr = prev(D_between_ihi_p.end());
                cout << "arg3: " << arg3 << endl; 
                IS_between_ihi_p[arg3].push_back(titr);
            }   
            if(history_rule4.insert({get<0>(*i),arg2,get<2>(*i),arg1,arg3,arg4}).second){
                int AC = get<0>(*i) - arg4;
                int BD = get<2>(*i) - arg1; 
                D_between_ihi_p.push_back({AC,arg3,BD});
                auto titr = prev(D_between_ihi_p.end());
                IS_between_ihi_p[arg3].push_back(titr);
            }
        }
        D_distance_ihhi_al.push_back({arg1,arg2,arg3,arg4,IS_distance_ihhi_al_2[arg2].size(),IS_distance_ihhi_al_3[arg3].size()});
        auto itr = D_distance_ihhi_al.end();
        --itr;
        IS_distance_ihhi_al_2[arg2].push_back({itr});
        IS_distance_ihhi_al_3[arg3].push_back({itr});
    }
    
    // between_ihi
    while (!(D_between_ihi_p.empty())){
        tuple<int,int,int> tmp = D_between_ihi_p.front();
        int arg1 = get<0>(tmp);
        int arg2 = get<1>(tmp);
        int arg3 = get<2>(tmp);
        D_between_ihi_p.pop_front();
        IS_between_ihi_p[arg2].pop_front();

        if(arg1 > arg3)continue;

        if(IS_between_ihi_p[arg2].size() > 0){
            auto itr = IS_between_ihi_p[arg2].begin();
            int max = get<0>(**itr)>arg1?get<0>(**itr):arg1;
            int min = get<2>(**itr)<arg3?get<2>(**itr):arg3;
            D_between_ihi_p.erase(*itr);
            IS_between_ihi_p[arg2].erase(itr);
            if(!(max==arg1 && min==arg3)){
                D_between_ihi_p.push_back({max,arg2,min});
                auto nitr = prev(D_between_ihi_p.end());
                IS_between_ihi_p[arg2].push_back(nitr);
                continue;
            }
        }
        if(!IS_between_ihi_al[arg2].empty()){           
            int max,min;
            auto tmp2 = IS_between_ihi_al[arg2].begin();
            int arg21 = get<0>(*tmp2);
            int arg22 = get<1>(*tmp2);
            int arg23 = get<2>(*tmp2);
            int arg24 = get<3>(*tmp2);

            max=arg1>arg21?arg1:arg21;
            min=arg3>arg23?arg23:arg3;

            if(arg21 != max || arg23 != min){
            D_between_ihi_p.push_back({max,arg2,min});
            auto titr = prev(D_between_ihi_p.end());
            IS_between_ihi_p[arg2].push_back(titr);
            IS_between_ihi_al[arg2].pop_back();
            D_between_ihi_al.erase(tmp2);
            }

            continue;
        }

        for(auto i: IS_distance_ihhi_al_2[arg2]){
            if(history_rule3.insert({arg1,arg2,arg3,get<0>(*i),get<2>(*i),get<3>(*i)}).second){
                int AC = arg1 + get<0>(*i);
                int BD = arg3 + get<3>(*i);
                D_between_ihi_p.push_back({AC,get<2>(*i),BD});
                auto titr = prev(D_between_ihi_p.end());
                IS_between_ihi_p[get<2>(*i)].push_back(titr);
            }  
        }
        for(auto i: IS_distance_ihhi_al_3[arg2]){
            if(history_rule4.insert({arg1,arg2,arg3,get<0>(*i),get<1>(*i),get<3>(*i)}).second){
                int AC = arg1 - get<3>(*i);
                int BD = arg3 - get<0>(*i);            
                D_between_ihi_p.push_back({AC,get<1>(*i),BD});
                auto titr = prev(D_between_ihi_p.end());
                IS_between_ihi_p[get<1>(*i)].push_back(titr);
            } 
        }
        D_between_ihi_al.push_back({arg1,arg2,arg3,IS_between_ihi_al[arg2].size()});
        auto itr = D_between_ihi_al.end();
        --itr;
        IS_between_ihi_al[arg2].push_back({itr});
    }

    print_distance(D_distance_ihhi_al);
    print_between(D_between_ihi_al);

    return 0;
}