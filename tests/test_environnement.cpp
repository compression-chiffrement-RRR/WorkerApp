#include <gtest/gtest.h>
#include <curl/curl.h>

class Environment : public ::testing::Environment {
 public:
  ~Environment() override {}

  // Override this to define how to set up the environment.
  void SetUp() override {
      curl_global_init(CURL_GLOBAL_ALL);
  }

  // Override this to define how to tear down the environment.
  void TearDown() override {
      curl_global_cleanup();
  }
};