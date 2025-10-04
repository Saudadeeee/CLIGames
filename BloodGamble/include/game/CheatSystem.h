#pragma once
#include "../core/Player.h"
#include <functional>

enum class DetectionSeverity { SMALL, MAJOR };

struct CheatType {
    std::string name;
    std::string description;
    double baseDetect;
    DetectionSeverity severity;
    int hpPenalty;
    int cooldown;
    std::function<void(Player*, Player*, class GameState*)> effect;
    
    CheatType() = default;  // Default constructor
    CheatType(const std::string& n, const std::string& desc, double bd, DetectionSeverity sev, int hp, int cd);
};

class CheatSystem {
private:
    std::unordered_map<std::string, CheatType> cheatTypes;
    
public:
    CheatSystem();
    
    void initializeCheatTypes();
    const std::unordered_map<std::string, CheatType>& getCheatTypes() const { return cheatTypes; }
    const CheatType* getCheat(const std::string& name) const;
};