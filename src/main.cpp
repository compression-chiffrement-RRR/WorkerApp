#include <iostream>
#include <curl/curl.h>

#include "worker_env.h"

using namespace std;

static void CheckEnv();

int main (__attribute__((unused)) int _argc,__attribute__((unused)) char **_argv){
    
    CheckEnv();

    curl_global_init(CURL_GLOBAL_ALL);

    curl_global_cleanup();
    
    return 0;
}

static void CheckEnv(){

    bool ret = true;
    unsigned int i = 0;

    const char *mandatory [] = {
        WorkerEnv::rabbitMQHost,
        WorkerEnv::rabbitMQPort,
        WorkerEnv::rabbitMQQueue,
        WorkerEnv::rabbitMQUsername,
        WorkerEnv::rabbitMQPassword,
        WorkerEnv::apiDomain,
        nullptr
    };

    while (mandatory[i] != nullptr){
        
        if (WorkerEnv::Get(mandatory[i]) == nullptr){
            ret = false;
            cerr << "ERROR: " << WorkerEnv::mainPrefix << mandatory[i] << " is missing from your environnement variables." << endl;
        }
        i++;
    }

    if (ret == false)
    exit(EXIT_FAILURE);
}