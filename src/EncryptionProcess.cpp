#include "EncryptionProcess.h"
#include "AES.h"
#include <memory>

void EncryptionProcess::Init(EncryptionOperation op, AESKeySize size, AESMode mode, vector<uint8_t> key){
    if ((long unsigned int)size / 8 != key.size()){
        throw runtime_error("Invalid key size."); 
    }

    this->aes = make_unique<AES>(size, mode, key.data());
    this->op = op;
}

EncryptionProcess::EncryptionProcess(EncryptionOperation op, AESKeySize size, AESMode mode, vector<uint8_t> key)  {
    this->Init(op, size, mode, key);
}

EncryptionProcess::EncryptionProcess(EncryptionOperation op, AESKeySize size, AESMode mode, vector<uint8_t> key, vector<uint8_t> iv) {

    this->Init(op, size, mode, key);

    if (AES_BLOCK_SIZE != iv.size()){
        throw runtime_error("Invalid IV size."); 
    }

    this->aes->SetIv(iv.data());
}

bool EncryptionProcess::Execute(string& filePath, string& outputPath){
    
    switch (this->op){
        case EncryptionOperation::ENCRYPT:
            if (this->aes->EncryptFile(filePath, outputPath) == 0){
                return true;
            }
        break;
        case EncryptionOperation::DECRYPT:
            if (this->aes->DecryptFile(filePath, outputPath) == 0){
                return true;
            }
        break;
    }
    return false;
}