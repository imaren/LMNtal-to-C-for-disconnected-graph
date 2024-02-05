#pragma once

#include <bits/stdc++.h>
#include "parse.hpp"
using namespace std;

struct input{
    string state="";
    vector<string> rules;
};

input get(string f){
    std::ifstream inputFile(f);

    if (!inputFile.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        exit(0);
    }

    std::string line;
    input result;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string token;
        std::string tmp="";
        while (std::getline(iss, token, '.')) {
            token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
            if (!token.empty()) {
                if (token.find(':') != std::string::npos) {
                    result.rules.push_back(token);
                } else {
                    result.state += token;
                }
            }
        }
    }

    inputFile.close();
    return result;
}

struct rule_str{
    string name;
    vector<string> head_rem;
    vector<string> head_del;
    vector<string> guard;
    vector<string> body;

    rule_str(string input){
        string head;
        string guard;
        string body;
        istringstream iss(input);
        string token;
        string tmp="";
        getline(iss, token, ':');
        auto pos = token.find("@@");
        if(pos != string::npos){
            name = token.substr(0,pos-1);
            head = token.substr(pos+2,token.length());
        }else{
            name = "";
            head = token;
        }
        pair<vector<string>, vector<string>> tp = split(head);
        this->head_del = tp.first;
        this->head_rem = tp.second;
        getline(iss, token, '.');
        pos = token.find('|');
        if(pos != string::npos){
            guard = token.substr(1,pos-1);
            body = token.substr(pos+1, token.length());
        }else{
            guard = "";
            body = token.substr(1, token.length());
        }
        this->guard = split(guard).first;
        this->body = split(body).first;
    }

    void p(){
        cout << "head_rem: " ;
        for(auto i : head_rem){
            cout << i << " " ;
        }cout << endl << "head_del: ";
        for(auto i : head_del){
            cout << i << " " ;
        }cout << endl;
        cout << "guard: " ;
        for(auto i : guard){
            cout << i << " " ;
        }cout << endl;
        cout << "body: " ;
        for(auto i : body){
            cout << i << " " ;
        }cout << endl;
    }
};

typedef int hlink;
struct datatype{
    bool def=false; // true= defined, false= undefined
    string t="";
};

struct atom{
    string name;
    vector<pair<string, string>> args; // pair<link name, data type>

    atom(string in){
        auto tmp = parse_atom(in);
        name = tmp.first; args = tmp.second;
    }

    void p(){
        cout << "name: " << name << endl;
        cout << "args: " ;
        for(auto i : args){
            cout << i.first << " " ;
        }cout << endl;
    }
};

enum gtype{
    Type,
    Assign,
    Compare,
    Uniq,
    Num
};

struct guard_t{
    string expr;
    gtype type;
    string left;
    string right;
    map<string,string> links;

    guard_t(string in, set<string> headlinks){
        expr = in;
    }
};

struct rule{
    string name;
    vector<atom> head_rem;
    vector<atom> head_del;
    vector<guard_t> guard;
    vector<atom> body;
    set<string> headlinks;
    map<string,string> links;


    rule(rule_str in){
        for(auto i : in.head_rem){
            atom a(i);
            for(auto j : a.args){
                headlinks.insert(j.first);
            }
            head_rem.push_back(a);
        }
        for(auto i : in.head_del){
            atom a(i);
            for(auto j : a.args){
                headlinks.insert(j.first);
            }
            head_del.push_back(a);
        }
        for(auto i : in.guard){
            guard_t a(i, headlinks);
            guard.push_back(a);
        }
        for(auto i : in.body){
            atom a(i);
            body.push_back(a);
        }
    }

};

struct rule_set{
    vector<rule> rules;
    set<string> typed_functor;
};
