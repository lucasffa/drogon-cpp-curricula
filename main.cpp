#include <drogon/drogon.h>

#include "Application.h"

int main() {
  Application app;
  app.initialize();
  drogon::app().run();
  return 0;
}
