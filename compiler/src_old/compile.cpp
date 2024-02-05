#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "TargetRuleStructure.hpp"

// file から '.' 区切りで入力. string に.
// string を rule と initial graph の文字列表現 に分割
// rule を RuleStructure に.
bool get_input(std::string filename, std::vector<TargetRuleStructure> *TRstructure, std::string *graph_string){
    std::ifstream i_file(filename);
    if(!i_file.is_open()){
        std::cerr << "Could not open input file - " << filename << std::endl;
        return false;
    }
    std::string line, item;
    std::vector<std::string> splits;

    bool cont_flag = false;
    while(getline(i_file,line)){
        if(line == "")continue;
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        std::istringstream st(line);
        bool fitr = true;
        while(std::getline(st, item, '.')){
            // std::cout << "item: " << item << std::endl; 
            
            if(fitr && cont_flag){
                splits.back() = splits.back() + item;
                fitr = false;
            }
            else splits.push_back(item);
            // float 
            char b = item.back();
            if( (b >= '0' && b <= '9') || b == '+' || b == '-' || b == '*' || b == '/' || b == '<' || b == '=' || b == '>'){
                cont_flag = true; fitr = true;
                splits.back() += '.';
            }else{
                cont_flag = false;
            }
        }

        if(line.back() == '.') cont_flag = false;
        else cont_flag = true;
    }

    std::vector<std::string> rules; 
    std::vector<RuleStructure> Rstructure;
    for (auto i : splits){
        if(i.find(":-") != std::string::npos)rules.push_back(i);
        else {
            *graph_string+=i+'.';
        }
    }
    for(auto i : rules){
        RuleStructure tmp(i);
        Rstructure.push_back(tmp);
    }
    for(auto i : Rstructure){
        for(int j = 0; j < i.head_remain.size(); j++){
            TargetRuleStructure tmp(i,j);
            TRstructure->push_back(tmp);
        }
        for(int j = 0; j < i.head_delete.size(); j++){
            TargetRuleStructure tmp(i,j+i.head_remain.size());
            TRstructure->push_back(tmp);
        }
    }
    return true;
}

//input を ルールと初期グラフに分ける
// bool split_rule(std::vector<std::string> input, std::vector<std::string> *rules, std::string *other){
//     for (auto i : input){
//         if(i.find(":-") != std::string::npos)rules->push_back(i);
//         else *other+='.'+i;
//     }
//     return true;
// }

// bool makeRuleStructure(std::vector<std::string> input, std::vector<RuleStructure> *rules){
//     for(auto i : input){
//         RuleStructure tmp(i);
//         rules->push_back(tmp);
//     }
//     return true;
// }

int main(int argc, char *argv[]){
    if(argc < 2){
        std::cerr << "no input file" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    std::vector<TargetRuleStructure> rules;
    std::string graph_string;
    if(!get_input(filename,&rules,&graph_string)){
        std::cerr << "cannot get input" << std::endl;
        exit(1);
    }
    
    //test input
    std::cout << "initial graph: " << graph_string << std::endl;
    int itr = 0;
    for(auto i : rules){
        std::cout << itr << std::endl;
        i.printRule();
        std::cout << std::endl;
        itr++;
    }
    //end test input
    

    return 0;
}