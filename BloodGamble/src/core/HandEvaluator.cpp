#include "../../include/core/HandEvaluator.h"
#include <algorithm>
#include <unordered_map>

std::string HandValue::getDescription() const {
    switch (rank) {
        case HandRank::ROYAL_FLUSH: return "Royal Flush";
        case HandRank::STRAIGHT_FLUSH: return "Straight Flush";
        case HandRank::FOUR_KIND: return "Four of a Kind";
        case HandRank::FULL_HOUSE: return "Full House";
        case HandRank::FLUSH: return "Flush";
        case HandRank::STRAIGHT: return "Straight";
        case HandRank::THREE_KIND: return "Three of a Kind";
        case HandRank::TWO_PAIR: return "Two Pair";
        case HandRank::PAIR: return "One Pair";
        case HandRank::HIGH_CARD: return "High Card";
        default: return "Unknown";
    }
}

bool HandValue::operator>(const HandValue& other) const {
    if (rank != other.rank) return rank > other.rank;
    return tieBreakers > other.tieBreakers;
}

HandValue HandEvaluator::evaluateHand(const std::vector<Card>& allCards) {
    std::vector<Card> cards = allCards;
    std::sort(cards.begin(), cards.end(), [](const Card& a, const Card& b) {
        return a.rank > b.rank;
    });
    
    // Check for flush
    std::unordered_map<Suit, int> suitCounts;
    for (const auto& card : cards) {
        suitCounts[card.suit]++;
    }
    
    bool isFlush = false;
    Suit flushSuit;
    for (const auto& pair : suitCounts) {
        if (pair.second >= 5) {
            isFlush = true;
            flushSuit = pair.first;
            break;
        }
    }
    
    // Check for straight
    std::vector<Rank> uniqueRanks;
    for (const auto& card : cards) {
        if (std::find(uniqueRanks.begin(), uniqueRanks.end(), card.rank) == uniqueRanks.end()) {
            uniqueRanks.push_back(card.rank);
        }
    }
    std::sort(uniqueRanks.begin(), uniqueRanks.end(), std::greater<Rank>());
    
    bool isStraight = false;
    Rank straightHigh = Rank::TWO;
    
    for (size_t i = 0; i <= uniqueRanks.size() - 5; i++) {
        bool consecutive = true;
        for (size_t j = 1; j < 5; j++) {
            if (static_cast<int>(uniqueRanks[i + j]) != static_cast<int>(uniqueRanks[i + j - 1]) - 1) {
                consecutive = false;
                break;
            }
        }
        if (consecutive) {
            isStraight = true;
            straightHigh = uniqueRanks[i];
            break;
        }
    }
    
    // Check for A-2-3-4-5 straight (wheel)
    if (!isStraight && uniqueRanks.size() >= 5) {
        if (uniqueRanks[0] == Rank::ACE &&
            std::find(uniqueRanks.begin(), uniqueRanks.end(), Rank::FIVE) != uniqueRanks.end() &&
            std::find(uniqueRanks.begin(), uniqueRanks.end(), Rank::FOUR) != uniqueRanks.end() &&
            std::find(uniqueRanks.begin(), uniqueRanks.end(), Rank::THREE) != uniqueRanks.end() &&
            std::find(uniqueRanks.begin(), uniqueRanks.end(), Rank::TWO) != uniqueRanks.end()) {
            isStraight = true;
            straightHigh = Rank::FIVE;
        }
    }
    
    // Count ranks
    std::unordered_map<Rank, int> rankCounts;
    for (const auto& card : cards) {
        rankCounts[card.rank]++;
    }
    
    std::vector<std::pair<int, Rank>> countRankPairs;
    for (const auto& pair : rankCounts) {
        countRankPairs.emplace_back(pair.second, pair.first);
    }
    std::sort(countRankPairs.begin(), countRankPairs.end(), std::greater<std::pair<int, Rank>>());
    
    // Determine hand rank
    if (isStraight && isFlush) {
        if (straightHigh == Rank::ACE) {
            return {HandRank::ROYAL_FLUSH, {static_cast<int>(straightHigh)}, "Royal Flush"};
        } else {
            return {HandRank::STRAIGHT_FLUSH, {static_cast<int>(straightHigh)}, "Straight Flush"};
        }
    }
    
    if (countRankPairs[0].first == 4) {
        return {HandRank::FOUR_KIND, {static_cast<int>(countRankPairs[0].second), static_cast<int>(countRankPairs[1].second)}, "Four of a Kind"};
    }
    
    if (countRankPairs[0].first == 3 && countRankPairs[1].first >= 2) {
        return {HandRank::FULL_HOUSE, {static_cast<int>(countRankPairs[0].second), static_cast<int>(countRankPairs[1].second)}, "Full House"};
    }
    
    if (isFlush) {
        std::vector<int> flushCards;
        for (const auto& card : cards) {
            if (card.suit == flushSuit) {
                flushCards.push_back(static_cast<int>(card.rank));
            }
        }
        std::sort(flushCards.begin(), flushCards.end(), std::greater<int>());
        flushCards.resize(5);
        return {HandRank::FLUSH, flushCards, "Flush"};
    }
    
    if (isStraight) {
        return {HandRank::STRAIGHT, {static_cast<int>(straightHigh)}, "Straight"};
    }
    
    if (countRankPairs[0].first == 3) {
        std::vector<int> kickers;
        for (size_t i = 1; i < countRankPairs.size() && kickers.size() < 2; i++) {
            kickers.push_back(static_cast<int>(countRankPairs[i].second));
        }
        return {HandRank::THREE_KIND, {static_cast<int>(countRankPairs[0].second), kickers[0], kickers[1]}, "Three of a Kind"};
    }
    
    if (countRankPairs[0].first == 2 && countRankPairs[1].first == 2) {
        int kicker = static_cast<int>(countRankPairs[2].second);
        return {HandRank::TWO_PAIR, {std::max(static_cast<int>(countRankPairs[0].second), static_cast<int>(countRankPairs[1].second)),
                                    std::min(static_cast<int>(countRankPairs[0].second), static_cast<int>(countRankPairs[1].second)),
                                    kicker}, "Two Pair"};
    }
    
    if (countRankPairs[0].first == 2) {
        std::vector<int> kickers;
        for (size_t i = 1; i < countRankPairs.size() && kickers.size() < 3; i++) {
            kickers.push_back(static_cast<int>(countRankPairs[i].second));
        }
        return {HandRank::PAIR, {static_cast<int>(countRankPairs[0].second), kickers[0], kickers[1], kickers[2]}, "One Pair"};
    }
    
    // High card
    std::vector<int> highCards;
    for (size_t i = 0; i < std::min(size_t(5), countRankPairs.size()); i++) {
        highCards.push_back(static_cast<int>(countRankPairs[i].second));
    }
    return {HandRank::HIGH_CARD, highCards, "High Card"};
}