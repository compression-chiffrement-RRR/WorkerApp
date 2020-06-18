#include "DictionnaryNode.h"

#include <iostream>
#include <memory>

void DictionnaryNode::Init(size_t weight, uint8_t value){
    this->weight = weight;
    this->value = value;
    this->left = nullptr;
    this->right = nullptr;
};

DictionnaryNode::DictionnaryNode(size_t weight, uint8_t value){
    this->Init(weight, value);
};

DictionnaryNode::DictionnaryNode(size_t weight){
    this->Init(weight, 0);
};

DictionnaryNode::~DictionnaryNode (){};