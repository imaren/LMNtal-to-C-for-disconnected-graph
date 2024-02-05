#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

class atom{
public:
    std::string name;
    std::string argtypes;
    std::vector<std::string> args;

    atom(){};
    ~atom(){};

    atom(std::string in){
        if(in.find('(') == std::string::npos) name = in;
        else{
            size_t pos = in.find('(');
            name = in.substr(0,pos);
            std::string argstring = in.substr(pos+1,in.length()-2);
            std::istringstream st(argstring);
            std::string tmp;
            while(getline(st, tmp, ',')){
                if(tmp == "" )continue;
                tmp = tmp.substr(0,tmp.find(')'));
                args.push_back(tmp);
                if(tmp[0] >= '0' && tmp[0] <= '9'){
                    if(tmp.find('.') != std::string::npos){
                        argtypes+='f';
                    }else{
                        argtypes+='i';
                    }
                }else if(tmp[0] == '!'){
                    argtypes+='h';
                }else if(tmp[0] == '"'){
                    argtypes+='s';
                }else{
                    //default
                    argtypes+='d';
                }
            }
        }
    };

    void updateArgType(size_t pos, char type){
        argtypes[pos] = type;
    }

    void printAtom(){
        std::cout << name ;
        if(args.size() >= 1){
            std::cout << "(" << args[0] ;
            for(size_t i = 1; i < args.size(); i++){
                std::cout << ", " << args[i];
            }
            std::cout << ")";
        }
    }

    void printDetails(){
        std::cout << "atom details: \n";
        std::cout << "\tname: " << name << std::endl;
        std::cout << "\targ types string: " << argtypes << std::endl;
        std::cout << "\targs: " ;
        for(auto i: args){
            std::cout << i << ", ";
        }
        std::cout << "\e[2D\e[0K" <<std::endl;
    }
};
