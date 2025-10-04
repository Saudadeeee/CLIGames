# Makefile for BloodGamble - Organized Structure

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude
TARGET = build/BloodGamble
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

# Source files
CORE_SOURCES = src/core/Card.cpp src/core/Player.cpp src/core/HandEvaluator.cpp
GAME_SOURCES = src/game/GameState.cpp src/game/CheatSystem.cpp src/game/BloodGambleGame.cpp
AI_SOURCES = src/ai/AIPlayer.cpp
MAIN_SOURCE = main.cpp

SOURCES = $(CORE_SOURCES) $(GAME_SOURCES) $(AI_SOURCES) $(MAIN_SOURCE)
OBJECTS = $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)

# Create directories
$(shell mkdir -p $(OBJ_DIR)/src/core $(OBJ_DIR)/src/game $(OBJ_DIR)/src/ai)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rmdir /s /q $(BUILD_DIR) 2>nul || true
	mkdir $(BUILD_DIR)
	mkdir $(OBJ_DIR)
	mkdir $(OBJ_DIR)\src
	mkdir $(OBJ_DIR)\src\core
	mkdir $(OBJ_DIR)\src\game
	mkdir $(OBJ_DIR)\src\ai

run: $(TARGET)
	$(TARGET).exe

.PHONY: all clean run

# Dependencies
$(OBJ_DIR)/main.o: main.cpp include/game/BloodGambleGame.h
$(OBJ_DIR)/src/core/Card.o: src/core/Card.cpp include/core/Card.h
$(OBJ_DIR)/src/core/Player.o: src/core/Player.cpp include/core/Player.h include/core/Card.h
$(OBJ_DIR)/src/core/HandEvaluator.o: src/core/HandEvaluator.cpp include/core/HandEvaluator.h include/core/Card.h
$(OBJ_DIR)/src/game/CheatSystem.o: src/game/CheatSystem.cpp include/game/CheatSystem.h include/core/Player.h include/game/GameState.h
$(OBJ_DIR)/src/game/GameState.o: src/game/GameState.cpp include/game/GameState.h include/core/Player.h include/core/Card.h include/game/CheatSystem.h include/core/Config.h
$(OBJ_DIR)/src/ai/AIPlayer.o: src/ai/AIPlayer.cpp include/ai/AIPlayer.h include/core/Player.h include/game/GameState.h include/core/Config.h include/core/Card.h
$(OBJ_DIR)/src/game/BloodGambleGame.o: src/game/BloodGambleGame.cpp include/game/BloodGambleGame.h include/game/GameState.h include/ai/AIPlayer.h include/core/HandEvaluator.h