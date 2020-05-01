#pragma once

class WorkerEnv {
    
    public:

    static const char* Get(const char *var);
    static int Set(const char *var, const char *value);

    static constexpr const char *mainPrefix = "WORKER_APP_";
    
    static constexpr const char *rabbitMQHost       = "RABBITMQ_HOST";
    static constexpr const char *rabbitMQPort       = "RABBITMQ_PORT";
    static constexpr const char *rabbitMQQueue      = "RABBITMQ_QUEUE";
    static constexpr const char *rabbitMQUsername   = "RABBITMQ_USERNAME";
    static constexpr const char *rabbitMQPassword   = "RABBITMQ_PASSWORD";
    static constexpr const char *apiDomain          = "API_DOMAIN";

};