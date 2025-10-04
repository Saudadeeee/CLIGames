#include "../../include/core/Player.h"

Player::Player(int playerId, bool human, int initialHp)
    : id(playerId), isHuman(human), hp(initialHp), folded(false), allIn(false),
      suspicion(0.0), aggression(0.5), tightness(0.5) {}

void Player::dealCards(const std::vector<Card>& cards) {
    hand = cards;
    folded = false;
    allIn = false;
}

bool Player::canBet(int amount) const {
    return !folded && !allIn && hp >= amount;
}

void Player::decreaseCooldowns() {
    for (auto& pair : cheatCooldowns) {
        if (pair.second > 0) pair.second--;
    }
}

bool Player::canUseCheat(const std::string& cheatName) const {
    auto it = cheatCooldowns.find(cheatName);
    return it == cheatCooldowns.end() || it->second <= 0;
}