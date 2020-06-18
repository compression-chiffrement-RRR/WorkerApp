#include "CompressionProcess.h"
#include "Huffman.h"

CompressionProcess::CompressionProcess(ProcessType alg){
    this->alg = alg;
}

bool CompressionProcess::Execute(string& filePath, string& outputPath){
    switch (this->alg){
        case ProcessType::COMPRESS_HUFFMAN:
            Huffman h;
            return h.CompressFile(filePath, outputPath); 
            break;
        case ProcessType::DECOMPRESS_HUFFMAN:
            Huffman h;
            return h.DecompressFile(filePath, outputPath); 
            break;
    }
}