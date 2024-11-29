#pragma once

#include <memory>
#include <string>

class OpenAIClient {
 public:
  static void initialize();
  static std::shared_ptr<OpenAIClient> getInstance();

  std::string generateContent(const std::string& prompt);

 private:
  OpenAIClient();
  static std::shared_ptr<OpenAIClient> instance;
  std::string apiKey;
  std::string apiUrl;
};
