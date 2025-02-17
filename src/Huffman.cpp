#include "Huffman.h"

#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "debug.h"

static bool CompareNodes (std::shared_ptr<DictionnaryNode> i, std::shared_ptr<DictionnaryNode> j) { 
    return i->weight < j->weight; 
};

/* CONSTRUCTORS/DESTRUCTOR */

Huffman::Huffman (){
    this->Init();
};  

Huffman::~Huffman (){};

/* PRIVATE FUNCTIONS */

void Huffman::Init(){

    for (size_t i = 0; i < 256; i++){
        this->bytesOccurrences[i] = 0;
    }

    for (size_t i = 0; i < 256; i++){
        this->codesMap[i] = {};
    }

    this->encodedBits = 0;
    this->tmpNodes = {};
    this->rawDictionnary = {};
    this->dictionnary = nullptr;
    this->currentNode = nullptr;
    this->currentByte = 0b00000000;

};

void Huffman::UpdateOccurences(uint8_t *buffer, size_t length){
    
    for (size_t i = 0; i < length; i ++){
        this->bytesOccurrences[buffer[i]]++;
    }
};

void Huffman::CreateTempNodes(){
    
    for (size_t i = 0; i < sizeof(this->bytesOccurrences) / sizeof(size_t); i++){
        
        if (this->bytesOccurrences[i] > 0){

            SerializedNode sNode;
            sNode.occurences = this->bytesOccurrences[i];
            sNode.value = i;
            
            this->rawDictionnary.push_back(sNode);
            
            this->tmpNodes.push_back(
                std::make_shared<DictionnaryNode>(this->bytesOccurrences[i], (uint8_t)i)
            );
        }
    }
};

void Huffman::CreateDictionnary(){

    // empty dictionnary
    if (this->tmpNodes.size() == 0){
        this->dictionnary = std::make_shared<DictionnaryNode>(0);
        this->currentNode = this->dictionnary;
        return;
    }

    // special case if there is only one character occuring in the whole data.
    if (this->tmpNodes.size() == 1){
        this->dictionnary = std::make_shared<DictionnaryNode>(this->tmpNodes[0]->weight);
        this->dictionnary->left = this->tmpNodes[0];
        this->currentNode = this->dictionnary;
        return;
    } 

    // more than two characters occuring
    while (this->tmpNodes.size() > 1){

        // Sort values
        std::sort(this->tmpNodes.rbegin(), this->tmpNodes.rend(), CompareNodes);

        size_t lastIndex = this->tmpNodes.size() - 1;

        std::shared_ptr<DictionnaryNode> newNode = std::make_shared<DictionnaryNode>(
            this->tmpNodes[lastIndex]->weight + this->tmpNodes[lastIndex - 1]->weight
        );

        newNode->left = this->tmpNodes[lastIndex];
        newNode->right = this->tmpNodes[lastIndex - 1];

        this->tmpNodes.pop_back();
        this->tmpNodes.pop_back();

        this->tmpNodes.push_back(newNode);
    }

    this->dictionnary = this->tmpNodes[0];
    this->tmpNodes.pop_back();

    this->currentNode = this->dictionnary;
};

void Huffman::GetCodes(std::shared_ptr<DictionnaryNode> node, std::vector<Direction> code){

    if (node->left != nullptr){
        std::vector<Direction> leftVector = code;
        leftVector.push_back(Direction::LEFT);
        this->GetCodes(node->left, leftVector);
    }

    if (node->right != nullptr){
        std::vector<Direction> rightVector = code;
        rightVector.push_back(Direction::RIGHT);
        this->GetCodes(node->right, rightVector);
    }

    if (node->right == nullptr && node->left == nullptr){
        for (Direction bit: code){
            this->codesMap[node->value].push_back(bit);
        }
    }
};

void Huffman::Compress(uint8_t *input, size_t *length, uint8_t *output){

    size_t originalLength = (*length);
    (*length) = 0;

    for (size_t i = 0; i < originalLength; i ++){

        std::vector<Direction> code = this->codesMap[input[i]];

        for (Direction bit: code){

            if (bit == Direction::LEFT){
                this->currentByte |= (0b10000000 >> (this->encodedBits % 8));
            }

            this->encodedBits++;

            if (this->encodedBits % 8 == 0){
                
                output[(*length)] = this->currentByte;
                (*length)++;

                this->currentByte = 0b00000000;
            }
        }
    }
};

void Huffman::CompressFinalize(uint8_t *buffer, size_t *length){
    
    if (this->encodedBits % 8 == 0){
        (*length) = 0;
        return;
    }

    buffer[0] = this->currentByte;
    (*length) = 1;
};

void Huffman::Decompress(uint8_t *bytes, size_t *length, uint8_t *output){

    size_t bytesWritten = 0;

    for (size_t i = 0; i < (*length); i ++){

        for (uint8_t y = 0; y < 8; y ++){

            if (this->encodedBits == 0){
                (*length) = bytesWritten;
                return;
            }

            if ((bytes[i] >> (7 - y)) & Direction::LEFT){
                this->currentNode = this->currentNode->left;
            } else {
                this->currentNode = this->currentNode->right;
            }

            if (this->currentNode->left == nullptr && this->currentNode->right == nullptr){

                output[bytesWritten] = this->currentNode->value;
                bytesWritten++;

                this->currentNode = dictionnary;
            }

            this->encodedBits--;
        }
    }

    (*length) = bytesWritten;
};

void Huffman::ExtractDictionnary(uint8_t *buffer, size_t length){

    if (length % sizeof(SerializedNode) != 0 || length == 0){
        return;
    }

    SerializedNode *n = (SerializedNode *) buffer;

    for (size_t i = 0; i < length / sizeof(SerializedNode); i ++){
        this->tmpNodes.push_back(
            std::make_shared<DictionnaryNode>(n[i].occurences, n[i].value)
        );
    }
};

size_t Huffman::MaxSymbolLength (){
    
    size_t max = 0;
    for (size_t i = 0; i < 256; i ++){
        size_t s = this->codesMap[i].size();
        if (s > max)
            max = s;
    }
    return max;
}

/* PUBLIC FUNCTIONS */

#define HUFFMAN_BUFFER_SIZE 8192

bool Huffman::CompressFile(const std::string& inputPath, const std::string& outputPath){

    std::ifstream in;
    std::ofstream out;

    bool ret = false;

    uint8_t *buffer = nullptr, *output = nullptr;
    size_t read = 0, sizeOfDictionnary = 0;

    buffer = new uint8_t[HUFFMAN_BUFFER_SIZE];

    if (buffer == nullptr)
        goto clean;

    in.open(inputPath, std::ifstream::binary);
    out.open(outputPath, std::ofstream::binary|std::ofstream::trunc);

    if (!in.is_open()){
        std::cout << "Failed to open input file" << inputPath << " for Huffman compression." << std::endl;
        goto clean;
    }

    if (!out.is_open()){
        std::cout << "Failed to open output file " << outputPath << " for Huffman compression." << std::endl;
        goto clean;
    }

    this->Init();
    
    while (!in.eof()){
        
        in.read((char *)buffer, HUFFMAN_BUFFER_SIZE);

        if (in.bad() || (in.fail() && !in.eof())){
            std::cout << "Could not read input data for compression." << std::endl;
            goto clean;
        }
        
        this->UpdateOccurences(buffer, (size_t)in.gcount());
    }

    this->CreateTempNodes();
    this->CreateDictionnary();
    this->GetCodes(this->dictionnary, {});

    output = new uint8_t[HUFFMAN_BUFFER_SIZE * (this->MaxSymbolLength())];

    if (output == nullptr)
        goto clean;

    in.clear();
    in.seekg(0);
    
    if (in.fail() || in.bad()){
        std::cout << "Couldn not seek start of file for compression." << std::endl;
        goto clean;
    }

    while (!in.eof()){
        
        in.read((char *)buffer, HUFFMAN_BUFFER_SIZE);

        if (in.bad() || (in.fail() && !in.eof())){
            std::cout << "Could not read input data." << std::endl;
            goto clean;
        }

        read = in.gcount();

        if (read){

            this->Compress(buffer, &read, output);

            out.write((char *)output, read);
            if (out.bad() || out.fail()){
                std::cout << "Could not write compressed data." << std::endl;
                goto clean;
            }

        }
    }

    read = 0;

    this->CompressFinalize(buffer, &read);

    if (read > 0){
        out.write((char *)buffer, read);
        if (out.bad() || out.fail()){
            std::cout << "IO error during compression (write)." << std::endl;
            goto clean;
        }
    }

    out.write((char *)&(this->encodedBits), sizeof(this->encodedBits));
    if (out.bad() || out.fail()){
        std::cout << "Couldn't write encodedBits value." << std::endl;
        goto clean;
    }

    sizeOfDictionnary = this->rawDictionnary.size() * sizeof(SerializedNode);

    out.write((char *)this->rawDictionnary.data(), sizeOfDictionnary);
    if (out.bad() || out.fail()){
        std::cout << "Couldn't write Huffman dictionnary data." << std::endl;
        goto clean;
    }

    out.write((char *)&sizeOfDictionnary, sizeof(sizeOfDictionnary));
    if (out.bad() || out.fail()){
        std::cout << "Couldn't write size of Huffman dictionnary." << std::endl;
        goto clean;
    }

    ret = true;

clean:

    if (buffer != nullptr)
        delete [] buffer;

    if (output != nullptr)
        delete [] output;

    if (in.is_open())
        in.close();

    if (out.is_open()){
        out.close();
        if (ret != true)
            remove(outputPath.c_str());
    }
    
    return ret;
};

bool Huffman::DecompressFile(const std::string& inputPath, const std::string& outputPath){

    std::ifstream in;
    std::ofstream out;

    bool ret = false;

    uint8_t *buffer = nullptr, *output = nullptr;
    size_t read = 0, sizeOfDictionnary = 0;

    std::streamoff offset = 0;

    buffer = new uint8_t[HUFFMAN_BUFFER_SIZE];
    output = new uint8_t[HUFFMAN_BUFFER_SIZE * 8];

    if (buffer == nullptr || output == nullptr)
        goto clean;

    in.open(inputPath, std::ifstream::binary);
    out.open(outputPath, std::ofstream::binary|std::ofstream::trunc);

    if (!in.is_open() || !out.is_open())
        goto clean;

    this->Init();

    offset -= sizeof(sizeOfDictionnary);
    in.seekg(offset, std::ios_base::end);

    if (in.fail() || in.bad())
        goto clean;

    in.read((char*)&sizeOfDictionnary, sizeof(sizeOfDictionnary));

    if (in.fail() || in.bad())
        goto clean;

    offset -= sizeOfDictionnary;
    in.seekg(offset, std::ios_base::end);

    if (in.fail() || in.bad())
        goto clean;

    in.read((char *)buffer, sizeOfDictionnary);

    if (in.fail() || in.bad())
        goto clean;

    this->ExtractDictionnary(buffer, sizeOfDictionnary);
    this->CreateDictionnary();

    offset -= sizeof(this->encodedBits);
    in.seekg(offset, std::ios_base::end);

    if (in.fail() || in.bad())
        goto clean;

    in.read((char *)&(this->encodedBits), sizeof(this->encodedBits));
    if (in.fail() || in.bad())
        goto clean;

    in.seekg(0);
    if (in.fail() || in.bad())
        goto clean;

    while (!in.eof()){
        
        in.read((char *)buffer, HUFFMAN_BUFFER_SIZE);

        if (in.bad() || (in.fail() && !in.eof()))
            goto clean;

        read = in.gcount();

        this->Decompress(buffer, &read, output);
        
        if (read > 0){
            
            out.write((char *)output, read);
            if (out.bad() || out.fail())
                goto clean;
            
            continue;
        }

        break;  
    }

    ret = true;

clean:

    if (buffer != nullptr)
        delete [] buffer;

    if (output != nullptr)
        delete [] output;

    if (in.is_open())
        in.close();

    if (out.is_open()){
        out.close();
        if (ret != true)
            remove(outputPath.c_str());
    }
    
    return ret;
};