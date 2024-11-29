#include "Services.h"

std::shared_ptr<LanguageService> ServiceFactory::createLanguageService() {
  auto db = Database::getInstance();
  auto aiClient = OpenAIClient::getInstance();
  return std::make_shared<LanguageService>(db, aiClient);
}

std::shared_ptr<LevelService> ServiceFactory::createLevelService() {
  auto db = Database::getInstance();
  auto aiClient = OpenAIClient::getInstance();
  return std::make_shared<LevelService>(db, aiClient);
}

// Implementação dos construtores

LanguageService::LanguageService(std::shared_ptr<Database> db,
                                 std::shared_ptr<OpenAIClient> aiClient)
    : database(db), openAIClient(aiClient) {}

LevelService::LevelService(std::shared_ptr<Database> db,
                           std::shared_ptr<OpenAIClient> aiClient)
    : database(db), openAIClient(aiClient) {}

// Implementação dos métodos

void LanguageService::createLanguage(
    const LanguageDTO& dto,
    std::function<void(const std::optional<Language>& language,
                       const std::optional<std::string>& error)>
        callback) {
  Language lang;
  lang.name = dto.name;
  lang.spokenInCountries = dto.spokenInCountries;

  // Gera a descrição se não for fornecida
  if (dto.description.empty()) {
    std::string prompt =
        "Provide a detailed description for the language: " + dto.name;
    lang.description = openAIClient->generateContent(prompt);
  } else {
    lang.description = dto.description;
  }

  // Insere no banco de dados
  database->insertLanguage(
      lang, [callback, lang](const std::optional<std::string>& error) {
        if (error) {
          callback(std::nullopt, error);
        } else {
          callback(lang, std::nullopt);
        }
      });
}

void LanguageService::getLanguages(
    std::function<void(const std::vector<Language>& languages,
                       const std::optional<std::string>& error)>
        callback) {
  database->getLanguages(callback);
}

void LevelService::createLevel(
    const LevelDTO& dto, const std::string& languageId,
    std::function<void(const std::optional<Level>& level,
                       const std::optional<std::string>& error)>
        callback) {
  Level level;
  level.languageId = languageId;
  level.name = dto.name;

  // Gera a descrição se não for fornecida
  if (dto.description.empty()) {
    std::string levelNameStr = "";  // Converter LevelName para string
    switch (dto.name) {
      case LevelName::A1:
        levelNameStr = "A1";
        break;
      case LevelName::A2:
        levelNameStr = "A2";
        break;
      case LevelName::B1:
        levelNameStr = "B1";
        break;
      case LevelName::B2:
        levelNameStr = "B2";
        break;
      case LevelName::C1:
        levelNameStr = "C1";
        break;
      case LevelName::C2:
        levelNameStr = "C2";
        break;
    }
    std::string prompt = "Provide a detailed description for level " +
                         levelNameStr + " in language.";
    level.description = openAIClient->generateContent(prompt);
  } else {
    level.description = dto.description;
  }

  // Insere no banco de dados
  database->insertLevel(
      level, [callback, level](const std::optional<std::string>& error) {
        if (error) {
          callback(std::nullopt, error);
        } else {
          callback(level, std::nullopt);
        }
      });
}

void LevelService::getLevelsByLanguageId(
    const std::string& languageId,
    std::function<void(const std::vector<Level>& levels,
                       const std::optional<std::string>& error)>
        callback) {
  database->getLevelsByLanguageId(languageId, callback);
}

// Implementação dos serviços para as outras entidades
