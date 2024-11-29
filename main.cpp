#include <drogon/drogon.h>

#include "Application.h"

int main() {
  drogon::app().setLogLevel(trantor::Logger::kDebug);

  // Obter a porta do servidor a partir da variável de ambiente
  const char* port_cstr = std::getenv("PORT");
  int port = 8080;  // Porta padrão

  if (port_cstr) {
    try {
      port = std::stoi(port_cstr);  // Converter para inteiro
    } catch (const std::exception& e) {
      std::cerr << "Invalid PORT value. Using default port 8080." << std::endl;
    }
  }

  // Configurar a porta do Drogon
  drogon::app().addListener("0.0.0.0", port);

  Application app;
  app.initialize();
  drogon::app().run();
  return 0;
}
