#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <nlohmann/json.hpp>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>

#include "upload_message.h"
#include "main_loop.h"
#include "worker_env.h"

using namespace std;
using namespace AmqpClient;
using json = nlohmann::json;

void MainLoop(){

    const char *queue = WorkerEnv::Get(WorkerEnv::rabbitMQQueue);
    string hostWithPort = "";

    hostWithPort.append(WorkerEnv::Get(WorkerEnv::rabbitMQHost));
    hostWithPort.append(":");
    hostWithPort.append(WorkerEnv::Get(WorkerEnv::rabbitMQPort));

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

    cout << "Subscribing to queue" << endl;

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

            json body = nlohmann::json::parse(envelope->Message()->Body());

            cout << "Received message : " << body << endl;

            UploadMessage uploadMessage(body);

        } 

        catch (vector<string>& errors){
            for (const string e: errors){
                cout << e << endl;
            }
        }
        
        catch (exception& e){
            cout << e.what() << endl;
        }

    }
}