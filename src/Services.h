#pragma once

#include <functional>
#include <memory>

#include "DTOs.h"
#include "Database.h"
#include "Models.h"
#include "OpenAIClient.h"

class ServiceFactory {
 public:
  static std::shared_ptr<class LanguageService> createLanguageService();
  static std::shared_ptr<class LevelService> createLevelService();
  // Serviços para as outras entidades
};

class LanguageService {
 public:
  LanguageService(std::shared_ptr<Database> db,
                  std::shared_ptr<OpenAIClient> aiClient);

  void createLanguage(
      const LanguageDTO& dto,
      std::function<void(const std::optional<Language>& language,
                         const std::optional<std::string>& error)>
          callback);
  void getLanguages(std::function<void(const std::vector<Language>& languages,
                                       const std::optional<std::string>& error)>
                        callback);

 private:
  std::shared_ptr<Database> database;
  std::shared_ptr<OpenAIClient> openAIClient;
};

class LevelService {
 public:
  LevelService(std::shared_ptr<Database> db,
               std::shared_ptr<OpenAIClient> aiClient);

  void createLevel(const LevelDTO& dto, const std::string& languageId,
                   std::function<void(const std::optional<Level>& level,
                                      const std::optional<std::string>& error)>
                       callback);
  void getLevelsByLanguageId(
      const std::string& languageId,
      std::function<void(const std::vector<Level>& levels,
                         const std::optional<std::string>& error)>
          callback);

 private:
  std::shared_ptr<Database> database;
  std::shared_ptr<OpenAIClient> openAIClient;
};

// Serviços para as outras entidades já preparados
