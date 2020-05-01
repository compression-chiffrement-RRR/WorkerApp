#include "worker_env.h"

#include <string>

using namespace std;

const char* WorkerEnv::Get(const char *var){
    string fullName = WorkerEnv::mainPrefix;
    fullName.append(var);
    return getenv(fullName.c_str());
};

int WorkerEnv::Set(const char *var, const char *value){
    string fullName = WorkerEnv::mainPrefix;
    fullName.append(var);
    return setenv(fullName.c_str(), value, 1);
};