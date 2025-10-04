# BloodGamble Project Structure

## Directory Organization

```
BloodGamble/
├── main.cpp                    # Entry point
├── Makefile                    # Build configuration
├── include/                    # Header files
│   ├── core/                   # Core system components
│   │   ├── Config.h           # Game constants and enums
│   │   ├── Card.h             # Card and Deck system
│   │   ├── Player.h           # Player class definition
│   │   └── HandEvaluator.h    # Poker hand evaluation
│   ├── game/                   # Game logic
│   │   ├── GameState.h        # Game state management
│   │   ├── CheatSystem.h      # Cheat mechanics
│   │   └── BloodGambleGame.h  # Main game engine
│   └── ai/                     # AI components
│       └── AIPlayer.h         # AI decision logic
├── src/                        # Source files
│   ├── core/                   # Core implementations
│   │   ├── Card.cpp
│   │   ├── Player.cpp
│   │   └── HandEvaluator.cpp
│   ├── game/                   # Game implementations
│   │   ├── GameState.cpp
│   │   ├── CheatSystem.cpp
│   │   └── BloodGambleGame.cpp
│   └── ai/                     # AI implementations
│       └── AIPlayer.cpp
├── build/                      # Build artifacts
│   ├── obj/                    # Object files
│   └── BloodGamble.exe         # Final executable
└── docs/                       # Documentation
    └── README.md               # Game documentation
```

## Module Descriptions

### Core (`include/core/`, `src/core/`)
- **Fundamental game components that could be reused**
- `Config.h`: Game constants, enums, and configuration
- `Card.h/cpp`: Playing card representation and deck management
- `Player.h/cpp`: Player data structure and basic operations
- `HandEvaluator.h/cpp`: Poker hand strength evaluation

### Game (`include/game/`, `src/game/`)
- **BloodGamble-specific game logic**
- `GameState.h/cpp`: Central game state management, cheat execution
- `CheatSystem.h/cpp`: Cheat types, effects, and detection system
- `BloodGambleGame.h/cpp`: Main game loop, betting rounds, showdown

### AI (`include/ai/`, `src/ai/`)
- **Artificial intelligence components**
- `AIPlayer.h/cpp`: AI decision making, personality simulation

### Build (`build/`)
- **Generated files and build artifacts**
- `obj/`: Compiled object files organized by module
- `BloodGamble.exe`: Final executable

### Documentation (`docs/`)
- **Project documentation**
- `README.md`: Comprehensive game documentation

## Build Process

1. **Compilation**: Each module compiles independently
2. **Organization**: Object files stored in `build/obj/` with same structure as source
3. **Linking**: All objects linked into final executable in `build/`
4. **Dependencies**: Makefile handles include paths and dependencies

## Benefits of This Structure

### 1. Modularity
- Clear separation of concerns
- Core components reusable for other poker variants
- Game-specific logic isolated

### 2. Maintainability
- Easy to locate and modify specific functionality
- Reduced coupling between modules
- Clean dependency hierarchy

### 3. Scalability
- Easy to add new cheat types in game module
- AI improvements isolated to ai module
- Core poker logic can be extended

### 4. Build Efficiency
- Incremental compilation
- Only modified modules rebuild
- Parallel compilation possible

## Include Path Strategy

- All includes use relative paths from project root
- Module headers include dependencies explicitly
- Forward declarations minimize compilation dependencies

## Development Workflow

1. **Core changes**: Modify fundamental game mechanics
2. **Game changes**: Add/modify BloodGamble-specific features
3. **AI changes**: Improve AI behavior and decision making
4. **Build**: `make all` builds everything
5. **Test**: `make run` executes the game
6. **Clean**: `make clean` removes build artifacts