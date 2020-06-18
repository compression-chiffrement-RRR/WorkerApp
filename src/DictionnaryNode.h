#pragma once

#include <stdint.h>
#include <memory>

class DictionnaryNode {
    
    private:

    void Init(size_t weight, uint8_t value);
    
    public:

    size_t weight;
    uint8_t value;
    std::shared_ptr<DictionnaryNode> left;
    std::shared_ptr<DictionnaryNode> right;
    
    DictionnaryNode(size_t weight, uint8_t value);
    DictionnaryNode(size_t weight);

    ~DictionnaryNode();

    void DisplayTree ();

};