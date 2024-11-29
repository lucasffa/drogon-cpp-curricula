#pragma once

enum class LevelName { A1, A2, B1, B2, C1, C2 };

// Enums para as outras entidades (já preparados)
enum class CompetencyType { General, Communicative };

enum class GeneralSubcompetency {
  DeclarativeKnowledge,
  ProceduralKnowledge,
  ExistentialCompetence,
  LearningAbility
};

enum class CommunicativeSubcompetency {
  LinguisticCompetence,
  SociolinguisticCompetence,
  PragmaticCompetence
};

enum class Domain { Educational, Occupational, Public, Personal };

enum class Activity { Reception, Production, Interaction, Mediation };

enum class KnowledgeObjectType {
  Grammar,
  Lexis,
  PhoneticsPhonology,
  Discourse,
  InterculturalCompetence
};

enum class SkillType {
  ListeningComprehension,
  ReadingComprehension,
  OralProduction,
  WrittenProduction,
  InteractiveCommunication,
  MediatingCommunication
};
