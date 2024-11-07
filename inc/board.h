#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <memory>
#include "player.h"
#include "pieces.h"
#include "playeraction.h"

class Board {
    std::vector<std::vector<std::shared_ptr<Piece>>> squares;
    bool whiteTurn;
    std::pair<Position, Position> lastMove;

public:
    Board();
    Board(const Board& other);
    Board& operator=(const Board& other);
    
    std::shared_ptr<Piece> at(const Position& pos) const;
    std::shared_ptr<Piece> at(const int x, const int y) const;

    void setupBoard();
    void clearBoard();
    void printBoard() const;
    bool isWhiteTurn() const;
    void setWhiteTurn(bool whiteTurn);

    std::shared_ptr<Piece> removePiece(const Position& pos);
    void setPiece(const Position& pos, std::shared_ptr<Piece> piece);
    bool movePiece(const Position& start, const Position& end);
    
    bool isCheck(bool isWhite) const;
    bool isCheckmate(bool isWhite) const;
    bool isStalemate(bool isWhite) const;

    bool canBeCaptured(const Position &position, const bool isWhite) const;
    std::vector<playerAction> getPossibleActions(bool isWhite) const;
    std::pair<Position, Position> getLastMove() const;
};

#endif // BOARD_H
