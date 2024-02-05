#pragma once

#include <list>
#include <iostream>
#include <set>
#include "TargetRuleStructure.hpp"
#include "atom.hpp"

class RuleBlock
{
public:
    std::vector<TargetRuleStructure> rules;
    int id;
    std::set<atom> generatable_atom;

    RuleBlock(int id);
    ~RuleBlock();
    void addRule(TargetRuleStructure);

    void printRuleBlock(){
        std::cout << "Block: " << id << std::endl;
        for(auto i : rules){
            i.printRule();
        }
    }
};

RuleBlock::RuleBlock(int id)
{
    this->id = id;
}

RuleBlock::~RuleBlock()
{
}

void RuleBlock::addRule(TargetRuleStructure r){
    for(auto i : r.body){
        generatable_atom.insert(i);
    }
    this->rules.push_back(r);
}
