#pragma once

#include <functional>
#include <optional>
#include <vector>

#include "../DTO.h"
#include "Language.h"

class LanguageService {
 public:
  virtual ~LanguageService() = default;

  virtual void createLanguage(
      const LanguageDTO &dto,
      std::function<void(const std::optional<Language> &,
                         const std::optional<std::string> &)>
          callback) = 0;

  virtual void getLanguages(
      std::function<void(const std::vector<Language> &,
                         const std::optional<std::string> &)>
          callback) = 0;

  virtual void getLanguageById(
      const std::string &id,
      std::function<void(const std::optional<Language> &,
                         const std::optional<std::string> &)>
          callback) = 0;
};
