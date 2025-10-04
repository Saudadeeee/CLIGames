#pragma once
#include "../core/Player.h"
#include "../core/Card.h"
#include "CheatSystem.h"
#include "../core/Config.h"
#include <vector>
#include <random>
#include <ctime>

class GameState {
public:
    std::vector<Player> players;
    Deck deck;
    std::vector<Card> board;
    int pot;
    int dealerIndex;
    int smallBlindIndex;
    int bigBlindIndex;
    std::vector<int> currentBets;
    GameStage stage;
    double vigilance;
    int roundNumber;
    std::mt19937 rng;
    std::vector<std::string> recentCheats; // For repeat penalty calculation
    
    CheatSystem cheatSystem;
    
    GameState(unsigned int seed = std::time(nullptr));
    
    // Cheat functions
    double computeDetectionProbability(const std::string& cheatName, int targetId, GameStage currentStage);
    bool executeCheat(const std::string& cheatName, int targetId = -1);
    void showCheatList();
    
    // Game state functions
    void updateVigilanceAfterRound(bool playerWon);
    std::vector<int> getActivePlayers();
    bool gameOver();
    void displayStatus();
    
    // Accessors
    Deck& getDeck() { return deck; }
    std::vector<Player>& getPlayers() { return players; }
};