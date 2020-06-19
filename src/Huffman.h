#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include "DictionnaryNode.h"

enum Direction {
    LEFT = 1,
    RIGHT = 0
};

typedef struct SerializedNode {
    size_t occurences;
    uint8_t value;
} SerializedNode;

class Huffman {

    private: 

    size_t bytesOccurrences[256] = { 0 };
    size_t encodedBits = 0;
    std::shared_ptr<DictionnaryNode> dictionnary = nullptr;
    std::shared_ptr<DictionnaryNode> currentNode = nullptr;
    std::vector<std::shared_ptr<DictionnaryNode>> tmpNodes = {};
    std::vector<Direction> codesMap [256] = { {} };
    std::vector<SerializedNode> rawDictionnary = {};
    uint8_t currentByte;

    void Init();

    void UpdateOccurences(uint8_t *bytes, size_t length);
    void CreateTempNodes();
    void CreateDictionnary();
    void GetCodes(std::shared_ptr<DictionnaryNode> node, std::vector<Direction> code);
    void Compress(uint8_t *bytes, size_t *length, uint8_t *output);
    void CompressFinalize(uint8_t *buffer, size_t *length);

    void ExtractDictionnary(uint8_t *buffer, size_t sizeOfDictionnary);
    void Decompress(uint8_t *bytes, size_t *length, uint8_t *output);

    public:

    //bool CompressBuffer(uint8_t *bytes, size_t length);
    //bool DecompressBufer(uint8_t *bytes, size_t length);
    bool CompressFile(const std::string& input, const std::string& output);
    bool DecompressFile(const std::string& input, const std::string& output);

    Huffman();
    ~Huffman();

};