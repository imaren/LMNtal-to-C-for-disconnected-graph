#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <set>
#include <list>
#include "atom.hpp"
#include "guard.hpp"
#include "RuleStructure.hpp"


class TargetRuleStructure
{
public:
    /* data */
    atom target;
    size_t numhead;
    
    std::vector<atom> head_delete;
    std::vector<atom> head_remain;
    std::vector<guard> guards;
    std::vector<guard> target_guard;
    std::unordered_set<std::string> linkes_from_target;
    std::vector<guard> from_target_guard;
    std::vector<guard> default_guard;
    std::vector<atom> body;
    bool uniq_flag = false;
    bool uniq_rule = false;
    bool target_delflag;

public:
    TargetRuleStructure(RuleStructure rule, size_t targetpos);
    TargetRuleStructure(){};
    ~TargetRuleStructure();
    void TypeFromGuard();
    void SplitTargetGuard();
    void CheckLinks();
    void SetLinkFromTarget(std::vector<std::string> vars);

    bool setTarget(size_t pos){
        if(pos >= head_remain.size()){
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

    void printTarget(){
        std::cout << "target: ";
        this->target.printAtom();
        std::cout << " del?: " << (this->target_delflag ? "yes" : "no") << std::endl;
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
        printTarget(); printHead(); printGuard(); printBody();
    }

    void printDetails(){
        printRule();
        std::cout << "target link names: " << "\n\t";
        for(auto i : target.args){
            std::cout << i << ", ";
        }
        std::cout << "\e[2D\e[0K\n pairs link names: " << "\n\t";
        for(auto i : head_remain){
            for(auto j : i.args){
                std::cout << j << ", ";
            }
        }
        for(auto i : head_delete){
            for(auto j : i.args){
                std::cout << j << ", ";
            }
        }
        std::cout << "\e[2D\e[0K\nbody link names: " << "\n\t";
        for(auto i : body){
            for(auto j : i.args)
            std::cout << j << ", ";
        }
        std::cout << "\e[2D\e[0K\n";
 
    }
};

TargetRuleStructure::TargetRuleStructure(RuleStructure rule, size_t targetpos)
{
    head_delete = rule.head_delete;
    head_remain = rule.head_remain;
    guards = rule.guards;
    body = rule.body;
    numhead = rule.getNumhead();
    setTarget(targetpos);
    SplitTargetGuard();
}

TargetRuleStructure::~TargetRuleStructure()
{
}

void TargetRuleStructure::TypeFromGuard(){

}

void TargetRuleStructure::SetLinkFromTarget(std::vector<std::string> vars){
    bool flag = false;
    for(auto i : vars){
        for(auto j : target.args){
            if(i == j){
                flag = true;
                break;
            }
        }
        if(flag)continue;
        this->linkes_from_target.insert(i);
    }
}

void TargetRuleStructure::SplitTargetGuard(){
    std::vector<std::string> links;
    links = this->target.args;
    std::vector<guard> g = this->guards;
    //target guard
    for(auto itr = g.begin();  itr != g.end(); ){
        if(itr->op == "uniq"){
            ++itr;
            continue;
        }
        if(itr->op == "num"){
            std::string var = itr->vars[0];
            bool syntax_check = false;
            for(auto i : links){
                if(var == i){
                    this->target_guard.push_back(*itr);
                    itr = g.erase(itr);
                    syntax_check = true;
                    break;
                }
            }
            if(!syntax_check){
                std::cerr << "syntax error: link not occurred " << itr->stringExpr << std::endl;
                exit(2);
            }
        }
        bool flag = false;
        // for(size_t i=0; i < itr->vars.size(); i++){
            for(std::string j : links){
                // 左辺と右辺のどちらかに完全一致する場合は、target_guard に登録する
                if(itr->left.strExpr == j){
                    this->target_guard.push_back(*itr);
                    SetLinkFromTarget(itr->right.vars);
                    itr = g.erase(itr);
                    flag = true;
                    break;
                }
                if(itr->right.strExpr == j){
                    this->target_guard.push_back(*itr);
                    SetLinkFromTarget(itr->left.vars);
                    itr = g.erase(itr);
                    flag = true;
                    break;
                }
                // 左辺と右辺のどちらかに部分一致する場合は、from_target_guard に登録する
                // 右辺に部分一致する場合は、代入文なら links_from_target に登録する
                for(auto k : itr->left.vars){
                    if(k == j){
                        if(itr->left.vars.size() == 1){//完全一致
                            this->target_guard.push_back(*itr);
                        }else{// 部分一致
                            this->from_target_guard.push_back(*itr);
                        }
                        SetLinkFromTarget(itr->vars);
                        itr = g.erase(itr);
                    }
                }
                for(auto k : itr->right.vars){
                    if(k == j){
                        if(itr->right.vars.size() == 1){//完全一致
                            this->target_guard.push_back(*itr);
                        }else{// 部分一致
                            this->from_target_guard.push_back(*itr);
                        }
                        SetLinkFromTarget(itr->vars);
                        itr = g.erase(itr);
                        flag = true;
                    }
                }
            }
        // }
        if(flag)continue;
        ++itr;
    }
}


void TargetRuleStructure::CheckLinks(){
    std::list<std::pair<std::string, char>> links;
    auto itr = links.begin();
    // guard に出現するリンクについて、リンク名と型の組み合わせを登録する
    for(guard g : this->guards){
        for(size_t i=0; i < g.vars.size(); i++){
            std::pair<std::string, char> tmp = {g.vars[i],g.varType};
            if(links.empty()){
                links.push_back(tmp);
            }else{
                 for(auto itr = links.begin(); itr != links.end(); itr++){
                    if(itr->first < tmp.first){
                        links.insert(itr,tmp);
                        break;
                    }else if(itr->first == tmp.first){
                        if(itr->second != tmp.second){
                            
                            std::cerr << "syntax error: missmatch guard type" << std::endl;
                            exit(1);
                        }
                        break;
                    }
                 }
            }
           
        }
    }
}

void EmbedSingleSourceRule(std::vector<TargetRuleStructure> vec_trs){

}