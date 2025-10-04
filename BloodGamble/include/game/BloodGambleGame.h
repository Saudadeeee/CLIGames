#pragma once
#include "GameState.h"
#include "../ai/AIPlayer.h"
#include "../core/HandEvaluator.h"

class BloodGambleGame {
private:
    GameState gameState;
    
public:
    BloodGambleGame(unsigned int seed = std::time(nullptr));
    
    void run();
    
private:
    void playRound();
    void postBlinds();
    void bettingRound();
    bool handlePlayerAction(Player& player, int currentBet, int playerIndex);
    bool handleAIAction(Player& ai, int currentBet, int aiIndex);
    void showdown();
    void awardPot(int winnerId);
    void endGame();
};