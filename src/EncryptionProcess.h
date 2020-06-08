#pragma once

#include <stdint.h>
#include <vector>
#include <memory>
#include <string>

#include "Process.h"
#include "AES.h"

using namespace std;

enum EncryptionOperation {
    ENCRYPT,
    DECRYPT
};

class EncryptionProcess: public Process {

    public:

    EncryptionProcess(EncryptionOperation op, AESKeySize size, AESMode mode, vector<uint8_t> key);
    EncryptionProcess(EncryptionOperation op, AESKeySize size, AESMode mode, vector<uint8_t> key, vector<uint8_t> iv);

    bool Execute(string& filePath, string& outputPath);

    private:

    void Init(EncryptionOperation op, AESKeySize size, AESMode mode, vector<uint8_t> key);

    unique_ptr<AES> aes;

    vector<uint8_t> key;
    vector<uint8_t> iv;

    EncryptionOperation op;
};