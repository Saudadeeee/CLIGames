#include "../../include/game/BloodGambleGame.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

BloodGambleGame::BloodGambleGame(unsigned int seed) : gameState(seed) {}

void BloodGambleGame::run() {
    std::cout << "=== BLOOD GAMBLE ===\n";
    std::cout << "A poker game where lives are the stakes!\n\n";
    
    while (!gameState.gameOver()) {
        playRound();
    }
    
    endGame();
}

void BloodGambleGame::playRound() {
    gameState.roundNumber++;
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "ROUND " << gameState.roundNumber << "\n";
    std::cout << std::string(50, '=') << "\n";
    
    // Reset for new round
    gameState.deck.reset();
    gameState.board.clear();
    gameState.pot = 0;
    std::fill(gameState.currentBets.begin(), gameState.currentBets.end(), 0);
    
    // Reset player states for new round
    for (auto& player : gameState.players) {
        player.folded = false;
        player.allIn = false;
    }
    
    // Deal hole cards
    for (auto& player : gameState.players) {
        if (player.hp > 0) {
            player.dealCards(gameState.deck.draw(2));
        }
    }
    
    // Post blinds
    postBlinds();
    
    // Play each stage
    for (int stageInt = 0; stageInt <= 4; stageInt++) {
        gameState.stage = static_cast<GameStage>(stageInt);
        
        if (gameState.stage == GameStage::FLOP) {
            std::cout << "\n=== FLOP ===\n";
            gameState.deck.draw(); // Burn card
            auto flopCards = gameState.deck.draw(3);
            gameState.board.insert(gameState.board.end(), flopCards.begin(), flopCards.end());
            std::cout << "Board: ";
            for (const auto& card : gameState.board) {
                std::cout << card.toStringBoard() << " ";
            }
            std::cout << "\n";
        } else if (gameState.stage == GameStage::TURN) {
            std::cout << "\n=== TURN ===\n";
            gameState.deck.draw(); // Burn card
            gameState.board.push_back(gameState.deck.draw());
            std::cout << "Board: ";
            for (const auto& card : gameState.board) {
                std::cout << card.toStringBoard() << " ";
            }
            std::cout << "\n";
        } else if (gameState.stage == GameStage::RIVER) {
            std::cout << "\n=== RIVER ===\n";
            gameState.deck.draw(); // Burn card
            gameState.board.push_back(gameState.deck.draw());
            std::cout << "Board: ";
            for (const auto& card : gameState.board) {
                std::cout << card.toStringBoard() << " ";
            }
            std::cout << "\n";
        } else if (gameState.stage == GameStage::SHOWDOWN) {
            showdown();
            break;
        }
        
        if (gameState.stage != GameStage::SHOWDOWN) {
            bettingRound();
            
            auto activePlayers = gameState.getActivePlayers();
            if (activePlayers.size() <= 1) {
                awardPot(activePlayers.empty() ? -1 : activePlayers[0]);
                return;
            }
            
            // Reset current bets for next stage (except first stage which has blinds)
            if (gameState.stage != GameStage::PRE_FLOP) {
                std::fill(gameState.currentBets.begin(), gameState.currentBets.end(), 0);
            }
        }
    }
}

void BloodGambleGame::postBlinds() {
    // Calculate blind positions
    gameState.smallBlindIndex = (gameState.dealerIndex + 1) % 4;
    gameState.bigBlindIndex = (gameState.dealerIndex + 2) % 4;
    
    // Post small blind
    if (gameState.players[gameState.smallBlindIndex].hp > 0) {
        int sbAmount = std::min(SMALL_BLIND, gameState.players[gameState.smallBlindIndex].hp);
        gameState.players[gameState.smallBlindIndex].hp -= sbAmount;
        gameState.pot += sbAmount;
        gameState.currentBets[gameState.smallBlindIndex] = sbAmount;
    }
    
    // Post big blind
    if (gameState.players[gameState.bigBlindIndex].hp > 0) {
        int bbAmount = std::min(BIG_BLIND, gameState.players[gameState.bigBlindIndex].hp);
        gameState.players[gameState.bigBlindIndex].hp -= bbAmount;
        gameState.pot += bbAmount;
        gameState.currentBets[gameState.bigBlindIndex] = bbAmount;
    }
}

void BloodGambleGame::bettingRound() {
    std::vector<int> activePlayers = gameState.getActivePlayers();
    if (activePlayers.size() <= 1) return;
    
    // Starting position depends on stage
    int actionIndex;
    if (gameState.stage == GameStage::PRE_FLOP) {
        actionIndex = (gameState.bigBlindIndex + 1) % 4; // UTG starts preflop
    } else {
        actionIndex = gameState.smallBlindIndex; // SB starts post-flop
    }
    
    // Find first active player
    while (std::find(activePlayers.begin(), activePlayers.end(), actionIndex) == activePlayers.end()) {
        actionIndex = (actionIndex + 1) % 4;
    }
    
    std::vector<bool> hasActed(4, false); // Track who has acted this round
    bool someoneRaised = false;
    int actionCount = 0; // Safety counter
    
    while (actionCount < 20) { // Safety limit
        actionCount++;
        
        // Update active players
        activePlayers = gameState.getActivePlayers();
        if (activePlayers.size() <= 1) break;
        
        // Skip inactive players
        if (std::find(activePlayers.begin(), activePlayers.end(), actionIndex) == activePlayers.end()) {
            actionIndex = (actionIndex + 1) % 4;
            continue;
        }
        
        Player& currentPlayer = gameState.players[actionIndex];
        int currentBet = *std::max_element(gameState.currentBets.begin(), gameState.currentBets.end());
        
        // Check if this player needs to act
        bool needsToAct = (gameState.currentBets[actionIndex] < currentBet && !currentPlayer.allIn) || 
                         !hasActed[actionIndex]; // Everyone gets at least one chance per stage
        
        if (needsToAct) {
            hasActed[actionIndex] = true;
            
            if (currentPlayer.isHuman) {
                gameState.displayStatus();
                bool raised = handlePlayerAction(currentPlayer, currentBet, actionIndex);
                if (raised) {
                    someoneRaised = true;
                    // Reset hasActed for everyone except the raiser
                    std::fill(hasActed.begin(), hasActed.end(), false);
                    hasActed[actionIndex] = true;
                }
            } else {
                bool raised = handleAIAction(currentPlayer, currentBet, actionIndex);
                if (raised) {
                    someoneRaised = true;
                    // Reset hasActed for everyone except the raiser
                    std::fill(hasActed.begin(), hasActed.end(), false);
                    hasActed[actionIndex] = true;
                }
            }
        }
        
        // Move to next player
        actionIndex = (actionIndex + 1) % 4;
        
        // Check if betting round is complete
        bool allActiveHaveActed = true;
        bool allBetsEqual = true;
        int maxBet = *std::max_element(gameState.currentBets.begin(), gameState.currentBets.end());
        
        for (int pid : activePlayers) {
            if (!hasActed[pid]) {
                allActiveHaveActed = false;
                break;
            }
            if (!gameState.players[pid].allIn && gameState.currentBets[pid] != maxBet) {
                allBetsEqual = false;
            }
        }
        
        // End round if everyone has acted and bets are equal
        if (allActiveHaveActed && allBetsEqual) {
            break;
        }
    }
}

bool BloodGambleGame::handlePlayerAction(Player& player, int currentBet, int playerIndex) {
    int callAmount = currentBet - gameState.currentBets[playerIndex];
    
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "YOUR TURN! Current bet to call: " << callAmount << " HP\n";
    std::cout << std::string(50, '=') << "\n";
    std::cout << "1. Fold\n";
    std::cout << "2. Call (" << callAmount << " HP)\n";
    std::cout << "3. Raise\n";
    std::cout << "4. All-in\n";
    std::cout << "5. Cheat list\n";
    std::cout << "6. Use cheat\n";
    std::cout << "7. Status\n";
    std::cout << std::string(50, '=') << "\n";
    std::cout << "Choose (1-7): ";
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1: // Fold
            player.folded = true;
            std::cout << "You fold.\n";
            return false;
            
        case 2: // Call
            if (callAmount > player.hp) {
                std::cout << "Not enough HP! Going all-in instead.\n";
                gameState.pot += player.hp;
                gameState.currentBets[playerIndex] += player.hp;
                player.hp = 0;
                player.allIn = true;
            } else {
                player.hp -= callAmount;
                gameState.pot += callAmount;
                gameState.currentBets[playerIndex] += callAmount;
                std::cout << "You call " << callAmount << " HP.\n";
            }
            return false;
            
        case 3: // Raise
            {
                std::cout << "Enter raise amount: ";
                int raiseAmount;
                std::cin >> raiseAmount;
                int totalBet = callAmount + raiseAmount;
                
                if (totalBet > player.hp) {
                    std::cout << "Not enough HP! Going all-in instead.\n";
                    gameState.pot += player.hp;
                    gameState.currentBets[playerIndex] += player.hp;
                    player.hp = 0;
                    player.allIn = true;
                } else {
                    player.hp -= totalBet;
                    gameState.pot += totalBet;
                    gameState.currentBets[playerIndex] += totalBet;
                    std::cout << "You raise to " << gameState.currentBets[playerIndex] << " HP.\n";
                }
                return true; // Raised
            }
            
        case 4: // All-in
            gameState.pot += player.hp;
            gameState.currentBets[playerIndex] += player.hp;
            player.hp = 0;
            player.allIn = true;
            std::cout << "You go all-in!\n";
            return true; // All-in counts as raise
            
        case 5: // Cheat list
            gameState.showCheatList();
            return handlePlayerAction(player, currentBet, playerIndex); // Retry action
            
        case 6: // Use cheat
            {
                std::cout << "Enter cheat name: ";
                std::string cheatName;
                std::cin >> cheatName;
                
                std::cout << "Enter target ID (-1 for no target): ";
                int targetId;
                std::cin >> targetId;
                
                if (gameState.executeCheat(cheatName, targetId)) {
                    std::cout << "Cheat executed. ";
                }
                return handlePlayerAction(player, currentBet, playerIndex); // Retry action after cheat
            }
            
        case 7: // Status
            gameState.displayStatus();
            return handlePlayerAction(player, currentBet, playerIndex); // Retry action
            
        default:
            std::cout << "Invalid choice. Try again.\n";
            return handlePlayerAction(player, currentBet, playerIndex); // Retry
    }
}

bool BloodGambleGame::handleAIAction(Player& ai, int currentBet, int aiIndex) {
    int callAmount = currentBet - gameState.currentBets[aiIndex];
    PlayerAction action = AIPlayer::decideAction(ai, gameState, callAmount, MIN_BET);
    
    switch (action) {
        case PlayerAction::FOLD:
            ai.folded = true;
            std::cout << "AI " << ai.id << " folds.\n";
            return false;
            
        case PlayerAction::CALL:
            if (callAmount > ai.hp) {
                gameState.pot += ai.hp;
                gameState.currentBets[aiIndex] += ai.hp;
                ai.hp = 0;
                ai.allIn = true;
                std::cout << "AI " << ai.id << " goes all-in (forced)!\n";
            } else {
                ai.hp -= callAmount;
                gameState.pot += callAmount;
                gameState.currentBets[aiIndex] += callAmount;
                std::cout << "AI " << ai.id << " calls " << callAmount << " HP.\n";
            }
            return false;
            
        case PlayerAction::RAISE: {
            int raiseAmount = AIPlayer::decideRaiseAmount(ai, callAmount, MIN_BET, MAX_BET);
            int totalBet = callAmount + raiseAmount;
            
            if (totalBet > ai.hp) {
                gameState.pot += ai.hp;
                gameState.currentBets[aiIndex] += ai.hp;
                ai.hp = 0;
                ai.allIn = true;
                std::cout << "AI " << ai.id << " goes all-in!\n";
            } else {
                ai.hp -= totalBet;
                gameState.pot += totalBet;
                gameState.currentBets[aiIndex] += totalBet;
                std::cout << "AI " << ai.id << " raises to " << gameState.currentBets[aiIndex] << " HP.\n";
            }
            return true;
        }
        
        case PlayerAction::ALL_IN:
            gameState.pot += ai.hp;
            gameState.currentBets[aiIndex] += ai.hp;
            ai.hp = 0;
            ai.allIn = true;
            std::cout << "AI " << ai.id << " goes all-in!\n";
            return true;
            
        default:
            return false;
    }
}

void BloodGambleGame::showdown() {
    std::vector<int> activePlayers = gameState.getActivePlayers();
    if (activePlayers.size() <= 1) {
        awardPot(activePlayers.empty() ? -1 : activePlayers[0]);
        return;
    }
    
    std::cout << "\n=== SHOWDOWN ===\n";
    
    std::vector<std::pair<int, HandValue>> playerHands;
    for (int pid : activePlayers) {
        std::vector<Card> allCards = gameState.players[pid].hand;
        allCards.insert(allCards.end(), gameState.board.begin(), gameState.board.end());
        HandValue handValue = HandEvaluator::evaluateHand(allCards);
        playerHands.emplace_back(pid, handValue);
        
        std::cout << (gameState.players[pid].isHuman ? "YOU" : ("AI " + std::to_string(pid)));
        std::cout << ": " << gameState.players[pid].hand[0].toString() 
                 << " " << gameState.players[pid].hand[1].toString() << "\n";
    }
    
    // Find winner
    std::sort(playerHands.begin(), playerHands.end(), 
             [](const auto& a, const auto& b) { return a.second > b.second; });
    
    int winnerId = playerHands[0].first;
    HandValue winningHand = playerHands[0].second;
    
    std::cout << "\nWinner: " << (gameState.players[winnerId].isHuman ? "YOU" : ("AI " + std::to_string(winnerId)));
    std::cout << " with " << winningHand.description << "!\n";
    
    awardPot(winnerId);
}

void BloodGambleGame::awardPot(int winnerId) {
    if (winnerId == -1 || gameState.pot == 0) return;
    
    gameState.players[winnerId].hp += gameState.pot;
    
    std::cout << "\n" << (gameState.players[winnerId].isHuman ? "YOU" : ("AI " + std::to_string(winnerId)));
    std::cout << " wins " << gameState.pot << " HP!\n";
    
    bool playerWon = gameState.players[winnerId].isHuman;
    gameState.updateVigilanceAfterRound(playerWon);
    
    gameState.pot = 0;
    std::fill(gameState.currentBets.begin(), gameState.currentBets.end(), 0);
    
    // Advance dealer
    do {
        gameState.dealerIndex = (gameState.dealerIndex + 1) % 4;
    } while (gameState.players[gameState.dealerIndex].hp <= 0);
    
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

void BloodGambleGame::endGame() {
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "GAME OVER\n";
    std::cout << std::string(50, '=') << "\n";
    
    if (gameState.players[0].hp <= 0) {
        std::cout << "You have been eliminated! The Hell Soldiers have won.\n";
    } else {
        std::cout << "Congratulations! You have defeated all Hell Soldiers!\n";
        std::cout << "Your final HP: " << gameState.players[0].hp << "\n";
    }
    
    std::cout << "\nFinal vigilance level: " << std::fixed << std::setprecision(2) 
              << gameState.vigilance << "\n";
    std::cout << "Rounds played: " << gameState.roundNumber << "\n";
}