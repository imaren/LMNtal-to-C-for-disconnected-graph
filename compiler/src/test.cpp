#include <bits/stdc++.h>
using namespace std;


int main(){
    vector<string> a,b;

    string tmp = "good";
    a.push_back(tmp);
    
    for(auto itr = a.begin(); itr != b.end(); ++itr){
        if(itr == a.end()){
            itr = b.begin();
            if(itr == b.end())break;
            continue;
        }
        cout << *itr << endl;
    }

}