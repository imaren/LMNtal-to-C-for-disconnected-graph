#include <bits/stdc++.h>
#include <boost/spirit/include/qi.hpp>

#include "input.hpp"

namespace qi = boost::spirit::qi;

int main(int argc, char *argv[]){
    if(argc < 2){
        std::cerr << "no input file" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    input in = get(filename);
    
    for(auto i : in.rules){
        rule_str r(i);
        for(auto j : r.head_del){
            atom a(j);
            a.p();
        }
    }


    return 0;
}