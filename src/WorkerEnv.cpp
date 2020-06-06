#include "WorkerEnv.h"

#include <string>

using namespace std;

const char *WorkerEnv::Get(const string& var){
    string fullName = WorkerEnv::mainPrefix;
    fullName.append(var);
    return getenv(fullName.c_str());
};

int WorkerEnv::Set(const string& var, const string& value){
    string fullName = WorkerEnv::mainPrefix;
    fullName.append(var);
    return setenv(fullName.c_str(), value.c_str(), 1);
};