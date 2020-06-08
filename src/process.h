#pragma once

#include <vector>
#include <string>
#include <stdint.h>
#include <string>

#include <stdexcept>

using namespace std;

enum ProcessType {
    ENCRYPT_AES_128_ECB,
    ENCRYPT_AES_192_ECB,
    ENCRYPT_AES_256_ECB,

    DECRYPT_AES_128_ECB,
    DECRYPT_AES_192_ECB,
    DECRYPT_AES_256_ECB,

    ENCRYPT_AES_128_CBC,
    ENCRYPT_AES_192_CBC,
    ENCRYPT_AES_256_CBC,

    DECRYPT_AES_128_CBC,
    DECRYPT_AES_192_CBC,
    DECRYPT_AES_256_CBC,

    COMPRESS_LZ78,
    DECOMPRESS_LZ78
};

class Process {
    
    public:
    
    static ProcessType DeserializeProcessType (string processName){

        // ENCRYPT ECB
        if (processName == "ENCRYPT_AES_128_ECB")
            return ProcessType::ENCRYPT_AES_128_ECB;

        if (processName == "ENCRYPT_AES_192_ECB")
            return ProcessType::ENCRYPT_AES_192_ECB;

        if (processName == "ENCRYPT_AES_256_ECB")
            return ProcessType::ENCRYPT_AES_256_ECB;

        // DECRYPT ECB
        if (processName == "DECRYPT_AES_128_ECB")
            return ProcessType::DECRYPT_AES_128_ECB;

        if (processName == "DECRYPT_AES_192_ECB")
            return ProcessType::DECRYPT_AES_192_ECB;

        if (processName == "DECRYPT_AES_256_ECB")
            return ProcessType::DECRYPT_AES_256_ECB;

        // ENCRYPT CBC
        if (processName == "ENCRYPT_AES_128_CBC")
            return ProcessType::ENCRYPT_AES_128_CBC;

        if (processName == "ENCRYPT_AES_192_CBC")
            return ProcessType::ENCRYPT_AES_192_CBC;

        if (processName == "ENCRYPT_AES_256_CBC")
            return ProcessType::ENCRYPT_AES_256_CBC;

        // DECRYPT CBC
        if (processName == "DECRYPT_AES_128_CBC")
            return ProcessType::DECRYPT_AES_128_CBC;

        if (processName == "DECRYPT_AES_192_CBC")
            return ProcessType::DECRYPT_AES_192_CBC;

        if (processName == "DECRYPT_AES_256_CBC")
            return ProcessType::DECRYPT_AES_256_CBC;

        //LZ78
        if (processName == "COMPRESS_LZ78")
            return ProcessType::COMPRESS_LZ78;

        if (processName == "DECOMPRESS_LZ78")
            return ProcessType::DECOMPRESS_LZ78;

        throw runtime_error(string("Unknown process name."));
    };

    virtual bool Execute(string& filePath, string& outputPath) = 0;

};
