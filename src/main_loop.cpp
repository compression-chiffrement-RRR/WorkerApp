#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <nlohmann/json.hpp>
#include <stdlib.h>
#include <string>
#include <memory>
#include <iostream>
#include <thread>
#include <vector>

#include "UploadMessage.h"
#include "main_loop.h"
#include "WorkerEnv.h"

using namespace std;
using namespace AmqpClient;
using json = nlohmann::json;

void MessageThreadRoutine (std::unique_ptr<UploadMessage> msg){
    cout << "Thread started for message with fileID = " << msg->fileID << endl;
    msg->Treat();
};

void MainLoop(){

    const char *queue = WorkerEnv::Get(WorkerEnv::rabbitMQQueue);
    string hostWithPort = WorkerEnv::Get(WorkerEnv::rabbitMQHost) + string(":") + WorkerEnv::Get(WorkerEnv::rabbitMQPort);
    cout << "Connecting to AMQP Broker (" << hostWithPort << ")" << endl;

    Channel::ptr_t channel = AmqpClient::Channel::Create(
        WorkerEnv::Get(WorkerEnv::rabbitMQHost), 
        strtoul(WorkerEnv::Get(WorkerEnv::rabbitMQPort), NULL, 10), 
        WorkerEnv::Get(WorkerEnv::rabbitMQUsername), 
        WorkerEnv::Get(WorkerEnv::rabbitMQPassword));

    cout << "Connected to AMQP Broker" << endl;

    cout << "Creating passive queue " << queue << endl;

    // create messages queue
    channel->DeclareQueue(
        queue, 
        false, // passive queue
        true, // durable queue
        false, // exclusive queue
        false // auto-delete queue
    );

    cout << "Queue created." << endl;

    // subscribe as a consumer
    string consumerTag = channel->BasicConsume(
        queue, 
        "",  // consumer tag
        true, // no-local
        true, //. auto-ack
        false // exclusive subscribe
    );

    cout << "Successfully subscribed to queue" << endl;

    // start reading messages.
    while (true){
        
        Envelope::ptr_t envelope;
        channel->BasicConsumeMessage(consumerTag, envelope);

        try {
            cout << "Message received. Parsing JSON..." << endl;
            json body = json::parse(envelope->Message()->Body());
            cout << body << endl;
            thread t (
                MessageThreadRoutine, 
                make_unique<UploadMessage>(body));
            t.detach();
        } 
        
        catch (exception& e){
            cout << e.what() << endl;
        }

    }
};