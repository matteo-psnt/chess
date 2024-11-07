#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include "board.h"
#include "player.h"
#include "display.h"

enum class GameOutcome {
    Running,
    WhiteWins,
    BlackWins,
    Stalemate
};

class Game {
    Board board;
    std::unique_ptr<Player> whitePlayer;
    std::unique_ptr<Player> blackPlayer;
    GameOutcome outcome;
    GraphicalDisplay display;

    void switchTurn();
    bool isGameOver();
public:
    Game(Board board, std::unique_ptr<Player>&& whitePlayer, std::unique_ptr<Player>&& blackPlayer);
    void runGame();
    GameOutcome getOutcome() const;
};


#endif // GAME_H
