#include "../../include/ai/AIPlayer.h"
#include "../../include/core/Card.h"
#include <algorithm>

PlayerAction AIPlayer::decideAction(Player& ai, const GameState& game, int callAmount, int minRaise) {
    if (ai.hp <= 0 || ai.folded) return PlayerAction::FOLD;
    
    // Calculate hand strength (simplified)
    std::vector<Card> allCards = ai.hand;
    allCards.insert(allCards.end(), game.board.begin(), game.board.end());
    
    double handStrength = 0.5; // Default average
    if (allCards.size() >= 2) {
        // Simple hand strength based on hole cards
        bool isPair = ai.hand[0].rank == ai.hand[1].rank;
        bool isHighCard = ai.hand[0].rank >= Rank::TEN || ai.hand[1].rank >= Rank::TEN;
        bool isSuited = ai.hand[0].suit == ai.hand[1].suit;
        
        if (isPair) handStrength = 0.8;
        else if (isHighCard && isSuited) handStrength = 0.7;
        else if (isHighCard) handStrength = 0.6;
        else if (isSuited) handStrength = 0.55;
    }
    
    // Adjust for AI personality and suspicion
    double effectiveAggression = ai.aggression * (1.0 - 0.5 * ai.suspicion);
    double effectiveTightness = ai.tightness * (1.0 + 0.3 * ai.suspicion);
    
    // Decision logic
    double foldThreshold = effectiveTightness * 0.8;
    double raiseThreshold = 0.7 + (1.0 - effectiveAggression) * 0.2;
    
    if (handStrength < foldThreshold) {
        return PlayerAction::FOLD;
    }
    
    if (callAmount >= ai.hp) {
        // Must go all-in or fold
        return handStrength > 0.6 ? PlayerAction::ALL_IN : PlayerAction::FOLD;
    }
    
    if (handStrength > raiseThreshold && ai.hp > callAmount + minRaise) {
        return PlayerAction::RAISE;
    }
    
    return PlayerAction::CALL;
}

int AIPlayer::decideRaiseAmount(const Player& ai, int callAmount, int minRaise, int maxBet) {
    int availableHP = ai.hp - callAmount;
    int raiseAmount = std::min({
        static_cast<int>(minRaise + ai.aggression * 10),
        maxBet,
        availableHP
    });
    return std::max(minRaise, raiseAmount);
}