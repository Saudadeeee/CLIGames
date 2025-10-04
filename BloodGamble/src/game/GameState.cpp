#include "../../include/game/GameState.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

GameState::GameState(unsigned int seed)
    : deck(seed), pot(0), dealerIndex(0), stage(GameStage::PRE_FLOP),
      vigilance(0.0), roundNumber(0), rng(seed) {
    
    // Initialize players (1 human + 3 AI)
    players.emplace_back(0, true, HP_PLAYER_INIT);   // Human player
    players.emplace_back(1, false, HP_AI_INIT);      // AI 1
    players.emplace_back(2, false, HP_AI_INIT);      // AI 2
    players.emplace_back(3, false, HP_AI_INIT);      // AI 3
    
    currentBets.resize(4, 0);
}

double GameState::computeDetectionProbability(const std::string& cheatName, int targetId, GameStage currentStage) {
    const CheatType* cheat = cheatSystem.getCheat(cheatName);
    if (!cheat) return 1.0;
    
    double baseDetect = cheat->baseDetect;
    
    // Stage factor
    double stageFactor = 1.0;
    switch (currentStage) {
        case GameStage::PRE_FLOP: stageFactor = 0.9; break;
        case GameStage::FLOP: stageFactor = 1.0; break;
        case GameStage::TURN: stageFactor = 1.1; break;
        case GameStage::RIVER: stageFactor = 1.2; break;
        case GameStage::SHOWDOWN: stageFactor = 1.3; break;
    }
    
    // Target suspicion
    double targetSuspicion = 0.0;
    if (targetId >= 0 && targetId < static_cast<int>(players.size())) {
        targetSuspicion = players[targetId].suspicion;
    }
    
    // Repeat penalty
    double repeatPenalty = 0.0;
    int recentUses = 0;
    for (const auto& recentCheat : recentCheats) {
        if (recentCheat == cheatName) recentUses++;
    }
    repeatPenalty = recentUses * CHEAT_REPEAT_PENALTY;
    
    double finalProb = baseDetect * (1 + vigilance) * (1 + targetSuspicion) * stageFactor + repeatPenalty;
    return std::clamp(finalProb, 0.0, 0.95);
}

bool GameState::executeCheat(const std::string& cheatName, int targetId) {
    const CheatType* cheat = cheatSystem.getCheat(cheatName);
    if (!cheat) {
        std::cout << "Unknown cheat: " << cheatName << "\n";
        return false;
    }
    
    // Find human player
    Player* humanPlayer = nullptr;
    for (auto& p : players) {
        if (p.isHuman) {
            humanPlayer = &p;
            break;
        }
    }
    
    if (!humanPlayer) {
        std::cout << "No human player found!\n";
        return false;
    }
    
    if (!humanPlayer->canUseCheat(cheatName)) {
        std::cout << "Cheat is on cooldown!\n";
        return false;
    }
    
    double detectProb = computeDetectionProbability(cheatName, targetId, stage);
    
    std::cout << "\nEstimated detection chance: " << std::fixed << std::setprecision(1) 
              << (detectProb * 100) << "% (vigilance=" << std::setprecision(2) 
              << vigilance << ")\n";
    
    std::cout << "Proceed with cheat? (y/n): ";
    std::string confirm;
    std::cin >> confirm;
    if (confirm != "y" && confirm != "Y") {
        return false;
    }
    
    // Roll for detection
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    bool detected = dis(rng) < detectProb;
    
    if (detected) {
        std::cout << "\n*** ALERT: Your cheat '" << cheatName 
                 << "' was detected! You lose " << cheat->hpPenalty << " HP. ***\n";
        humanPlayer->hp -= cheat->hpPenalty;
        vigilance = std::min(MAX_VIGILANCE, vigilance + VIGILANCE_INCREMENT_PER_DETECT);
        
        // Increase suspicion of all AIs
        for (auto& p : players) {
            if (!p.isHuman) {
                p.suspicion = std::min(0.5, p.suspicion + 0.1);
            }
        }
    } else {
        std::cout << "\n[CHEAT SUCCESS] Using " << cheatName << "...\n";
        
        // Execute cheat effect
        Player* target = nullptr;
        if (targetId >= 0 && targetId < static_cast<int>(players.size())) {
            target = &players[targetId];
        }
        
        cheat->effect(humanPlayer, target, this);
    }
    
    // Set cooldown
    humanPlayer->cheatCooldowns[cheatName] = cheat->cooldown;
    
    // Add to recent cheats list (keep last 5)
    recentCheats.push_back(cheatName);
    if (recentCheats.size() > 5) {
        recentCheats.erase(recentCheats.begin());
    }
    
    return true;
}

void GameState::showCheatList() {
    // Find human player
    const Player* humanPlayer = nullptr;
    for (const auto& p : players) {
        if (p.isHuman) {
            humanPlayer = &p;
            break;
        }
    }
    
    if (!humanPlayer) return;
    
    std::cout << "\n=== AVAILABLE CHEATS ===\n";
    
    for (const auto& pair : cheatSystem.getCheatTypes()) {
        const CheatType& cheat = pair.second;
        bool onCooldown = !humanPlayer->canUseCheat(cheat.name);
        
        std::cout << cheat.name;
        if (onCooldown) {
            std::cout << " (COOLDOWN: " << humanPlayer->cheatCooldowns.at(cheat.name) << " rounds)";
        } else {
            double detectProb = computeDetectionProbability(cheat.name, -1, stage);
            std::cout << " (Detection: " << std::fixed << std::setprecision(1) 
                     << (detectProb * 100) << "%)";
        }
        std::cout << "\n  " << cheat.description << "\n\n";
    }
}

void GameState::updateVigilanceAfterRound(bool playerWon) {
    if (playerWon) {
        vigilance = std::min(MAX_VIGILANCE, vigilance + VIGILANCE_INCREMENT_PER_WIN);
    } else {
        vigilance = std::max(0.0, vigilance - VIGILANCE_DECREMENT_PER_AI_WIN);
    }
    
    // Decrease all cheat cooldowns
    for (auto& player : players) {
        player.decreaseCooldowns();
    }
}

std::vector<int> GameState::getActivePlayers() {
    std::vector<int> active;
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i].hp > 0 && !players[i].folded) {
            active.push_back(static_cast<int>(i));
        }
    }
    return active;
}

bool GameState::gameOver() {
    // Find human player
    int humanHp = 0;
    for (const auto& p : players) {
        if (p.isHuman) {
            humanHp = p.hp;
            break;
        }
    }
    
    int aiAlive = 0;
    
    for (size_t i = 1; i < players.size(); i++) {
        if (players[i].hp > 0) aiAlive++;
    }
    
    return humanHp <= 0 || aiAlive == 0;
}

void GameState::displayStatus() {
    std::cout << "\n=== GAME STATUS ===\n";
    std::cout << "Round: " << roundNumber << " | Stage: ";
    switch (stage) {
        case GameStage::PRE_FLOP: std::cout << "Pre-flop"; break;
        case GameStage::FLOP: std::cout << "Flop"; break;
        case GameStage::TURN: std::cout << "Turn"; break;
        case GameStage::RIVER: std::cout << "River"; break;
        case GameStage::SHOWDOWN: std::cout << "Showdown"; break;
    }
    std::cout << " | Pot: " << pot << " HP\n";
    std::cout << "Vigilance: " << std::fixed << std::setprecision(2) << vigilance << "\n";
    
    std::cout << "\nPlayers:\n";
    for (const auto& player : players) {
        std::cout << (player.isHuman ? "YOU" : ("AI " + std::to_string(player.id)));
        std::cout << " - HP: " << player.hp;
        if (player.folded) std::cout << " (FOLDED)";
        if (player.allIn) std::cout << " (ALL-IN)";
        std::cout << "\n";
    }
    
    if (!board.empty()) {
        std::cout << "\nBoard: ";
        for (const auto& card : board) {
            std::cout << card.toStringBoard() << " ";
        }
        std::cout << "\n";
    }
    
    // Find human player and show hand
    for (const auto& p : players) {
        if (p.isHuman && !p.hand.empty()) {
            std::cout << "Your hand: " << p.hand[0].toStringYours() 
                     << " " << p.hand[1].toStringYours() << "\n";
            break;
        }
    }
}