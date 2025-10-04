#include "include/game/BloodGambleGame.h"
#include <iostream>
#include <ctime>

int main() {
    std::cout << "Enter seed (0 for random): ";
    unsigned int seed;
    std::cin >> seed;
    
    if (seed == 0) {
        seed = std::time(nullptr);
    }
    
    BloodGambleGame game(seed);
    game.run();
    
    return 0;
}