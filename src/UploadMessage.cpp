#include <nlohmann/json.hpp>
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include <random>

#include "AES.h"
#include "HttpClient.h"
#include "UploadMessage.h"
#include "Process.h"
#include "CompressionProcess.h"
#include "EncryptionProcess.h"

using json = nlohmann::json;
using namespace std;

static string RandomString(size_t length);

static string RandomString(size_t length)
{
    const string allowedChars = "abcdefghijklmnopqrstuvwxyz0123456789";

    random_device device;
    mt19937 generator(device());
    uniform_int_distribution<> distribution(0, allowedChars.size() - 1);

    string out;

    for (size_t i = 0; i < length; ++i)
    {
        out += allowedChars[distribution(generator)];
    }

    return out;
}

UploadMessage::UploadMessage (json body){

    this->fileUrl = body["fileUrl"];
    this->responseUrl = body["responseUrl"];
    this->fileID = body["fileID"];

    for (const json process: body["processes"]){

        switch (Process::DeserializeProcessType(process["type"])){

            case ProcessType::ENCRYPT_AES_128_ECB:
                this->processes.push_back(make_shared<EncryptionProcess>(
                    EncryptionOperation::ENCRYPT,
                    AESKeySize::AES_128, 
                    AESMode::ECB, 
                    process["key"])
                );
                break;

            case ProcessType::ENCRYPT_AES_192_ECB:
                this->processes.push_back(make_shared<EncryptionProcess>(
                    EncryptionOperation::ENCRYPT,
                    AESKeySize::AES_192, 
                    AESMode::ECB, 
                    process["key"])
                );
                break;
                
            case ProcessType::ENCRYPT_AES_256_ECB:
                this->processes.push_back(make_shared<EncryptionProcess>(
                    EncryptionOperation::ENCRYPT,
                    AESKeySize::AES_256, 
                    AESMode::ECB, 
                    process["key"])
                );
                break;

            case ProcessType::DECRYPT_AES_128_ECB:
                this->processes.push_back(make_shared<EncryptionProcess>(
                    EncryptionOperation::DECRYPT,
                    AESKeySize::AES_128, 
                    AESMode::ECB, 
                    process["key"])
                );
                break;

            case ProcessType::DECRYPT_AES_192_ECB:
                this->processes.push_back(make_shared<EncryptionProcess>(
                    EncryptionOperation::DECRYPT,
                    AESKeySize::AES_192, 
                    AESMode::ECB, 
                    process["key"])
                );
                break;

            case ProcessType::DECRYPT_AES_256_ECB:
                this->processes.push_back(make_shared<EncryptionProcess>(
                    EncryptionOperation::DECRYPT,
                    AESKeySize::AES_256, 
                    AESMode::ECB, 
                    process["key"])
                );
                break;

            case ProcessType::ENCRYPT_AES_128_CBC:
                this->processes.push_back(make_shared<EncryptionProcess>(
                    EncryptionOperation::ENCRYPT,
                    AESKeySize::AES_128, 
                    AESMode::CBC, 
                    process["key"],
                    process["iv"])
                );
                break;

            case ProcessType::ENCRYPT_AES_192_CBC:
                this->processes.push_back(make_shared<EncryptionProcess>(
                    EncryptionOperation::ENCRYPT,
                    AESKeySize::AES_192, 
                    AESMode::CBC, 
                    process["key"],
                    process["iv"])
                );
                break;
                
            case ProcessType::ENCRYPT_AES_256_CBC:
                this->processes.push_back(make_shared<EncryptionProcess>(
                    EncryptionOperation::ENCRYPT,
                    AESKeySize::AES_256, 
                    AESMode::CBC, 
                    process["key"],
                    process["iv"])
                );
                break;

            case ProcessType::DECRYPT_AES_128_CBC:
                this->processes.push_back(make_shared<EncryptionProcess>(
                    EncryptionOperation::DECRYPT,
                    AESKeySize::AES_128, 
                    AESMode::CBC, 
                    process["key"],
                    process["iv"])
                );
                break;

            case ProcessType::DECRYPT_AES_192_CBC:
                this->processes.push_back(make_shared<EncryptionProcess>(
                    EncryptionOperation::DECRYPT,
                    AESKeySize::AES_192, 
                    AESMode::CBC, 
                    process["key"],
                    process["iv"])
                );
                break;

            case ProcessType::DECRYPT_AES_256_CBC:
                this->processes.push_back(make_shared<EncryptionProcess>(
                    EncryptionOperation::DECRYPT,
                    AESKeySize::AES_256, 
                    AESMode::CBC, 
                    process["key"],
                    process["iv"])
                );
                break;

            case ProcessType::COMPRESS_LZ78:
                this->processes.push_back(make_shared<CompressionProcess>(ProcessType::COMPRESS_LZ78));
                break;

            case ProcessType::DECOMPRESS_LZ78:
                this->processes.push_back(make_shared<CompressionProcess>(ProcessType::DECOMPRESS_LZ78));
                break;
        }
    }

};

UploadMessage::~UploadMessage(){};

void UploadMessage::TreatMessage (){

    HttpClient http;
    string tmpPath = RandomString(20);
    string newTmpPath;
    
    if (http.DownloadFile(this->fileUrl, tmpPath) != 0)
    return;

    for (const shared_ptr<Process> p: this->processes){
        newTmpPath = RandomString(20);
        p->Execute(tmpPath, newTmpPath);
        if (remove(tmpPath.c_str()) != 0){
            return;
        }
        tmpPath = newTmpPath;
    }

    rename(tmpPath.c_str(), this->fileID.c_str());
}