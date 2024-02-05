#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cctype>
#include <algorithm>
#include "expr.hpp"

class expr{
public:
    std::string strExpr;
    std::vector<std::string> vars;
    std::vector<std::string> ops;
    char type = 'd'; // i: int, f: float, s: string, h: hl, u: unary, d:undefined

    expr(){};
    ~expr(){};

    //TODO: 数式の変形
    // 四則演算に対応

    expr(std::string str){
        std::string tmp="";

        int level = 0;

        for(size_t i=0; i < str.length(); i++){
            switch (str[i])
            {
            case '+':
                if(str[i+1] == '.'){
                    updateType('f');
                }else{
                    updateType('i');
                }
                ops.push_back("+");
                vars.push_back(tmp);
                tmp="";
                break;

            case '-':
                if(str[i+1] == '.'){
                    updateType('f');
                }else{
                    updateType('i');
                }
                ops.push_back("-");
                vars.push_back(tmp);
                tmp="";
                break;

            case '*':
                if(str[i+1] == '.'){
                    updateType('f');
                }else{
                    updateType('i');
                }
                ops.push_back("*");
                vars.push_back(tmp);
                tmp="";
                break;

            case '/':
                if(str[i+1] == '.'){
                    updateType('f');
                }else{
                    updateType('i');
                }
                ops.push_back("/");
                vars.push_back(tmp);
                tmp="";
                break;

            case 'm':
                if(str.size() > i+2){
                    if(str[i+1] == 'o' && str[i+2] == 'd'){
                        updateType('i');
                        ops.push_back("mod");
                        vars.push_back(tmp);
                        tmp="";
                        break;
                    }
                }

                tmp+=str[i];
                break;

            case '(':
                level++;

            case ')':
                level--;
            
            default:
                tmp+=str[i];
                break;
            }
        }
        
        strExpr = str;
        if(level != 0){
            std::cerr << "sytanx error: unmatched parentheses parentheses" << std::endl;
        }
    };

    std::string getStrExpr(){
        return strExpr;
    }

    void updateType(char c){
        if(this->type != 'd'){
            if(this->type != c){
                std::cerr << "syntax error: guard type " << this->type << " ? " << c << std::endl; 
            }
        }
        this->type = c;
    }

    void printText(){
        std::cout << strExpr;
    }
};

class node_expr{

};