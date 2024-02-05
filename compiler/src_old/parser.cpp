#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <stdlib.h>

#include "guard.hpp"
#include "atom.hpp"
#include "expr.hpp"
#include "TargetRuleStructure.hpp"

enum class State : uint32_t{
    Atom,
    Head,
    H_atom,
    H_args,
    Guard,
    GuardOrBody,
    Body,
    B_atom,
    B_args,
};

// TargetRuleStructure まで作っちゃう
// TRstructure は Target が定められたルールのベクタ, graph_string は初期状態の文字列表現
void parse(std::string filename, std::vector<TargetRuleStructure> *TRstructure, std::string *graph_string){
    std::ifstream i_file(filename);
    if(!i_file.is_open()){
        std::cerr << "Could not open input file - " << filename << std::endl;
        exit(1);
    }
    std::string line, item="";
    std::vector<std::string> splits;

    bool cont_flag = false;
    while(getline(i_file,line)){
        if(line == "")continue;
            item+= line;
    }
    
    State state = State::Atom;
    std::vector<atom> atoms;
    for(size_t i = 0; i < item.length(); i++){
        switch (state)
        {
        case State::Atom:
            
            break;
        
        default:
            break;
        }
    }
}