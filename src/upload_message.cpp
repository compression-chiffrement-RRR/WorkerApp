#include "upload_message.h"
#include <nlohmann/json.hpp>
#include <vector>
#include <iostream>

using json = nlohmann::json;
using namespace std;

enum ParameterType {
    STRING,
    BYTE_ARRAY,
};

typedef struct ProcessParameter {
    const string key;
    const ParameterType type;
} ProcessParameter;

typedef struct ProcessType {
    const string name;
    const vector<ParameterType> params;
} ProcessType;

UploadMessage::UploadMessage (json body){

    const vector<ProcessType> processTypes = {

        // AES in ECB mode
        { "ENCRYPT_AES_128_ECB", { { "key", ParameterType::BYTE_ARRAY } } },
        { "ENCRYPT_AES_192_ECB", { { "key", ParameterType::BYTE_ARRAY } } },
        { "ENCRYPT_AES_256_ECB", { { "key", ParameterType::BYTE_ARRAY } } },
        { "DECRYPT_AES_128_ECB", { { "key", ParameterType::BYTE_ARRAY } } },
        { "DECRYPT_AES_192_ECB", { { "key", ParameterType::BYTE_ARRAY } } },
        { "DECRYPT_AES_256_ECB", { { "key", ParameterType::BYTE_ARRAY } } },

        // AES in CBC mode
        { "ENCRYPT_AES_128_CBC", { { "key", ParameterType::BYTE_ARRAY }, { "iv", ParameterType::BYTE_ARRAY } } },
        { "ENCRYPT_AES_192_CBC", { { "key", ParameterType::BYTE_ARRAY }, { "iv", ParameterType::BYTE_ARRAY } } },
        { "ENCRYPT_AES_256_CBC", { { "key", ParameterType::BYTE_ARRAY }, { "iv", ParameterType::BYTE_ARRAY } } },
        { "DECRYPT_AES_128_CBC", { { "key", ParameterType::BYTE_ARRAY }, { "iv", ParameterType::BYTE_ARRAY } } },
        { "DECRYPT_AES_192_CBC", { { "key", ParameterType::BYTE_ARRAY }, { "iv", ParameterType::BYTE_ARRAY } } },
        { "DECRYPT_AES_256_CBC", { { "key", ParameterType::BYTE_ARRAY }, { "iv", ParameterType::BYTE_ARRAY } } },

        // LZ78
        { "COMPRESS_LZ78", {} },
        { "DECOMPRESS_LZ78", {} },
        
    };

    vector<string> errors;

    if (!body["fileUrl"].is_string()){
        errors.push_back("\"fileUrl\" must be a string representing a file URL");
    }
    
    this->fileUrl = body["fileUrl"];

    if (!body["processes"].is_array()){
        errors.push_back("\"processes\" must be an array representing processes");
    }

    for (const json process: body["processes"]){

        if (!process["name"].is_string()){
            errors.push_back("\"name\" must be a string representing a valid process type name");
        }

        bool found = false;

        // find if process type exist

        if (!found){
            string error = process["name"];
            error.append(" is not a valid process type name");
            errors.push_back(error);
        }

    }

    if (errors.size() != 0){
        throw errors;
    }
};

UploadMessage::~UploadMessage(){};