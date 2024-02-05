#include<stdio.h>
#include<cmath>
#include<random>
#include<vector>
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


void both_uniform(int num, int time_max, int cost_max){
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::random_device seed_gen2;
    std::mt19937 engine2(seed_gen2());
    //一様分布
    std::uniform_real_distribution<float> t(0, time_max);
    std::uniform_real_distribution<float> c(0, cost_max);
    
    std::vector<pair<int,int>> job;

    for(int i = 0; i < num; i++){
        pair<int,int> tmp;
        tmp.first = (int)t(engine);
        tmp.second = (int)c(engine2);
        job.push_back(tmp);
    }

    // c++ 改行無し。読みやすくするなら改行を加える
    // number of jobs
    cout << job.size() << " ";
    // atom n(job.size)
    cout << job.size() << " ";
    // jobs
    for(int i = 0; i < job.size(); i++){
        pair<int,int> tmp = job[i];
        cout << tmp.first << " " << tmp.second << " " << i << " ";
    }
    cout << endl;

    // lmntal
    cout << "n(" << job.size() << ").";
    for(int i = 0; i < job.size(); i++){
        pair<int,int> tmp = job[i];
        cout << "job(" << tmp.first << "," << tmp.second << ",!J" << i << ").";
    }
    cout << endl;
}

void both_lognormal(int num, float average, float dist){
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::random_device seed_gen2;
    std::mt19937 engine2(seed_gen2());
    //対数正規分布
    std::lognormal_distribution<float> t(average, dist);
    
    std::vector<pair<int,int>> job;

    for(int i = 0; i < num; i++){
        pair<int,int> tmp;
        tmp.first = (int)t(engine);
        tmp.second = (int)t(engine2);
        job.push_back(tmp);
    }

    // c++ 改行無し。読みやすくするなら改行を加える
    // number of jobs
    cout << job.size() << " ";
    // atom n(job.size)
    cout << job.size() << " ";
    // jobs
    for(int i = 0; i < job.size(); i++){
        pair<int,int> tmp = job[i];
        cout << tmp.first+1 << " " << tmp.second+1 << " " << i << " ";
    }
    cout << endl;

    // lmntal
    cout << "n(" << job.size() << ").";
    for(int i = 0; i < job.size(); i++){
        pair<int,int> tmp = job[i];
        cout << "job(" << tmp.first+1 << "," << tmp.second+1 << ",!J" << i << ").";
    }
    cout << endl;

    // lmntal nd 
    for(int i = 0; i < job.size(); i++){
        pair<int,int> tmp = job[i];
        cout << "job(" << tmp.first+1 << "," << tmp.second+1 << "," << i << ").";
    }
}


int main(int argc, char *argv[]){

    int num = 10 ;
    int time_max = 50; 
    int cost_max = 50;
    float average = 1.2;
    float dist = 0.8;
    string in;
    if(argc >= 2){
        in = argv[1];
        num = stoi(in);
    }
    // both_uniform(num, time_max, cost_max);
    both_lognormal(num,average,dist);
    return 0;
}