#pragma once

enum class Direction { Up, Down, Left, Right };

constexpr int kPlayerHp       = 100;
constexpr int kPlayerDmg      = 20;
constexpr int kEnemyHp        = 40;
constexpr int kEnemyDmg       = 10;
constexpr int kScorePerKill   = 15;
constexpr int kMinMapSize     = 10;
constexpr int kMaxMapSize     = 25;
constexpr int kDefaultMapSize = 15;

// --- Система заклинаний ---
constexpr int kHandMaxSize      = 5;
constexpr int kSpellDirectDmg   = 35;
constexpr int kSpellAreaDmg     = 15;
constexpr int kDirectSpellRange = 3;
constexpr int kAreaSpellRange   = 4;
constexpr int kKillsPerSpell    = 2;