#pragma once

// Game configuration constants
const int HP_PLAYER_INIT = 100;
const int HP_AI_INIT = 80;
const int SMALL_BLIND = 1;
const int BIG_BLIND = 2;
const int MIN_BET = BIG_BLIND;
const int MAX_BET = 20;
const double BASE_DETECT_DEFAULT = 0.05;
const double VIGILANCE_INCREMENT_PER_WIN = 0.02;
const double VIGILANCE_INCREMENT_PER_DETECT = 0.05;
const double VIGILANCE_DECREMENT_PER_AI_WIN = 0.01;
const double MAX_VIGILANCE = 0.6;
const double CHEAT_REPEAT_PENALTY = 0.03;

enum class GameStage { PRE_FLOP, FLOP, TURN, RIVER, SHOWDOWN };
enum class PlayerAction { FOLD, CALL, RAISE, ALL_IN, NONE };