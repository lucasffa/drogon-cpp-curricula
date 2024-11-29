#pragma once

#include <drogon/HttpController.h>

#include <memory>

#include "LanguageService.h"

using namespace drogon;

class LanguageController : public drogon::HttpController<LanguageController> {
 public:
  METHOD_LIST_BEGIN
  METHOD_ADD(LanguageController::createLanguage, "/languages", Post);
  METHOD_ADD(LanguageController::getLanguages, "/languages", Get);
  METHOD_ADD(LanguageController::getLanguageById, "/languages/{1}", Get);
  METHOD_LIST_END

  LanguageController();

  void createLanguage(const HttpRequestPtr &req,
                      std::function<void(const HttpResponsePtr &)> &&callback);

  void getLanguages(const HttpRequestPtr &req,
                    std::function<void(const HttpResponsePtr &)> &&callback);

  void getLanguageById(const HttpRequestPtr &req,
                       std::function<void(const HttpResponsePtr &)> &&callback,
                       const std::string &languageId);

 private:
  std::shared_ptr<LanguageService> languageService;
};
