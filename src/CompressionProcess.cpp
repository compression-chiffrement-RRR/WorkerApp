#include "CompressionProcess.h"
#include "Huffman.h"

CompressionProcess::CompressionProcess(CompressionOperation alg){
    this->alg = alg;
}

bool CompressionProcess::Execute(string& filePath, string& outputPath){
    Huffman h;
    switch (this->alg){
        case CompressionOperation::OPERATION_COMPRESS_HUFFMAN:
            return h.CompressFile(filePath, outputPath); 
        case CompressionOperation::OPERATION_DECOMPRESS_HUFFMAN:
            return h.DecompressFile(filePath, outputPath); 
    }
    return false;
}