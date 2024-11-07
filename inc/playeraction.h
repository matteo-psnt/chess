#ifndef PLAYERACTION_H
#define PLAYERACTION_H

#include "position.h"
#include "pieces.h"

struct playerAction {
    Position start;
    Position end;
    Piece* piece;
    Piece* captured;
    bool isCapture;
    bool canBeCaptured;
    bool isCheck;
    bool isCheckmate;
    int score;

    playerAction(Position start, Position end, Piece* piece, Piece* captured, bool isCapture, bool canBeCaptured, bool isCheck, bool isCheckmate)
        : start(start), end(end), piece(piece), captured(captured), isCapture(isCapture), canBeCaptured(canBeCaptured), isCheck(isCheck), isCheckmate(isCheckmate) {
            score = piece->getValue() + (captured ? captured->getValue() : 0);
        }
    
    playerAction() : start(Position()), end(Position()), piece(nullptr), captured(nullptr), isCapture(false), canBeCaptured(false), isCheck(false), isCheckmate(false) {}
};

#endif // PLAYERACTION_H


