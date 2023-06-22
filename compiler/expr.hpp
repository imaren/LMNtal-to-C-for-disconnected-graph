#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cctype>
#include <algorithm>
#include "expr.hpp"

class expr{
private:
    std::string strExpr;

public:
    expr(){};
    ~expr(){};

    //TODO: 数式の変形
    // 四則演算に対応

    expr(std::string str){
        strExpr = str;
    };

    std::string getStrExpr(){
        return strExpr;
    }
};