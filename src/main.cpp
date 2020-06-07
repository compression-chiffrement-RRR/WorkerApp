#include <iostream>
#include <vector>
#include <curl/curl.h>

#include "WorkerEnv.h"
#include "main_loop.h"

using namespace std;

static void CheckEnv();

int main (__attribute__((unused)) int argc, __attribute__((unused)) char **argv){
    
    CheckEnv();

    curl_global_init(CURL_GLOBAL_ALL);

    MainLoop();

    curl_global_cleanup();

    return 0;
}

static void CheckEnv(){

    bool failure = false;

    vector<string> mandatory {
        WorkerEnv::rabbitMQHost,
        WorkerEnv::rabbitMQPort,
        WorkerEnv::rabbitMQQueue,
        WorkerEnv::rabbitMQUsername,
        WorkerEnv::rabbitMQPassword
    };

    for (string s : mandatory){
        if (WorkerEnv::Get(s) == nullptr){
            failure = true;
            cerr << "ERROR: " << WorkerEnv::mainPrefix << s << " is missing from your environnement variables." << endl;
        }
    }

    if (failure)
        exit(EXIT_FAILURE);
}