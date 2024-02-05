#pragma once

#include <bits/stdc++.h>
#include "input.hpp"
using namespace std;

pair<vector<string>,vector<string>> split(string input){
    vector<string> result;
    vector<string> rem;
    istringstream ss(input);
    char ch;
    string token;
    while (ss.get(ch)) {
        if (ch == ',') {
            result.push_back(token);
            token.clear();
        } else if (ch == '(') {
            int nestc = 1;
            token += '(';
            while (nestc > 0) {
                ss.get(ch);
                if(ch == '('){
                    nestc++;
                }else if(ch == ')'){
                    nestc--;
                }
                token += ch;
            }
        }else if(ch == '\\'){
            result.push_back(token);
            rem = result;
            result.clear();
            token.clear();
        }else{
            token += ch;
        }
    }
    result.push_back(token);
    return {result,rem};
}

pair<string, vector<pair<string,string>>> parse_atom(string input){
    pair<string, vector<pair<string,string>>> result;
    
    char ch;
    string name;
    vector<pair<string,string>> args;
    string token;
    auto pos = input.find('(');
    if(pos == string::npos){
        name = input;
        return {name,args};
    }else{
        name = input.substr(0,pos);
        input = input.substr(pos+1,input.length());
    }
    
    istringstream ss(input);
    int nestc = 1;
    while (nestc > 0) {
        ss.get(ch);
        if (ch == ',') {
            args.push_back({token,""});
            token.clear();
        } else if (ch == '(') {
            nestc++;
            token += '(';
        }else if(ch == ')'){
            nestc--;
            token += ')';
        }else{
            token += ch;
        }
    }
    args.push_back({token.substr(0,token.length()-1),""});
    return {name,args};
}

tuple<string,string,vector<string>,vector<string>,char> parse_guard(string input){
    // 型制約と uniq -> op, null, list of links, null, guard type
    // assignとcomparison -> left, right, left-hand links, right-hand links, guard type
    tuple<string,string,gtype,vector<string>> result;
    string op="";
    auto pos = input.find('(');
    if(pos != string::npos){
        op = input.substr(0,pos-1);
        input = input.substr(pos,input.length());
    }

    

    char ch;
    string token;
    istringstream ss(input);
    int nestc = 1;
    while (nestc > 0) {
        ss.get(ch);
        if (ch == ',') {
            args.push_back({token,""});
            token.clear();
        } else if (ch == '(') {
            nestc++;
            token += '(';
        }else if(ch == ')'){
            nestc--;
            token += ')';
        }else{
            token += ch;
        }
    }
}
