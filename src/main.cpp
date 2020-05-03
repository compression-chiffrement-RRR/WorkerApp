#include <iostream>
#include <vector>
#include <curl/curl.h>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

#include "worker_env.h"

using namespace std;

static void CheckEnv();

int main (__attribute__((unused)) int argc, __attribute__((unused)) char **argv){
    
    CheckEnv();

    curl_global_init(CURL_GLOBAL_ALL);

    string fullHost = WorkerEnv::Get(WorkerEnv::rabbitMQHost) + ":" + WorkerEnv::Get(WorkerEnv::rabbitMQPort);

    AmqpClient::Channel::ptr_t connection = AmqpClient::Channel::Create(fullHost);

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
        WorkerEnv::rabbitMQPassword,
        WorkerEnv::apiDomain,
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