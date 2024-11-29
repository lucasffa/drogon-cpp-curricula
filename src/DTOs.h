#pragma once

#include <string>
#include <vector>

#include "Enums.h"

// DTO para Language
struct LanguageDTO {
  std::string name;         // Nome da língua
  std::string description;  // Pode ser vazio, será gerado pela OpenAI
  std::vector<std::string>
      spokenInCountries;  // Lista de países onde a língua é falada
};

// DTO para Level
struct LevelDTO {
  LevelName name;           // Nome do nível (A1, A2, etc.)
  std::string description;  // Pode ser vazio, será gerado pela OpenAI
};

// DTO para Competency
struct CompetencyDTO {
  CompetencyType type;      // Tipo de competência (General ou Communicative)
  std::string name;         // Nome da competência
  std::string description;  // Descrição detalhada
  std::vector<std::string>
      subcompetencies;  // Lista de subcompetências associadas
};

// DTO para Unit (Unidade Temática)
struct UnitDTO {
  Domain domain;      // Domínio da unidade (Educational, Occupational, etc.)
  Activity activity;  // Atividade linguística (Reception, Production, etc.)
  std::string name;   // Nome da unidade
  std::string description;  // Descrição detalhada
};

// DTO para KnowledgeObject (Objeto de Conhecimento)
struct KnowledgeObjectDTO {
  KnowledgeObjectType type;  // Tipo do KnowledgeObject (Grammar, Lexis, etc.)
  std::string name;          // Nome do Objeto de Conhecimento
  std::string description;   // Descrição detalhada
  std::vector<std::string>
      subtypes;  // Lista de subtipos ou categorias adicionais
};

// DTO para Skill (Habilidade)
struct SkillDTO {
  SkillType type;           // Tipo da habilidade (Listening, Reading, etc.)
  std::string name;         // Nome da habilidade
  std::string description;  // Descrição detalhada
  std::vector<std::string> examples;  // Exemplos práticos da habilidade
  std::vector<std::string> assessmentCriteria;  // Critérios de avaliação
};
