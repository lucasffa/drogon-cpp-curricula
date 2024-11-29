#pragma once

#include <drogon/HttpController.h>

#include <memory>

#include "LevelService.h"

using namespace drogon;

class LevelController : public drogon::HttpController<LevelController> {
 public:
  METHOD_LIST_BEGIN
  METHOD_ADD(LevelController::createLevel, "/languages/{1}/levels", Post);
  METHOD_ADD(LevelController::getLevelsByLanguageId, "/languages/{1}/levels",
             Get);
  METHOD_LIST_END

  LevelController();

  void createLevel(const HttpRequestPtr &req,
                   std::function<void(const HttpResponsePtr &)> &&callback,
                   const std::string &languageId);

  void getLevelsByLanguageId(
      const HttpRequestPtr &req,
      std::function<void(const HttpResponsePtr &)> &&callback,
      const std::string &languageId);

 private:
  std::shared_ptr<LevelService> levelService;
};
