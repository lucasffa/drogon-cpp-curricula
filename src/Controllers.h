#pragma once

#include <drogon/HttpController.h>

#include <functional>
#include <memory>

#include "DTOs.h"
#include "Services.h"

class ControllerFactory {
 public:
  static std::shared_ptr<class LanguageController> createLanguageController();
  static std::shared_ptr<class LevelController> createLevelController();
  // Controladores para as outras entidades
};

class LanguageController {
 public:
  LanguageController(std::shared_ptr<LanguageService> service);

  void createLanguage(
      const drogon::HttpRequestPtr &req,
      std::function<void(const drogon::HttpResponsePtr &)> &&callback);
  void getLanguages(
      const drogon::HttpRequestPtr &req,
      std::function<void(const drogon::HttpResponsePtr &)> &&callback);

 private:
  std::shared_ptr<LanguageService> languageService;
};

class LevelController {
 public:
  LevelController(std::shared_ptr<LevelService> service);

  void createLevel(
      const drogon::HttpRequestPtr &req,
      std::function<void(const drogon::HttpResponsePtr &)> &&callback);
  void getLevelsByLanguageId(
      const drogon::HttpRequestPtr &req,
      std::function<void(const drogon::HttpResponsePtr &)> &&callback);

 private:
  std::shared_ptr<LevelService> levelService;
};

// Controladores para as outras entidades já preparados
