#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <memory>
#include "position.h"

// Forward declaration
class Board;
class Piece;

class Player {
protected:
    bool isWhite;
public:
    Player(bool isWhite) : isWhite(isWhite) {}
    virtual ~Player() = default;

    virtual bool makeAction(Board& board) = 0;
};

class HumanPlayer : public Player {
public:
    HumanPlayer(bool isWhite) : Player(isWhite) {}
    bool makeAction(Board& board) override;
};

class ComputerPlayer : public Player {
    int difficulty;

public:
    ComputerPlayer(bool isWhite, int difficulty) : Player(isWhite), difficulty(difficulty) {}
    bool makeAction(Board& board) override;
};

#endif // PLAYER_H
