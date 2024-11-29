#include "Validators.h"

std::optional<std::string> Validators::validateLanguageDTO(
    const LanguageDTO& dto) {
  if (dto.name.empty()) {
    return "Language name is required.";
  }
  if (dto.spokenInCountries.empty()) {
    return "At least one country where the language is spoken is required.";
  }
  return std::nullopt;  // Sem erros
}

std::optional<std::string> Validators::validateLevelDTO(const LevelDTO& dto) {
  // Não há validações adicionais necessárias neste exemplo
  return std::nullopt;
}

// Implementações dos validadores para as outras entidades
