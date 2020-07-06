#pragma once

#include <memory>
#include "UploadMessage.h"

#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <string>

using namespace std;
using namespace AmqpClient;
using json = nlohmann::json;

void MainLoop();
void MessageThreadRoutine (std::unique_ptr<UploadMessage> msg);