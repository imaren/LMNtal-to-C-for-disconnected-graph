#include<stdio.h>
#include<cmath>
#include<random>
#include<vector>
#include<iostream>
#include<list>
#include<set>
using namespace std;

int main(int argc, char *argv[]){
    int tmp;
    int E,V;
    cin >>tmp;
    cin >> E >> V;
    cin>>tmp;
    for(int i = 0; i < E; i++){
        int t1,t2,t3,t4;
        cin >> t1 >> t2 >> t3 >> t4;
        cout << "distance(" << t1 << "," << t2 << "," << t3 << "," << t4 << ").";
    }

    return 0;
}