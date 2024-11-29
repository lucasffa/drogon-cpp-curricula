#pragma once

#include <string>
#include <vector>

#include "Enums.h"

// Representa uma Habilidade específica
struct Skill {
  std::string id;                 // Identificador único
  std::string knowledgeObjectId;  // ID do KnowledgeObject pai
  SkillType type;           // Tipo da habilidade (Listening, Reading, etc.)
  std::string name;         // Nome da habilidade
  std::string description;  // Descrição detalhada
  std::vector<std::string> examples;  // Exemplos práticos da habilidade
  std::vector<std::string> assessmentCriteria;  // Critérios de avaliação
};

// Representa um Objeto de Conhecimento
struct KnowledgeObject {
  std::string id;            // Identificador único
  std::string unitId;        // ID da Unidade Temática pai
  KnowledgeObjectType type;  // Tipo do KnowledgeObject (Grammar, Lexis, etc.)
  std::string name;          // Nome do Objeto de Conhecimento
  std::string description;   // Descrição detalhada
  std::vector<std::string> subtypes;  // Subtipos ou categorias adicionais
  std::vector<Skill> skills;          // Lista de habilidades associadas
};

// Representa uma Unidade Temática
struct Unit {
  std::string id;            // Identificador único
  std::string competencyId;  // ID da Competência pai
  Domain domain;      // Domínio da unidade (Educational, Occupational, etc.)
  Activity activity;  // Atividade linguística (Reception, Production, etc.)
  std::string name;   // Nome da unidade
  std::string description;  // Descrição detalhada
  std::vector<KnowledgeObject>
      knowledgeObjects;  // Objetos de conhecimento relacionados
};

// Representa uma Competência
struct Competency {
  std::string id;           // Identificador único
  std::string levelId;      // ID do Nível pai
  CompetencyType type;      // Tipo de Competência (General, Communicative)
  std::string name;         // Nome da competência
  std::string description;  // Descrição detalhada
  std::vector<std::string>
      subcompetencies;      // Lista de subcompetências associadas
  std::vector<Unit> units;  // Unidades temáticas relacionadas
};

// Representa um Nível de Proficiência
struct Level {
  std::string id;                        // Identificador único
  std::string languageId;                // ID da Língua pai
  LevelName name;                        // Nome do nível (A1, A2, etc.)
  std::string description;               // Descrição detalhada
  std::vector<Competency> competencies;  // Competências associadas
};

// Representa uma Língua
struct Language {
  std::string id;                              // Identificador único
  std::string name;                            // Nome da língua
  std::string description;                     // Descrição detalhada
  std::vector<std::string> spokenInCountries;  // Lista de países onde é falada
  std::vector<Level> levels;  // Níveis de proficiência associados
};
