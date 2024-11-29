#include "LanguageController.h"

#include <json/json.h>

#include "Validators.h"

LanguageController::LanguageController()
    : languageService(ServiceFactory::createLanguageService()) {}

void LanguageController::createLanguage(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  auto json = req->getJsonObject();
  if (!json) {
    Json::Value errorResponse;
    errorResponse["error"] = "Invalid JSON";
    auto resp = HttpResponse::newHttpJsonResponse(errorResponse);
    resp->setStatusCode(k400BadRequest);
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
    auto resp = HttpResponse::newHttpJsonResponse(errorResponse);
    resp->setStatusCode(k400BadRequest);
    callback(resp);
    return;
  }

  languageService->createLanguage(
      dto, [callback](const std::optional<Language> &language,
                      const std::optional<std::string> &error) {
        if (error) {
          Json::Value errorResponse;
          errorResponse["error"] = *error;
          auto resp = HttpResponse::newHttpJsonResponse(errorResponse);
          resp->setStatusCode(k500InternalServerError);
          callback(resp);
        } else {
          Json::Value successResponse;
          successResponse["message"] = "Language created successfully";
          successResponse["id"] = language->id;
          auto resp = HttpResponse::newHttpJsonResponse(successResponse);
          resp->setStatusCode(k201Created);
          callback(resp);
        }
      });
}

void LanguageController::getLanguages(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  languageService->getLanguages(
      [callback](const std::vector<Language> &languages,
                 const std::optional<std::string> &error) {
        if (error) {
          Json::Value errorResponse;
          errorResponse["error"] = *error;
          auto resp = HttpResponse::newHttpJsonResponse(errorResponse);
          resp->setStatusCode(k500InternalServerError);
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
          auto resp = HttpResponse::newHttpJsonResponse(jsonLanguages);
          resp->setStatusCode(k200OK);
          callback(resp);
        }
      });
}

void LanguageController::getLanguageById(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    const std::string &languageId) {
  languageService->getLanguageById(
      languageId, [callback](const std::optional<Language> &language,
                             const std::optional<std::string> &error) {
        if (error) {
          Json::Value errorResponse;
          errorResponse["error"] = *error;
          auto resp = HttpResponse::newHttpJsonResponse(errorResponse);
          resp->setStatusCode(k500InternalServerError);
          callback(resp);
        } else if (!language) {
          Json::Value errorResponse;
          errorResponse["error"] = "Language not found";
          auto resp = HttpResponse::newHttpJsonResponse(errorResponse);
          resp->setStatusCode(k404NotFound);
          callback(resp);
        } else {
          Json::Value jsonLang;
          jsonLang["id"] = language->id;
          jsonLang["name"] = language->name;
          jsonLang["description"] = language->description;

          Json::Value countries(Json::arrayValue);
          for (const auto &country : language->spokenInCountries) {
            countries.append(country);
          }
          jsonLang["spokenInCountries"] = countries;

          auto resp = HttpResponse::newHttpJsonResponse(jsonLang);
          resp->setStatusCode(k200OK);
          callback(resp);
        }
      });
}
