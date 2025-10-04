#pragma once
#include "../core/Player.h"
#include "../game/GameState.h"
#include "../core/Config.h"

class AIPlayer {
public:
    static PlayerAction decideAction(Player& ai, const GameState& game, int callAmount, int minRaise);
    static int decideRaiseAmount(const Player& ai, int callAmount, int minRaise, int maxBet);
};