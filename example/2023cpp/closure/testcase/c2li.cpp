#include<stdio.h>
#include<cmath>
#include<random>
#include<vector>
#include<iostream>
#include<list>
#include<set>
using namespace std;

int main(int argc, char *argv[]){

    int E,V;

    cin >> V >> E;

    for(int i = 0; i < E; i++){
        int t1,t2;
        cin >> t1 >> t2;
        cout << "path(" << t1 << "," << t2 << ").";
    }

    return 0;
}