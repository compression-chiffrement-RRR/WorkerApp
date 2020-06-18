#pragma once

#include "Process.h"

enum CompressionOperation {
    OPERATION_COMPRESS_HUFFMAN,
    OPERATION_DECOMPRESS_HUFFMAN
};

class CompressionProcess : public Process {

    public:

    CompressionProcess(CompressionOperation alg);

    bool Execute(string& filePath, string& outputPath);

    private:

    CompressionOperation alg;
};