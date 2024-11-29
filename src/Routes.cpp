#include "Routes.h"

#include <drogon/drogon.h>

#include "Controllers.h"

void Routes::init() {
  auto languageController = ControllerFactory::createLanguageController();
  auto levelController = ControllerFactory::createLevelController();

  // Rotas para Language
  drogon::app().registerHandler(
      "/languages",
      [languageController](
          const drogon::HttpRequestPtr &req,
          std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
        if (req->getMethod() == drogon::Post) {
          languageController->createLanguage(req, std::move(callback));
        } else if (req->getMethod() == drogon::Get) {
          languageController->getLanguages(req, std::move(callback));
        } else {
          auto resp = drogon::HttpResponse::newHttpResponse();
          resp->setStatusCode(drogon::k405MethodNotAllowed);
          callback(resp);
        }
      });

  // Rotas para Level
  drogon::app().registerHandler(
      "/languages/{languageId}/levels",
      [levelController](
          const drogon::HttpRequestPtr &req,
          std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
        if (req->getMethod() == drogon::Post) {
          levelController->createLevel(req, std::move(callback));
        } else if (req->getMethod() == drogon::Get) {
          levelController->getLevelsByLanguageId(req, std::move(callback));
        } else {
          auto resp = drogon::HttpResponse::newHttpResponse();
          resp->setStatusCode(drogon::k405MethodNotAllowed);
          callback(resp);
        }
      });

  // Rotas para as outras entidades
}
