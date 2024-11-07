#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <string>
#include "setupmanager.h"
#include "game.h"
#include "board.h"
#include "player.h"

class GameManager {
    Board board;
    SetupManager setupManager;
    bool hasSetup;
    int whiteScore;
    int blackScore;
    std::unique_ptr<Player> whitePlayer;
    std::unique_ptr<Player> blackPlayer;

    std::unique_ptr<Player> createPlayer(const std::string& playerType, bool isWhite);
    void processCommand(const std::string& command);
    void printFinalScore() const;
public:
    GameManager();
    void gameLoop();
    void startNewGame(const std::string& whitePlayerType, const std::string& blackPlayerType);
};

#endif // GAME_MANAGER_H
