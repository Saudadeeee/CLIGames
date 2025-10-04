#pragma once
#include "Card.h"
#include <vector>
#include <string>
#include <unordered_map>

class Player {
public:
    int id;
    bool isHuman;
    int hp;
    std::vector<Card> hand;
    bool folded;
    bool allIn;
    std::unordered_map<std::string, int> cheatCooldowns;
    double suspicion;
    double aggression;
    double tightness;
    
    Player(int playerId, bool human, int initialHp);
    
    void dealCards(const std::vector<Card>& cards);
    bool canBet(int amount) const;
    void decreaseCooldowns();
    bool canUseCheat(const std::string& cheatName) const;
};