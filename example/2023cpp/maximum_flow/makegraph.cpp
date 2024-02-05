#include<stdio.h>
#include<cmath>
#include<random>
#include<vector>
#include<array>
#include<iostream>
#include<list>
#include<algorithm>
#include<tuple>
#include<unordered_set>
using namespace std;

void uniform_spath(int num){
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    //一様分布
    std::uniform_real_distribution<float> dist(0, num);
    printf("edge(%d,%d)",(int)dist(engine),(int)dist(engine));
    for (int i = 1; i < num; i++)
    {
            printf(", edge(%d,%d,%d)\n",(int)dist(engine),(int)dist(engine),(int)dist(engine));
    }   
}


void both_uniform(int n, int m, int U){
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::random_device seed_gen2;
    std::mt19937 engine2(seed_gen2());
    //一様分布
    std::uniform_int_distribution<int> t(1, U);
    

    using ht = std::tuple<int,int>;
    const auto hash = [](const ht &key){
        return (get<0>(key)*4027+get<1>(key));
    };

    unordered_set<ht, decltype(hash)> edge(1024,hash);
    vector<tuple<int,int,int>> cap;

    // 2
    for(int i = 0; i < n-1; i++){
        uniform_int_distribution<> tmp(i+1, n-1);
        int dst = tmp(engine);
        int c = t(engine);
        if(edge.insert({i,dst}).second){
            cap.push_back({i,dst,c});
        }
    }

    // 3
    for(int i = 1; i < n; i++){
        uniform_int_distribution<> tmp(0,i-1);
        int src = tmp(engine);
        if(edge.insert({src,i}).second){
            int c = t(engine);
            cap.push_back({src,i,c});
        }
    }

    // 4 
    while(cap.size() < m){
        uniform_int_distribution<> ts(0,n-2);
        int src = ts(engine);
        uniform_int_distribution<> td(src+1,n-1);
        int dst = td(engine);
        if(edge.insert({src,dst}).second){
            int c = t(engine);
            cap.push_back({src,dst,c});
        }
    }

    // c++ 改行無し。読みやすくするなら改行を加える
    // number of jobs
    cout << n << " ";
    // atom n(job.size)
    cout << m << " ";
    // start = 0 goal = n-1
    cout << "0" << " " << n-1 << " ";
    // cap
    for(int i = 0; i < cap.size(); i++){
        auto tmp = cap[i];
        cout << get<0>(tmp) << " " << get<1>(tmp) << " " << get<2>(tmp) << " ";
    }
    cout << endl;

    // lmntal
    cout << "start(!H0).goal(!H" << n-1 << ").";
    for(int i = 0; i < cap.size(); i++){
        auto tmp = cap[i];
        cout << "cap(!H" << get<0>(tmp) << ",!H" << get<1>(tmp) << "," << get<2>(tmp) << ").";
    }
    cout << endl;
}

int main(int argc, char *argv[]){

    int n = 10;
    int U = 50; 
    string in;
    if(argc >= 2){
        in = argv[1];
        n = stoi(in);
    }
    both_uniform(n, 2*n, U);
    return 0;
}