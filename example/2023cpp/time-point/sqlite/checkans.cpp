#include <iostream>
#include <unordered_set>
#include <set>
#include <string>
#include <iterator>
#include <fstream>
#include <chrono>
#include <list>
#include <algorithm>
#include <vector>
#include <sstream>

using namespace std;

void edge(){
    string file1 = "check1.txt";
    string file2 = "check2.txt";

    ifstream if1(file1);
    if(!if1.is_open()){
        cerr << "Could not open input file - " << file1 << endl;
        return ;
    }

    ifstream if2(file2);
    if(!if2.is_open()){
        cerr << "Could not open input file - " << file2 << endl;
        return ;
    }
    char c;
    string tmp,in;
    multiset<string> mset,mset2;
    cout << "read file1..." << endl;
    while(getline(if1, tmp)){
        istringstream st(tmp);
        while(getline(st, in, '.')){
            // cout << in << endl;
            mset.insert(in);
        }
    }
    cout << "\nread file2..." << endl;
    while(getline(if2, tmp)){
        istringstream st(tmp);
        while(getline(st, in, '.')){
            // cout << in << endl;
            mset2.insert(in);
        }
    }
    for(auto it = mset2.begin(); it != mset2.end();){
        cout << "\n*iter = " << *it << " : ";
        cout << "\tf1 = " << mset.count(*it) << " f2 = " << mset2.count(*it) << endl;
        if(mset2.count(*it) == mset.count(*it)){
            mset.erase(mset.find(*it));
            it = mset2.erase(it);
        }else{
            cout << "different content found: " << *it << endl;
            return;
        }
    }
    if(mset.empty() && mset2.empty()){
       cout << "succeeded" << endl; 
    }
    else{
        cout << "failed" << endl;
    }
    return ;
}

int main(){

    edge();

    return 0;
}