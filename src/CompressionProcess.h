#pragma once

#include "Process.h"

class CompressionProcess : public Process {

    public:

    CompressionProcess(ProcessType alg);

    bool Execute(string& filePath, string& outputPath);

    private:

    ProcessType alg;
};