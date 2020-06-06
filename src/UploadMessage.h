#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "Process.h"

using json = nlohmann::json;
using namespace std;

class UploadMessage {

    private:

    string fileUrl;
    string responseUrl;
    string fileID;
    vector<shared_ptr<Process>> processes;

    public:

    void TreatMessage();

    UploadMessage(json body);
    ~UploadMessage();
};