#pragma once

#include <optional>
#include <string>

#include "DTOs.h"

class Validators {
 public:
  static std::optional<std::string> validateLanguageDTO(const LanguageDTO& dto);
  static std::optional<std::string> validateLevelDTO(const LevelDTO& dto);
  // Validadores para as outras entidades
};
