#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <stdlib.h>
#include "expr.hpp"

class guard{
public:
    std::string stringExpr;
    std::string op;
    char varType; // i: int, f: float, s: string, h: hl, u: unary
    std::vector<std::string> vars;
    bool infix = false;
    expr left;
    expr right;

    // 引数無し uniq 
    bool uniqrule = false;


    guard(){};
    ~guard(){};

    guard(std::string in){
        stringExpr = in;
        std::string tmp1="", tmp2="";
        bool t2flag = false;
        if(stringExpr.find("uniq") != std::string::npos){
            op = "uniq";
            size_t pos = stringExpr.find('(');
            if(pos == std::string::npos){
                uniqrule = true;
            }else{
                tmp1 = stringExpr.substr(pos,tmp1.find(')')-1);
                std::istringstream st(tmp1);
                while(getline(st, tmp2, ',')){
                    vars.push_back(tmp2);
                }
            }
        }else if(stringExpr.find("num") != std::string::npos){
            op = "num";
            size_t pos = stringExpr.find('(');
            tmp1 = stringExpr.substr(pos,tmp1.find(')')-1);
            vars.push_back(tmp1);
            varType = 'h';
        }else if(stringExpr.substr(0,2) == "int"){
            if(in[3] != '('){
                std::cerr << "syntax error" << std::endl;
                exit(1);
            }
            op = "int";
            size_t pos = stringExpr.find(')');
            if(pos == std::string::npos){
                std::cerr << "syntax error" << std::endl;
                exit(1);
            }
            tmp1 = stringExpr.substr(4,pos);
            varType = 'i';
        }else if(stringExpr.substr(0,4) == "float"){
            if(in[5] != '('){
                std::cerr << "syntax error" << std::endl;
                exit(1);
            }
            op = "float";
            size_t pos = stringExpr.find(')');
            if(pos == std::string::npos){
                std::cerr << "syntax error" << std::endl;
                exit(1);
            }
            tmp1 = stringExpr.substr(6,pos);
            varType = 'f';
        }else if(stringExpr.substr(0,4) == "hlink"){
            if(in[5] != '('){
                std::cerr << "syntax error" << std::endl;
                exit(1);
            }
            op = "hlink";
            size_t pos = stringExpr.find(')');
            if(pos == std::string::npos){
                std::cerr << "syntax error" << std::endl;
                exit(1);
            }
            tmp1 = stringExpr.substr(6,pos);
            varType = 'h';
        }else{
            infix = true;
            for(size_t i = 0; i < stringExpr.length(); i++){
                if(t2flag){
                    tmp2+=stringExpr[i];
                }
                else{
                    switch (stringExpr[i])
                    {
                    case '\\':
                        t2flag = true;
                        if(stringExpr[i+1] == '='){
                            if(stringExpr[i+2] == '='){
                                varType = 'u'; i+=2;
                            }else{
                                //ground だけど一旦 unary 扱い
                                varType = 'u'; i++;
                            }
                            op = "!=";
                        }else{
                            std::cerr << "syntax error: guard '\\'" << std::endl;
                        }
                        break;

                    case '=':
                        t2flag = true;
                        if(stringExpr[i+1] == '<'){
                            if(stringExpr[i+2] == '.'){
                                varType = 'f'; i+=2;
                            }else{
                                varType = 'i'; i++;
                            }
                            op = "<=";
                        }else if(stringExpr[i+1] == '='){
                            varType = 'u'; op = "=="; i++;
                        }else if(stringExpr[i+1] == ':'){
                            if(stringExpr[i+2] == '='){
                                if(stringExpr[i+3] == '.'){
                                    varType = 'f'; i+=3;
                                }else{
                                    varType = 'i'; i+=2;
                                }
                                op = "==";
                            }else{
                                i++;
                                std::cerr << "syntax error: guard '=:'" << std::endl;
                            }
                        }else if(stringExpr[i+1] == '\\'){
                            if(stringExpr[i+2] == '='){
                                if(stringExpr[i+3] == '.'){
                                    varType = 'f'; i+=3;
                                }else{
                                    varType = 'i'; i+=2;
                                }
                                op = "!=";
                            }else{
                                i++;
                                std::cerr << "syntax error: guard '=\\'" << std::endl;
                            }
                        }else{
                            //ground だけど一旦 unary
                            varType = 'u'; op = "=";
                        }
                        break;
                    
                    case '<':
                        t2flag = true;
                        if(stringExpr[i+1] == '.'){
                            varType = 'f'; i++;
                        }else{
                            varType = 'i';
                        }
                        op = "<";
                        break;

                    case '>':
                        t2flag = true;
                        if(stringExpr[i+1] == '='){
                            if(stringExpr[i+2] == '.'){
                                varType = 'f'; i+=2;
                            }else{
                                varType = 'i'; i++;
                            }
                            op = ">=";
                        }else if(stringExpr[i+1] == '.'){
                            varType = 'f'; i++; op = ">";
                        }else{
                            varType = 'i'; op = ">";
                        }
                        break;

                    default:
                        tmp1+=stringExpr[i];
                        break;
                    }
                }
            }
            left = expr(tmp1);
            updateType(left.type);
            right = expr(tmp2);
            updateType(right.type);
            vars.push_back(tmp1);
            vars.push_back(tmp2);
            if(op.size() == 0){
                std::cerr << "syntax error: " << stringExpr << std::endl;
                exit(1);
            }
        }
    };

    void printText(){
        std::cout << stringExpr ;
    }

    void printVars(){
        std::cout << "guard vars: " ;
        for(auto i : vars){
            std::cout << i << ", " ;
        }
        std::cout << std::endl;
    }

    void printDetails(){
        std::cout << "op: " << this->op << std::endl;
        std::cout << "left: ";
        this->left.printText();
        std::cout << std::endl << "right: ";
        this->right.printText();
        std::cout << std::endl;
    }

    void updateType(char type){
        if(type == 'd') return;
        if(varType == type) return;
        if(varType == 'i' || varType == 'c' || varType == 'h' || varType == 's'){
            std::cerr << "syntax error: type mismatch" << std::endl;
            exit(1);
        }
    }

    std::vector<std::string> getGuardLinks(std::unordered_multiset<std::string> headLinks){
        std::vector<std::string> result;
        if(op == "="){
            if(left.vars.size() == 1){

            }
        }else{

        }
    }
};