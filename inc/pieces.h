#ifndef PIECES_H
#define PIECES_H

#include <iostream>
#include <vector>
#include <memory>
#include "position.h"

class Board;

// Abstract class for chess pieces
class Piece {
protected:
    char symbol;
    bool isWhite;
    int value;
public:
    Piece(char symbol, bool isWhite, int value);
    virtual ~Piece();
    virtual std::shared_ptr<Piece> clone() const = 0;

    virtual bool isValidMove(const Position& start, const Position& end, const Board& board) const = 0;
    virtual void move(const Position& start, const Position& end, Board& board);

    virtual std::vector<Position> getValidMoves(const Position& start, const Board& board) const;

    char getSymbol() const;
    bool getIsWhite() const;
    int getValue() const;
};

bool isWithinBounds(const Position& pos);

bool isPathClear(const Board& board, const Position& start, const Position& end);

// King
class King : public Piece {
    bool hasMoved;
public:
    King(bool isWhite);
    std::shared_ptr<Piece> clone() const override;

    bool isValidMove(const Position& start, const Position& end, const Board& board) const override;
    void move(const Position& start, const Position& end, Board& board) override;
};

// Queen
class Queen : public Piece {
public:
    Queen(bool isWhite);
    std::shared_ptr<Piece> clone() const override;

    bool isValidMove(const Position& start, const Position& end, const Board& board) const override;
};

// Rook
class Rook : public Piece {
    bool hasMoved;
public:
    Rook(bool isWhite);
    std::shared_ptr<Piece> clone() const override;

    bool isValidMove(const Position& start, const Position& end, const Board& board) const override;
    void move(const Position& start, const Position& end, Board& board) override;

    bool getHasMoved() const;
};

// Bishop
class Bishop : public Piece {
public:
    Bishop(bool isWhite);
    std::shared_ptr<Piece> clone() const override;

    bool isValidMove(const Position& start, const Position& end, const Board& board) const override;
};

// Knight
class Knight : public Piece {
public:
    Knight(bool isWhite);
    std::shared_ptr<Piece> clone() const override;

    bool isValidMove(const Position& start, const Position& end, const Board& board) const override;
};

// Pawn
class Pawn : public Piece {
    bool hasMoved;
public:
    Pawn(bool isWhite);
    std::shared_ptr<Piece> clone() const override;

    bool isValidMove(const Position& start, const Position& end, const Board& board) const override;
    void move(const Position& start, const Position& end, Board& board) override;
    void promote(const Position& end, Board& board, const std::string& promotion);
};

#endif // PIECES_H
