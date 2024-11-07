#include <algorithm>
#include <iostream>
#include <sstream>
#include <memory>
#include "gamemanager.h"

GameManager::GameManager() : hasSetup(false), whiteScore(0), blackScore(0) {}

void GameManager::gameLoop() {
    std::string command;

    while (true) {
        std::cout << "Enter command: ";
        if (!std::getline(std::cin, command)) {
            std::cout << std::endl;
            break; // End of input (Ctrl-D)
        }

        processCommand(command);
    }

    printFinalScore();
}

void GameManager::processCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "game") {
        std::string whitePlayerType, blackPlayerType;
        iss >> whitePlayerType >> blackPlayerType;
        startNewGame(whitePlayerType, blackPlayerType);
    } else if (cmd == "setup") {
        setupManager.enterSetupMode();
        board = setupManager.getBoard();
        hasSetup = true;
    } else {
        std::cout << "Unknown command: " << command << std::endl;
    }
}

std::unique_ptr<Player> GameManager::createPlayer(const std::string& playerType, bool isWhite) {
    if (playerType == "human") {
        return std::make_unique<HumanPlayer>(isWhite);
    } else if (playerType.rfind("computer", 0) == 0) {
        std::string levelStr = playerType.substr(8); // Extract the level part of the string
        levelStr.erase(std::remove_if(levelStr.begin(), levelStr.end(), [](char c) { return !std::isdigit(c); }), levelStr.end());
        
        if (levelStr.empty()) {
            std::cout << "Invalid computer level format. Expected 'computer1', 'computer[1]', etc." << std::endl;
            return nullptr;
        }
        
        int level = 0;
        try {
            level = std::stoi(levelStr);
        } catch (const std::invalid_argument& e) {
            std::cout << "Error: Invalid argument. Could not convert " << levelStr << " to an integer." << std::endl;
            return nullptr;
        } catch (const std::out_of_range& e) {
            std::cout << "Error: Out of range. The level " << levelStr << " is too large." << std::endl;
            return nullptr;
        }

        if (level < 1 || level > 4) {
            std::cout << "Invalid computer level. Must be between 1 and 4." << std::endl;
            return nullptr;
        }
        return std::make_unique<ComputerPlayer>(isWhite, level);
    } else {
        std::cout << "Invalid player type: " << playerType << std::endl;
        return nullptr;
    }
}

void GameManager::startNewGame(const std::string& whitePlayerType, const std::string& blackPlayerType) {
    whitePlayer = createPlayer(whitePlayerType, true);
    blackPlayer = createPlayer(blackPlayerType, false);
    if (!blackPlayer || !whitePlayer) return;

    if (!hasSetup) {
        board.setupBoard();
    }

    std::cout << "New game started between " << (whitePlayerType == "human" ? "Human" : "Computer Level " + whitePlayerType.substr(8)) 
              << " (White) and " << (blackPlayerType == "human" ? "Human" : "Computer Level " + blackPlayerType.substr(8)) 
              << " (Black)." << std::endl;

    Game game(board, std::move(whitePlayer), std::move(blackPlayer));
    game.runGame();

    GameOutcome outcome = game.getOutcome();
    if (outcome == GameOutcome::WhiteWins) {
        whiteScore++;
    } else if (outcome == GameOutcome::BlackWins) {
        blackScore++;
    }
}

void GameManager::printFinalScore() const {
    std::cout << "Final Score:\n";
    std::cout << "White: " << whiteScore << "\n";
    std::cout << "Black: " << blackScore << "\n";
}
