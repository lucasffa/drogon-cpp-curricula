#include "Database.h"

#include <drogon/drogon.h>

#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

class MongoDatabase::Impl {
 public:
  Impl() {
    mongocxx::instance instance{};  // Deve ser declarado uma vez
    std::string uri_str =
        drogon::app().getCustomConfig()["mongo"]["uri"].asString();
    mongocxx::uri uri(uri_str);
    client = mongocxx::client(uri);
    db = client["language_backend"];
  }

  mongocxx::client client;
  mongocxx::database db;
};

std::shared_ptr<Database> Database::getInstance() {
  static std::shared_ptr<Database> instance = std::make_shared<MongoDatabase>();
  return instance;
}

MongoDatabase::MongoDatabase() : impl(new Impl()) {}

void MongoDatabase::insertLanguage(
    const Language& language,
    std::function<void(const std::optional<std::string>& error)> callback) {
  auto collection = impl->db["languages"];
  bsoncxx::builder::stream::document document{};

  document << "name" << language.name << "description" << language.description
           << "spoken_in_countries" << language.spokenInCountries;

  auto result = collection.insert_one(document.view());

  if (result && result->result().inserted_count() == 1) {
    callback(std::nullopt);  // Sucesso
  } else {
    callback("Failed to insert language into database.");
  }
}

void MongoDatabase::getLanguages(
    std::function<void(const std::vector<Language>& languages,
                       const std::optional<std::string>& error)>
        callback) {
  auto collection = impl->db["languages"];
  mongocxx::cursor cursor = collection.find({});
  std::vector<Language> languages;

  for (const auto& doc : cursor) {
    Language lang;

    // ID
    if (doc["_id"] && doc["_id"].type() == bsoncxx::type::k_oid) {
      lang.id = doc["_id"].get_oid().value.to_string();
    }

    // Name
    if (doc["name"] && doc["name"].type() == bsoncxx::type::k_string) {
      lang.name = std::string(doc["name"].get_string().value);
    }

    // Description
    if (doc["description"] &&
        doc["description"].type() == bsoncxx::type::k_string) {
      lang.description = std::string(doc["description"].get_string().value);
    }

    // Spoken in countries
    if (doc["spoken_in_countries"] &&
        doc["spoken_in_countries"].type() == bsoncxx::type::k_array) {
      for (const auto& country : doc["spoken_in_countries"].get_array().value) {
        if (country.type() == bsoncxx::type::k_string) {
          lang.spokenInCountries.push_back(
              std::string(country.get_string().value));
        }
      }
    }

    languages.push_back(lang);
  }

  callback(languages, std::nullopt);
}

void MongoDatabase::insertLevel(
    const Level& level,
    std::function<void(const std::optional<std::string>& error)> callback) {
  auto collection = impl->db["levels"];
  bsoncxx::builder::stream::document document{};

  document << "language_id" << bsoncxx::oid{level.languageId} << "name"
           << static_cast<int>(level.name) << "description"
           << level.description;

  auto result = collection.insert_one(document.view());

  if (result && result->result().inserted_count() == 1) {
    callback(std::nullopt);  // Sucesso
  } else {
    callback("Failed to insert level into database.");
  }
}

void MongoDatabase::getLevelsByLanguageId(
    const std::string& languageId,
    std::function<void(const std::vector<Level>& levels,
                       const std::optional<std::string>& error)>
        callback) {
  auto collection = impl->db["levels"];
  bsoncxx::builder::stream::document filter{};

  // Construindo o filtro com o ObjectId
  try {
    filter << "language_id" << bsoncxx::oid{languageId};
  } catch (const std::exception& e) {
    callback({}, "Invalid languageId format: " + std::string(e.what()));
    return;
  }

  mongocxx::cursor cursor = collection.find(filter.view());
  std::vector<Level> levels;

  for (const auto& doc : cursor) {
    Level level;

    // ID
    if (doc["_id"] && doc["_id"].type() == bsoncxx::type::k_oid) {
      level.id = doc["_id"].get_oid().value.to_string();
    }

    // language_id
    if (doc["language_id"] &&
        doc["language_id"].type() == bsoncxx::type::k_oid) {
      level.languageId = doc["language_id"].get_oid().value.to_string();
    }

    // name
    if (doc["name"] && doc["name"].type() == bsoncxx::type::k_int32) {
      level.name = static_cast<LevelName>(doc["name"].get_int32().value);
    }

    // description
    if (doc["description"] &&
        doc["description"].type() == bsoncxx::type::k_string) {
      level.description = std::string(doc["description"].get_string().value);
    }

    levels.push_back(level);
  }

  callback(levels, std::nullopt);
}

// Implementações para outras entidades
