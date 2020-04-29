#include <SimpleAmqpClient/SimpleAmqpClient.h>

int main (__attribute__((unused)) int _argc,__attribute__((unused)) char **_argv){

    AmqpClient::Channel::ptr_t connection = AmqpClient::Channel::Create("localhost");

    return 0;
}