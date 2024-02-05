#include<iostream>
#include<string>
using namespace std;


void makegraph(int n){
    cout << "start(!H), distance(1,!H,!H0,5).";
    for (int i = 1; i < n; i++)
    {
        cout << "distance(1,!H0,!H" << i << ",5).";
        cout << "distance(1,!H" << i <<",!H" << n << ",5).";
    }
    cout << endl;
}

void makegraphc(int n){
    // start_1 atom の数
    cout << 1 << endl;
    // distance_4 atom の数 (edge の数) 
    cout << 2*n << endl;
    // hl id の数 (node の数)
    cout << n+1 << endl;
    //start !H0
    cout << "0" << endl;

    for (int i = 1; i < n; i++)
    {
        cout << "1 0 " << i << " 5" << endl;
        cout << "1 " << i << " " << n << " 5" << endl;
    }
    cout << endl;
}

int main(int argc, char *argv[]){
    int num = 10 ;
    string in;
    if(argc >= 2){
        in = argv[1];
        num = stoi(in);
    }
    makegraph(num);
}