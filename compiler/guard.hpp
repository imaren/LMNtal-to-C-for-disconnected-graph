#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cctype>
#include <algorithm>
#include "expr.hpp"

class guard{
private:
    std::string stringExpr;
    std::string op;
    char varType; // i: int, f: float, s: string, h: hl, u: unary
    std::vector<std::string> vars;
    bool infix = false;
    expr left;
    expr right;

    // 引数無し uniq 
    bool uniqrule = false;

public:
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
        }
        else{
            infix = true;
            for(size_t i = 0; i < stringExpr.length(); i++){
                if(t2flag){
                    tmp2+=stringExpr[i];
                }
                else{
                    switch (stringExpr[i])
                    {
                    case '\\':
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
                        if(stringExpr[i+1] == '.'){
                            varType = 'f'; i++;
                        }else{
                            varType = 'i';
                        }
                        op = "<";
                        break;

                    case '>':
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
            right = expr(tmp2);
        }
    };

    void printText(){
        std::cout << stringExpr ;
    }
};