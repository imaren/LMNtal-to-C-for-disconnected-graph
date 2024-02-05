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
#include <regex>
#include <sstream>

using namespace std;

void flow(){
    //lmntal
    string file1 = "check1.txt";
    //c++
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
    string tmp,in,arg;
    string goal1 = "";
    string goal2 = "";
    multiset<string> mset,mset2;
    vector<pair<string,int>> flow1, flow2;
    string lmnmax = "";

    cout << "read file1..." << endl;
    while(getline(if1, tmp)){
        istringstream st(tmp);
        while(getline(st, in, '.')){
            istringstream args(in);
            string tp = "";
            int cnt = 0;
            if(in.find("goal") != string::npos){
                int len = in.length();
                goal1 = in.substr(6,len-2);
            }else if(in.find("aflow") != string::npos){
                int f = 0;
                while(getline(args, arg, ',')){
                    if(cnt < 2){
                        tp += arg+",";
                    }
                    else{
                        std::smatch matches;
                        regex_search(arg, matches, regex("\\d+"));
                        f = stoi(matches.str());
                    }
                    cnt++;
                } 
                // cout << tp << f << endl;
                flow1.push_back({tp,f});
            }else if(in.find("maxflow") != string::npos){
                lmnmax = in;
            }
        }
    }
    goal1 = goal1.substr(0,goal1.length()-1);
    cout << "goal1 : " << goal1 << endl;
    cout << "\nread file2..." << endl;
    string cmax ="";
    while(getline(if2, tmp)){
        istringstream st(tmp);
        while(getline(st, in, '.')){
            istringstream args(in);
            string tp = "";
            int cnt = 0;
            if(in.find("goal") != string::npos){
                goal2 = in.substr(5,in.length()-4);
            }else if(in.find("aflow") != string::npos){
                int f = 0;
                while(getline(args, arg, ',')){
                    if(cnt < 2){
                        tp += arg+",";
                    }
                    else{
                        std::smatch matches;
                        regex_search(arg, matches, regex("\\d+"));
                        f = stoi(matches.str());
                    }
                    cnt++;
                } 
                // cout << tp << f << endl;
                flow2.push_back({tp,f});
            }else if(in.find("maxflow") != string::npos){
                cmax = in;
            }
        }
    }
    goal2 = goal2.substr(0,goal2.length()-1);
    cout << "goal : " << goal2 << endl;
    

    int sum1 = 0;
    int g1=0;
    for(auto i : flow1){
        sum1 += i.second;
        if(i.first.find(goal1) != string::npos){
            g1+= i.second;
        }
    }
    int sum2 = 0;
    int g2=0;
    for(auto i : flow2){
        sum2 += i.second;
        if(i.first.find(goal2) != string::npos){
            g2+= i.second;
        }
    }
    
    // cout << "sum1: " << sum1 << " vs sum2: " << sum2 << endl;
    // cout << "g1: " << g1 << " vs g2: " << g2 << endl; 
    cout << "max: " << lmnmax << " vs " << cmax << endl;    
    return ;
}


int main(){

    // edge();
    flow();

    return 0;
}