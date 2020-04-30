#include <iostream>

#include <SimpleAmqpClient/SimpleAmqpClient.h>

using namespace std;

int main (__attribute__((unused)) int _argc,__attribute__((unused)) char **_argv){

    

    AmqpClient::Channel::ptr_t connection = AmqpClient::Channel::Create("localhost");

    return 0;
}