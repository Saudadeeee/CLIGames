#pragma once
#include "Card.h"
#include <vector>
#include <unordered_map>

enum class HandRank { HIGH_CARD, PAIR, TWO_PAIR, THREE_KIND, STRAIGHT, FLUSH, FULL_HOUSE, FOUR_KIND, STRAIGHT_FLUSH, ROYAL_FLUSH };

struct HandValue {
    HandRank rank;
    std::vector<int> tieBreakers;
    std::string description; // Thêm mô tả lý do thắng
    
    bool operator>(const HandValue& other) const;
    std::string getDescription() const;
};

class HandEvaluator {
public:
    static HandValue evaluateHand(const std::vector<Card>& allCards);
};