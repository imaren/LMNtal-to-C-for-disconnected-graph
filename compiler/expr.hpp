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

    expr(std::string str){
        strExpr = str;
    };
};