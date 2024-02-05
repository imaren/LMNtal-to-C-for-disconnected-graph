#include<stdio.h>
#include<cmath>
#include<random>
#include<vector>
#include<iostream>
#include<list>
#include<set>
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

void uniform_cpp_spath(int num){
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    //一様分布
    std::uniform_real_distribution<float> dist(0, num);
    printf("%d\n",num+2);
    for (int i = 0; i < num; i++)
    {
            printf("%d %d %d\n",(int)dist(engine),(int)dist(engine),(int)dist(engine));
    }   
}

void make_edge(int num_E, int num_V){

    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    //一様分布
    set<pair<int,int>> edge;
    int id=1;
    int from;
    int to;
    int E = num_E;

    while(true){
        if(num_E <= 0){
            break;
        }

        std::uniform_real_distribution<float> rnd(0, num_V-1);
        from = rnd(engine);
        to = rnd(engine);
        if(from != to)
        if(edge.insert({from,to}).second){
            num_E--;
        }
    }
    //lmntal
    for (auto i : edge)
    {
        cout << "path(!H" << i.first << ",!H" << i.second << ").";
    }
    //cpp
    cout << endl <<  num_V << " " << E << " ";
    for (auto i : edge)
    {
        cout << i.first << " " << i.second << " ";
    }
    return ;
}




int main(int argc, char *argv[]){

    int num = 10 ;
    string in;
    if(argc >= 2){
        in = argv[1];
        num = stoi(in);
    }
    make_edge(2*num, num);

    return 0;
}