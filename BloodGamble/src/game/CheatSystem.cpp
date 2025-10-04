#include "../../include/game/CheatSystem.h"
#include "../../include/game/GameState.h"
#include <iostream>
#include <iomanip>

CheatType::CheatType(const std::string& n, const std::string& desc, double bd, DetectionSeverity sev, int hp, int cd)
    : name(n), description(desc), baseDetect(bd), severity(sev), hpPenalty(hp), cooldown(cd) {}

CheatSystem::CheatSystem() {
    initializeCheatTypes();
}

void CheatSystem::initializeCheatTypes() {
    cheatTypes.emplace("SwapHands", CheatType("SwapHands", 
        "Swap your hole cards with 2 random cards from deck", 
        0.10, DetectionSeverity::MAJOR, 10, 4));
    cheatTypes.at("SwapHands").effect = [](Player* user, Player* target, GameState* game) {
        auto newCards = game->getDeck().draw(2);
        user->hand = newCards;
    };
    cheatTypes.emplace("PeekOpponentHole", CheatType("PeekOpponentHole",
        "Peek at an opponent's hole cards",
        0.06, DetectionSeverity::SMALL, 5, 2));
    cheatTypes.at("PeekOpponentHole").effect = [](Player* user, Player* target, GameState* game) {
        if (target && !target->isHuman) {
            std::cout << "\n[CHEAT SUCCESS] " << target->hand[0].toString() 
                     << " " << target->hand[1].toString() << " (AI " << target->id << ")\n";
        }
    };
    cheatTypes.emplace("MuckSwap", CheatType("MuckSwap",
        "Replace next board card with a random card",
        0.18, DetectionSeverity::MAJOR, 15, 5));
    cheatTypes.at("MuckSwap").effect = [](Player* user, Player* target, GameState* game) {
        if (!game->getDeck().empty()) {
            game->getDeck().draw(); // Remove top card
            game->getDeck().shuffle(); // Shuffle to randomize next card
        }
    };
    cheatTypes.emplace("ForceFold", CheatType("ForceFold",
        "Increase target AI's fold probability this round",
        0.12, DetectionSeverity::SMALL, 8, 4));
    cheatTypes.at("ForceFold").effect = [](Player* user, Player* target, GameState* game) {
        if (target && !target->isHuman) {
            target->tightness = std::min(1.0, target->tightness + 0.3);
            std::cout << "\n[CHEAT SUCCESS] AI " << target->id << " feels tilted!\n";
        }
    };
    cheatTypes.emplace("StackPeek", CheatType("StackPeek",
        "Reveal AI's HP and behavioral tendencies",
        0.05, DetectionSeverity::SMALL, 4, 3));
    cheatTypes.at("StackPeek").effect = [](Player* user, Player* target, GameState* game) {
        if (target && !target->isHuman) {
            std::cout << "\n[CHEAT SUCCESS] AI " << target->id << " - HP: " << target->hp 
                     << ", Aggression: " << std::fixed << std::setprecision(2) << target->aggression
                     << ", Tightness: " << target->tightness 
                     << ", Suspicion: " << target->suspicion << "\n";
        }
    };
    cheatTypes.emplace("CardMarking", CheatType("CardMarking",
        "Slightly bias next card draw in your favor",
        0.15, DetectionSeverity::MAJOR, 12, 6));
    cheatTypes.at("CardMarking").effect = [](Player* user, Player* target, GameState* game) {
        game->getDeck().shuffle();
        std::cout << "\n[CHEAT SUCCESS] Deck shuffled in your favor!\n";
    };
    cheatTypes.emplace("BluffBoost", CheatType("BluffBoost",
        "Make AIs less likely to call your bets this round",
        0.08, DetectionSeverity::SMALL, 6, 3));
    cheatTypes.at("BluffBoost").effect = [](Player* user, Player* target, GameState* game) {
        for (auto& player : game->getPlayers()) {
            if (!player.isHuman) {
                player.tightness = std::min(1.0, player.tightness + 0.2);
            }
        }
        std::cout << "\n[CHEAT SUCCESS] All AIs become more cautious!\n";
    };
}

const CheatType* CheatSystem::getCheat(const std::string& name) const {
    auto it = cheatTypes.find(name);
    return it != cheatTypes.end() ? &it->second : nullptr;
}