#pragma once

#include <string>

using namespace std;

class WorkerEnv {
    
    public:

    static const char *Get(const string& var);
    static int Set(const string& var, const string& value);

    static constexpr const char *mainPrefix = "WORKER_APP_";
    
    static constexpr const char *rabbitMQHost       = "RABBITMQ_HOST";
    static constexpr const char *rabbitMQPort       = "RABBITMQ_PORT";
    static constexpr const char *rabbitMQQueue      = "RABBITMQ_QUEUE";
    static constexpr const char *rabbitMQUsername   = "RABBITMQ_USERNAME";
    static constexpr const char *rabbitMQPassword   = "RABBITMQ_PASSWORD";
};