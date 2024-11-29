#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>

#include "Models.h"

class Database {
 public:
  virtual ~Database() = default;

  // Métodos abstratos para operações com Language
  virtual void insertLanguage(
      const Language& language,
      std::function<void(const std::optional<std::string>& error)>
          callback) = 0;
  virtual void getLanguages(
      std::function<void(const std::vector<Language>& languages,
                         const std::optional<std::string>& error)>
          callback) = 0;

  // Métodos abstratos para operações com Level
  virtual void insertLevel(
      const Level& level,
      std::function<void(const std::optional<std::string>& error)>
          callback) = 0;
  virtual void getLevelsByLanguageId(
      const std::string& languageId,
      std::function<void(const std::vector<Level>& levels,
                         const std::optional<std::string>& error)>
          callback) = 0;

  // Métodos para outras entidades

  // Método estático para obter a instância da implementação
  static std::shared_ptr<Database> getInstance();
};

// Implementação concreta para MongoDB
class MongoDatabase : public Database {
 public:
  MongoDatabase();
  void insertLanguage(
      const Language& language,
      std::function<void(const std::optional<std::string>& error)> callback)
      override;
  void getLanguages(std::function<void(const std::vector<Language>& languages,
                                       const std::optional<std::string>& error)>
                        callback) override;

  void insertLevel(const Level& level,
                   std::function<void(const std::optional<std::string>& error)>
                       callback) override;
  void getLevelsByLanguageId(
      const std::string& languageId,
      std::function<void(const std::vector<Level>& levels,
                         const std::optional<std::string>& error)>
          callback) override;

  // Implementações para outras entidades

 private:
  // Membros para conexão com o MongoDB
  class Impl;
  std::unique_ptr<Impl> impl;
};
