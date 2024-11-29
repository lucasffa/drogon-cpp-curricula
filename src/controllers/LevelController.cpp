#include "LevelController.h"

#include <json/json.h>

#include "Validators.h"

LevelController::LevelController()
    : levelService(ServiceFactory::createLevelService()) {}

void LevelController::createLevel(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    const std::string &languageId) {
  auto json = req->getJsonObject();
  if (!json) {
    Json::Value errorResponse;
    errorResponse["error"] = "Invalid JSON";
    auto resp = HttpResponse::newHttpJsonResponse(errorResponse);
    resp->setStatusCode(k400BadRequest);
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
    auto resp = HttpResponse::newHttpJsonResponse(errorResponse);
    resp->setStatusCode(k400BadRequest);
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
          auto resp = HttpResponse::newHttpJsonResponse(errorResponse);
          resp->setStatusCode(k500InternalServerError);
          callback(resp);
        } else {
          Json::Value successResponse;
          successResponse["message"] = "Level created successfully";
          successResponse["id"] = level->id;
          auto resp = HttpResponse::newHttpJsonResponse(successResponse);
          resp->setStatusCode(k201Created);
          callback(resp);
        }
      });
}

void LevelController::getLevelsByLanguageId(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    const std::string &languageId) {
  levelService->getLevelsByLanguageId(
      languageId, [callback](const std::vector<Level> &levels,
                             const std::optional<std::string> &error) {
        if (error) {
          Json::Value errorResponse;
          errorResponse["error"] = *error;
          auto resp = HttpResponse::newHttpJsonResponse(errorResponse);
          resp->setStatusCode(k500InternalServerError);
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
          auto resp = HttpResponse::newHttpJsonResponse(jsonLevels);
          resp->setStatusCode(k200OK);
          callback(resp);
        }
      });
}
