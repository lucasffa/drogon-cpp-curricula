#include "Controllers.h"

#include <drogon/drogon.h>

#include "Validators.h"

std::shared_ptr<LanguageController>
ControllerFactory::createLanguageController() {
  auto service = ServiceFactory::createLanguageService();
  return std::make_shared<LanguageController>(service);
}

std::shared_ptr<LevelController> ControllerFactory::createLevelController() {
  auto service = ServiceFactory::createLevelService();
  return std::make_shared<LevelController>(service);
}

// Implementação dos construtores

LanguageController::LanguageController(std::shared_ptr<LanguageService> service)
    : languageService(service) {}

LevelController::LevelController(std::shared_ptr<LevelService> service)
    : levelService(service) {}

// Implementação dos métodos

void LanguageController::createLanguage(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
  auto json = req->getJsonObject();
  if (!json) {
    Json::Value errorResponse;
    errorResponse["error"] = "Invalid JSON";
    auto resp = drogon::HttpResponse::newHttpJsonResponse(errorResponse);
    resp->setStatusCode(drogon::k400BadRequest);
    callback(resp);
    return;
  }

  LanguageDTO dto;
  dto.name = (*json)["name"].asString();
  dto.description = (*json).get("description", "").asString();
  for (const auto &country : (*json)["spokenInCountries"]) {
    dto.spokenInCountries.push_back(country.asString());
  }

  auto validationError = Validators::validateLanguageDTO(dto);
  if (validationError) {
    Json::Value errorResponse;
    errorResponse["error"] = *validationError;
    auto resp = drogon::HttpResponse::newHttpJsonResponse(errorResponse);
    resp->setStatusCode(drogon::k400BadRequest);
    callback(resp);
    return;
  }

  languageService->createLanguage(
      dto, [callback](const std::optional<Language> &language,
                      const std::optional<std::string> &error) {
        if (error) {
          Json::Value errorResponse;
          errorResponse["error"] = *error;
          auto resp = drogon::HttpResponse::newHttpJsonResponse(errorResponse);
          resp->setStatusCode(drogon::k500InternalServerError);
          callback(resp);
        } else {
          Json::Value successResponse;
          successResponse["message"] = "Language created successfully";
          auto resp =
              drogon::HttpResponse::newHttpJsonResponse(successResponse);
          resp->setStatusCode(drogon::k201Created);
          callback(resp);
        }
      });
}

void LanguageController::getLanguages(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
  languageService->getLanguages(
      [callback](const std::vector<Language> &languages,
                 const std::optional<std::string> &error) {
        if (error) {
          Json::Value errorResponse;
          errorResponse["error"] =
              *error;  // Cria um JSON válido com a mensagem de erro
          auto resp = drogon::HttpResponse::newHttpJsonResponse(errorResponse);
          resp->setStatusCode(drogon::k500InternalServerError);
          callback(resp);
        } else {
          Json::Value jsonLanguages(Json::arrayValue);
          for (const auto &lang : languages) {
            Json::Value jsonLang;
            jsonLang["id"] = lang.id;
            jsonLang["name"] = lang.name;
            jsonLang["description"] = lang.description;

            Json::Value countries(Json::arrayValue);
            for (const auto &country : lang.spokenInCountries) {
              countries.append(country);
            }
            jsonLang["spokenInCountries"] = countries;

            jsonLanguages.append(jsonLang);
          }
          auto resp = drogon::HttpResponse::newHttpJsonResponse(jsonLanguages);
          resp->setStatusCode(drogon::k200OK);
          callback(resp);
        }
      });
}

void LevelController::createLevel(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
  auto languageId = req->getParameter("languageId");
  if (languageId.empty()) {
    Json::Value errorResponse;
    errorResponse["error"] = "languageId is required";
    auto resp = drogon::HttpResponse::newHttpJsonResponse(errorResponse);
    resp->setStatusCode(drogon::k400BadRequest);
    callback(resp);
    return;
  }

  auto json = req->getJsonObject();
  if (!json) {
    Json::Value errorResponse;
    errorResponse["error"] = "Invalid JSON";
    auto resp = drogon::HttpResponse::newHttpJsonResponse(errorResponse);
    resp->setStatusCode(drogon::k400BadRequest);
    callback(resp);
    return;
  }

  LevelDTO dto;
  dto.name = static_cast<LevelName>((*json)["name"].asInt());
  dto.description = (*json).get("description", "").asString();

  auto validationError = Validators::validateLevelDTO(dto);
  if (validationError) {
    Json::Value errorResponse;
    errorResponse["error"] = *validationError;
    auto resp = drogon::HttpResponse::newHttpJsonResponse(errorResponse);
    resp->setStatusCode(drogon::k400BadRequest);
    callback(resp);
    return;
  }

  levelService->createLevel(
      dto, languageId,
      [callback](const std::optional<Level> &level,
                 const std::optional<std::string> &error) {
        if (error) {
          Json::Value errorResponse;
          errorResponse["error"] = *error;
          auto resp = drogon::HttpResponse::newHttpJsonResponse(errorResponse);
          resp->setStatusCode(drogon::k500InternalServerError);
          callback(resp);
        } else {
          Json::Value successResponse;
          successResponse["message"] = "Level created successfully";
          auto resp =
              drogon::HttpResponse::newHttpJsonResponse(successResponse);
          resp->setStatusCode(drogon::k201Created);
          callback(resp);
        }
      });
}

void LevelController::getLevelsByLanguageId(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
  auto languageId = req->getParameter("languageId");
  if (languageId.empty()) {
    Json::Value errorResponse;
    errorResponse["error"] = "languageId is required";
    auto resp = drogon::HttpResponse::newHttpJsonResponse(errorResponse);
    resp->setStatusCode(drogon::k400BadRequest);
    callback(resp);
    return;
  }

  levelService->getLevelsByLanguageId(
      languageId, [callback](const std::vector<Level> &levels,
                             const std::optional<std::string> &error) {
        if (error) {
          Json::Value errorResponse;
          errorResponse["error"] = *error;
          auto resp = drogon::HttpResponse::newHttpJsonResponse(errorResponse);
          resp->setStatusCode(drogon::k500InternalServerError);
          callback(resp);
        } else {
          Json::Value jsonLevels(Json::arrayValue);
          for (const auto &level : levels) {
            Json::Value jsonLevel;
            jsonLevel["id"] = level.id;
            jsonLevel["languageId"] = level.languageId;
            jsonLevel["name"] = static_cast<int>(level.name);
            jsonLevel["description"] = level.description;
            jsonLevels.append(jsonLevel);
          }
          auto resp = drogon::HttpResponse::newHttpJsonResponse(jsonLevels);
          resp->setStatusCode(drogon::k200OK);
          callback(resp);
        }
      });
}
