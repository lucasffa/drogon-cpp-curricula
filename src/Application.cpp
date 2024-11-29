#include "Application.h"

#include <drogon/drogon.h>

#include <cstdlib>
#include <iostream>

#include "Application.h"
#include "OpenAIClient.h"
#include "Routes.h"

void Application::initialize() {
  // Inicialize o OpenAIClient e as Rotas
  OpenAIClient::initialize();
  Routes::init();
}
