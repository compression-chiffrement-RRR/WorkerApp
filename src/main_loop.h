#pragma once

#include <memory>
#include "UploadMessage.h"

void MainLoop();
void TreatMessageThreadRoutine (shared_ptr<UploadMessage> msg);