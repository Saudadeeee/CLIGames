#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

// ===== ENUMS AND CONSTANTS =====
enum class Suit { HEARTS, DIAMONDS, CLUBS, SPADES };
enum class Rank { TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };

// ===== CARD SYSTEM =====
struct Card {
    Suit suit;
    Rank rank;
    
    Card(Suit s, Rank r) : suit(s), rank(r) {}
    
    std::string toString() const;
    std::string toStringYours() const;    // Yellow color for your cards
    std::string toStringBoard() const;    // Cyan color for board cards
};

class Deck {
private:
    std::vector<Card> cards;
    std::mt19937 rng;
    
public:
    Deck(unsigned int seed = std::time(nullptr));
    
    void reset();
    void shuffle();
    Card draw();
    std::vector<Card> draw(int count);
    bool empty() const { return cards.empty(); }
    size_t size() const { return cards.size(); }
};