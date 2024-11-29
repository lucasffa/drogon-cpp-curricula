#include "Application.h"

#include "OpenAIClient.h"
#include "Routes.h"

void Application::initialize() {
  OpenAIClient::initialize();
  Routes::init();
}
