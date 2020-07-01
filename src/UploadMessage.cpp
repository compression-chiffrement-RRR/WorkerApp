#include <nlohmann/json.hpp>
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include <random>
#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "AES.h"
#include "HttpClient.h"
#include "UploadMessage.h"
#include "Process.h"
#include "CompressionProcess.h"
#include "EncryptionProcess.h"

using json = nlohmann::json;
using namespace std;

static string RandomTmpFilename(size_t length);

static string RandomTmpFilename(size_t length)
{
    const string allowedChars = "AZERTYUIOPQSDFGHJKLMWXCVBN1234567890";

    random_device device;
    mt19937 generator(device());
    uniform_int_distribution<> distribution(0, allowedChars.size() - 1);

    string out;

    for (size_t i = 0; i < length; ++i)
    {
        out += allowedChars[distribution(generator)];
    }

    return string(filesystem::temp_directory_path()) + string("/") + out;
}

UploadMessage::UploadMessage (json& body){

    if (!body["responseUrl"].is_string()){
        string error = "\"responseUrl\" must be a string.";
        throw runtime_error(error);
    }

    this->responseUrl = body["responseUrl"];

    if (!body["fileUrl"].is_string()){
        string error = "\"fileUrl\" must be a string.";
        this->Fail(error);
        throw runtime_error(error);
    }

    this->fileUrl = body["fileUrl"];

    if (!body["uploadUrl"].is_string()){
        string error = "\"uploadUrl\" must be a string.";
        this->Fail(error);
        throw runtime_error(error);
    }

    this->uploadUrl = body["uploadUrl"];

    if (!body["fileID"].is_string()){
        string error = "\"fileID\" must be a string.";
        this->Fail(error);
        throw runtime_error(error);
    }

    this->fileID = body["fileID"];

    if (!body["processes"].is_array()){
        string error = "\"processes\" must be an array of process objects.";
        this->Fail(error);
        throw runtime_error(error);
    }

    for (const json process: body["processes"]){

        ProcessType type;

        if (!process["type"].is_string()){
            string error = "\"type\" must be a string representing a valid process type.";
            this->Fail(error);
            throw runtime_error(error);
        }
        try {
            type = Process::DeserializeProcessType(process["type"]);
        } catch (exception& e){
            string error = string(process["type"]) + string(" is not a valid process type: ") + string(e.what());
            this->Fail(error);
            throw runtime_error(error);
        }

        try {

            switch (type){

                case ProcessType::ENCRYPT_AES_128_ECB:
                    this->processes.push_back(make_unique<EncryptionProcess>(
                        EncryptionOperation::ENCRYPT,
                        AESKeySize::AES_128, 
                        AESMode::ECB, 
                        process["key"])
                    );
                    break;

                case ProcessType::ENCRYPT_AES_192_ECB:
                    this->processes.push_back(make_unique<EncryptionProcess>(
                        EncryptionOperation::ENCRYPT,
                        AESKeySize::AES_192, 
                        AESMode::ECB, 
                        process["key"])
                    );
                    break;
                    
                case ProcessType::ENCRYPT_AES_256_ECB:
                    this->processes.push_back(make_unique<EncryptionProcess>(
                        EncryptionOperation::ENCRYPT,
                        AESKeySize::AES_256, 
                        AESMode::ECB, 
                        process["key"])
                    );
                    break;

                case ProcessType::DECRYPT_AES_128_ECB:
                    this->processes.push_back(make_unique<EncryptionProcess>(
                        EncryptionOperation::DECRYPT,
                        AESKeySize::AES_128, 
                        AESMode::ECB, 
                        process["key"])
                    );
                    break;

                case ProcessType::DECRYPT_AES_192_ECB:
                    this->processes.push_back(make_unique<EncryptionProcess>(
                        EncryptionOperation::DECRYPT,
                        AESKeySize::AES_192, 
                        AESMode::ECB, 
                        process["key"])
                    );
                    break;

                case ProcessType::DECRYPT_AES_256_ECB:
                    this->processes.push_back(make_unique<EncryptionProcess>(
                        EncryptionOperation::DECRYPT,
                        AESKeySize::AES_256, 
                        AESMode::ECB, 
                        process["key"])
                    );
                    break;

                case ProcessType::ENCRYPT_AES_128_CBC:
                    this->processes.push_back(make_unique<EncryptionProcess>(
                        EncryptionOperation::ENCRYPT,
                        AESKeySize::AES_128, 
                        AESMode::CBC, 
                        process["key"],
                        process["iv"])
                    );
                    break;

                case ProcessType::ENCRYPT_AES_192_CBC:
                    this->processes.push_back(make_unique<EncryptionProcess>(
                        EncryptionOperation::ENCRYPT,
                        AESKeySize::AES_192, 
                        AESMode::CBC, 
                        process["key"],
                        process["iv"])
                    );
                    break;
                    
                case ProcessType::ENCRYPT_AES_256_CBC:
                    this->processes.push_back(make_unique<EncryptionProcess>(
                        EncryptionOperation::ENCRYPT,
                        AESKeySize::AES_256, 
                        AESMode::CBC, 
                        process["key"],
                        process["iv"])
                    );
                    break;

                case ProcessType::DECRYPT_AES_128_CBC:
                    this->processes.push_back(make_unique<EncryptionProcess>(
                        EncryptionOperation::DECRYPT,
                        AESKeySize::AES_128, 
                        AESMode::CBC, 
                        process["key"],
                        process["iv"])
                    );
                    break;

                case ProcessType::DECRYPT_AES_192_CBC:
                    this->processes.push_back(make_unique<EncryptionProcess>(
                        EncryptionOperation::DECRYPT,
                        AESKeySize::AES_192, 
                        AESMode::CBC, 
                        process["key"],
                        process["iv"])
                    );
                    break;

                case ProcessType::DECRYPT_AES_256_CBC:
                    this->processes.push_back(make_unique<EncryptionProcess>(
                        EncryptionOperation::DECRYPT,
                        AESKeySize::AES_256, 
                        AESMode::CBC, 
                        process["key"],
                        process["iv"])
                    );
                    break;

                case ProcessType::COMPRESS_HUFFMAN:
                    this->processes.push_back(make_unique<CompressionProcess>(
                        CompressionOperation::OPERATION_COMPRESS_HUFFMAN));
                    break;

                case ProcessType::DECOMPRESS_HUFFMAN:
                    this->processes.push_back(make_unique<CompressionProcess>(
                        CompressionOperation::OPERATION_DECOMPRESS_HUFFMAN));
                    break;
            }

        } catch (exception& e){
            string error = string("Missing or invalid parameter: ") + string(e.what());
            this->Fail(error);
            throw runtime_error(error);
        }
    }

};

UploadMessage::~UploadMessage(){};

bool UploadMessage::Treat (){

    HttpClient http;
    string tmpPath = RandomTmpFilename(20), newTmpPath;
    
    if (http.DownloadFile(this->fileUrl, tmpPath) != true){
        string error = string("Failed to download ") + this->fileUrl;
        cout << error << endl;
        this->Fail(error);
        return false;
    }

    for (size_t i = 0; i < processes.size(); i ++){
        
        newTmpPath = RandomTmpFilename(20);
        
        if (!processes[i]->Execute(tmpPath, newTmpPath)){
            string error = string("Failed to process file ") + tmpPath;
            cout << error << endl;
            this->Fail(error);
            return false;
        }

        if (remove(tmpPath.c_str()) != 0){
            string error = string("Failed to remove temp file ") + tmpPath;
            cout << error << endl;
            this->Fail(error);
            return false;
        }

        tmpPath = newTmpPath;
    }


    if (http.UploadFile(this->uploadUrl, tmpPath) != true){
        string error = string("Couldn't upload processed file to ") + this->uploadUrl;
        cout << error << endl;
        this->Fail(error);
        return false;
    }

    if (remove(tmpPath.c_str()) != 0){
        string error = string("Failed to remove temp file ") + tmpPath;
        cout << error << endl;
    }

    this->Success();

    cout << "Successfully processed file " << this->fileID << endl;

    return true;
}

void UploadMessage::Success(){

    json j = {
        { "success", true },
        { "fileID", this->fileID }
    };

    if (!this->SendResponse(j)){
        cout << "Failed to send success response for ";
        cout << this->fileID << " (" << this->responseUrl << ")" << endl;
    }
}

void UploadMessage::Fail(string& error){

    json j = {
        { "success", false },
        { "error", error },
        { "fileID", this->fileID }
    };

    if (!this->SendResponse(j)){
        cout << "Failed to send failure response for ";
        cout << this->fileID << " (" << this->responseUrl << ").";
        cout << " Error message: " << error << endl;   
    }
}



bool UploadMessage::SendResponse (json& body){
    HttpClient client;
    return client.SendPostRequest(this->responseUrl, body);
}