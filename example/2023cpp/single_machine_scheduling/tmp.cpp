#include <iostream>
#include <unordered_set>
#include <string>
#include <iterator>
#include <fstream>
#include <chrono>
#include <list>
#include <algorithm>
#include <vector>

using namespace std;

// int main(void){
//     list<int> lst;
//     int n = 0;

//     // lst.push_back(n);
//     // cout << n << endl;

//     // for(auto i : lst){
//     //     cout << i << endl;
//     // }

//     for(auto i = 0; i < n; i++){
//         cout << i << endl;
//     }
// }


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
    vector<list<list<tuple<int,int,int,list<int>,int,used>>::iterator>> vec_s;
    list<tuple<int,int,int,list<int>,int>> p_s;
    list<tuple<int,int,int,list<int>,int,used>> al_s;
    bool flag = true;

    int link1, link2, link3, link4;
    string tmp1,tmp2;
cout <<"test";
    for (size_t i = 0; i < n_job; i++)
    {
        i_file >> link1 >> link2 >> link3;
        p_job.push_back({link1,link2,link3});
        cout << link1 << "," << link2 << "," << link3 << endl;
    }
p_n.push_back(n);
}