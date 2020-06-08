#include "CompressionProcess.h"

CompressionProcess::CompressionProcess(ProcessType alg){
    this->alg = alg;
}

bool CompressionProcess::Execute(string& filePath, string& outputPath){
    return filePath == "nullptr" && outputPath == "nullptr";
}