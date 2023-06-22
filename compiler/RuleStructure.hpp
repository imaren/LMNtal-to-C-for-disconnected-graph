#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "atom.hpp"
#include "guard.hpp"

class RuleStructure
{
private:
    /* data */
    atom target;
    size_t numhead;
    std::vector<atom> head_delete;
    std::vector<atom> head_remain;
    std::vector<guard> guards;
    std::vector<atom> body;
    bool uniq_flag = false;
    bool uniq_rule = false;
    bool target_delflag;

public:
    RuleStructure(std::string rule);
    RuleStructure(){};
    ~RuleStructure();
    void TypeFromGuard();

    bool setTarget(size_t pos){
        if(pos > head_remain.size()-1){
            pos = pos - head_remain.size();
            target = head_delete[pos];
            head_delete[pos] = head_delete.back();
            head_delete.pop_back();
            target_delflag = true;
        }else{
            target = head_remain[pos];
            head_remain[pos] = head_remain.back();
            head_remain.pop_back();
            target_delflag = false;
        }
        return true;
    }

    size_t getNumhead(){
        return numhead;
    }

    void printHead(){
        std::cout << "head delete: " ;
        for(size_t i = 0; i < head_delete.size(); i++){
            head_delete[i].printAtom();
            if(i != head_delete.size()-1) std::cout << ", ";
        }
        std::cout << std::endl << "head remain: ";
        for(size_t i = 0; i < head_remain.size(); i++){
            head_remain[i].printAtom();
            if(i != head_remain.size()-1) std::cout << ", ";
        }
        std::cout << std::endl;
    }

    void printGuard(){
        std::cout << "guard: " ;
        bool f = false;
        for(auto i : guards){
            if(f)std::cout << ", ";
            i.printText();
            f=true;
        }
        std::cout << std::endl;
    }

    void printBody(){
        std::cout << "body: " ;
        for(size_t i = 0; i < body.size(); i++){
            body[i].printAtom();
            if(i != body.size()-1) std::cout << ", ";
        }
        std::cout << std::endl;
    }

    void printRule(){
        printHead(); printGuard(); printBody();
    }
};

RuleStructure::RuleStructure(std::string rule)
{
    std::string head1, head2, tmp;
    std::istringstream st(rule);
    bool fitr = true;
    while(getline(st,tmp,':')){
        if(fitr){
            size_t bslash = tmp.find('\\');
            if(bslash != std::string::npos){
                head1 = tmp.substr(0,bslash-1);
                head2 = tmp.substr(bslash+1);
            }else{
                head1 = "";
                head2 = tmp;
            }

            std::istringstream h(head1);
            while(getline(h,tmp,',')){
                if(tmp != ""){
                    head_remain.push_back(atom(tmp));
                }
            }
            std::istringstream h2(head2);
            while(getline(h2,tmp,',')){
                if(tmp != ""){
                    head_delete.push_back(atom(tmp));
                }
            }            
            fitr = false;
        }else{
            size_t bar = tmp.find('|');
            std::string tmp_g = tmp.substr(1,bar-1), tmp2;
            std::istringstream b(tmp_g);
            if(bar != std::string::npos){
                // std::cout << "guard & body: " << tmp_g << std::endl;

                while(getline(b,tmp2,',')){
                    if(tmp2 != ""){
                        if(tmp2.find("uniq(") != std::string::npos){
                            uniq_flag = true;
                        }else if(tmp2.find("uniq") != std::string::npos){
                            uniq_rule = true;
                        }
                        guards.push_back(guard(tmp2));
                    }
                }
                tmp_g = tmp.substr(bar+1);
            }
            std::istringstream b2(tmp_g);
            while(getline(b2,tmp2,',')){
                if(tmp2.find('<') != std::string::npos){
                    //併合
                }else{
                    body.push_back(atom(tmp2));
                }
            }
        }
    }
    numhead = head_delete.size() + head_remain.size();
}

RuleStructure::~RuleStructure()
{
}

void RuleStructure::TypeFromGuard(){

}