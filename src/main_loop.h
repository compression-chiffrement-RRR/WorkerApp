#pragma once

#include <memory>
#include "UploadMessage.h"

void MainLoop();
void TreatMessageThreadRoutine (std::unique_ptr<UploadMessage> msg);