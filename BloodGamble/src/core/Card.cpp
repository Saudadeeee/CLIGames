#include "../../include/core/Card.h"

// ANSI color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"      // Hearts, Diamonds
#define BLACK   "\033[30m"      // Clubs, Spades  
#define BOLD    "\033[1m"
#define YELLOW  "\033[33m"      // Your cards
#define CYAN    "\033[36m"      // Board cards

std::string Card::toString() const {
    std::string rankStr;
    if (rank <= Rank::TEN) rankStr = std::to_string(static_cast<int>(rank));
    else if (rank == Rank::JACK) rankStr = "J";
    else if (rank == Rank::QUEEN) rankStr = "Q";
    else if (rank == Rank::KING) rankStr = "K";
    else rankStr = "A";
    
    std::string suitStr;
    std::string color;
    switch (suit) {
        case Suit::HEARTS: 
            suitStr = "H"; 
            color = RED;
            break;
        case Suit::DIAMONDS: 
            suitStr = "D"; 
            color = RED;
            break;
        case Suit::CLUBS: 
            suitStr = "C"; 
            color = BLACK;
            break;
        case Suit::SPADES: 
            suitStr = "S"; 
            color = BLACK;
            break;
    }
    
    return std::string(color) + BOLD + rankStr + suitStr + RESET;
}

std::string Card::toStringYours() const {
    std::string rankStr;
    if (rank <= Rank::TEN) rankStr = std::to_string(static_cast<int>(rank));
    else if (rank == Rank::JACK) rankStr = "J";
    else if (rank == Rank::QUEEN) rankStr = "Q";
    else if (rank == Rank::KING) rankStr = "K";
    else rankStr = "A";
    
    std::string suitStr;
    switch (suit) {
        case Suit::HEARTS: suitStr = "H"; break;
        case Suit::DIAMONDS: suitStr = "D"; break;
        case Suit::CLUBS: suitStr = "C"; break;
        case Suit::SPADES: suitStr = "S"; break;
    }
    
    return std::string(YELLOW) + BOLD + rankStr + suitStr + RESET;
}

std::string Card::toStringBoard() const {
    std::string rankStr;
    if (rank <= Rank::TEN) rankStr = std::to_string(static_cast<int>(rank));
    else if (rank == Rank::JACK) rankStr = "J";
    else if (rank == Rank::QUEEN) rankStr = "Q";
    else if (rank == Rank::KING) rankStr = "K";
    else rankStr = "A";
    
    std::string suitStr;
    switch (suit) {
        case Suit::HEARTS: suitStr = "H"; break;
        case Suit::DIAMONDS: suitStr = "D"; break;
        case Suit::CLUBS: suitStr = "C"; break;
        case Suit::SPADES: suitStr = "S"; break;
    }
    
    return std::string(CYAN) + BOLD + rankStr + suitStr + RESET;
}

Deck::Deck(unsigned int seed) : rng(seed) {
    reset();
}

void Deck::reset() {
    cards.clear();
    for (int s = 0; s < 4; s++) {
        for (int r = 2; r <= 14; r++) {
            cards.emplace_back(static_cast<Suit>(s), static_cast<Rank>(r));
        }
    }
    shuffle();
}

void Deck::shuffle() {
    std::shuffle(cards.begin(), cards.end(), rng);
}

Card Deck::draw() {
    if (cards.empty()) {
        throw std::runtime_error("Cannot draw from empty deck!");
    }
    Card card = cards.back();
    cards.pop_back();
    return card;
}

std::vector<Card> Deck::draw(int count) {
    std::vector<Card> drawn;
    for (int i = 0; i < count; i++) {
        drawn.push_back(draw());
    }
    return drawn;
}