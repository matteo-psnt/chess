#include "board.h"
#include <unordered_set>
#include <iostream>

Board::Board() : squares(8, std::vector<std::shared_ptr<Piece>>(8, nullptr)), whiteTurn(true), lastMove({Position(0, 0), Position(0, 0)}) {}

Board::Board(const Board& other) : squares(8, std::vector<std::shared_ptr<Piece>>(8, nullptr)) {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (other.squares[y][x]) {
                squares[y][x] = other.squares[y][x]->clone(); // Ensure each piece is also copied
            }
        }
    }
    this->whiteTurn = other.whiteTurn;
    this->lastMove = other.lastMove;

}

Board& Board::operator=(const Board& other) {
    if (this == &other) return *this; // Handle self-assignment

    // Deep copy of the board
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (other.squares[y][x]) {
                squares[y][x] = other.squares[y][x]->clone();
            } else {
                squares[y][x] = nullptr;
            }
        }
    }

    this->whiteTurn = other.whiteTurn;
    this->lastMove = other.lastMove;
    return *this;
}

std::shared_ptr<Piece> Board::at(const Position& pos) const {
    return squares[pos.y][pos.x];
}

std::shared_ptr<Piece> Board::at(const int x, const int y) const {
    return squares[y][x];
}

void Board::setupBoard() {
    // Placing white pieces
    squares[0][0] = std::make_shared<Rook>(true);
    squares[0][1] = std::make_shared<Knight>(true);
    squares[0][2] = std::make_shared<Bishop>(true);
    squares[0][3] = std::make_shared<Queen>(true);
    squares[0][4] = std::make_shared<King>(true);
    squares[0][5] = std::make_shared<Bishop>(true);
    squares[0][6] = std::make_shared<Knight>(true);
    squares[0][7] = std::make_shared<Rook>(true);

    // Placing black pieces
    squares[7][0] = std::make_shared<Rook>(false);
    squares[7][1] = std::make_shared<Knight>(false);
    squares[7][2] = std::make_shared<Bishop>(false);
    squares[7][3] = std::make_shared<Queen>(false);
    squares[7][4] = std::make_shared<King>(false);
    squares[7][5] = std::make_shared<Bishop>(false);
    squares[7][6] = std::make_shared<Knight>(false);
    squares[7][7] = std::make_shared<Rook>(false);

    // Placing pawns
    for (int i = 0; i < 8; i++) {
        squares[1][i] = std::make_shared<Pawn>(true);
        squares[6][i] = std::make_shared<Pawn>(false);
    }
}

void Board::clearBoard() {
    for (auto &row : squares) {
        std::fill(row.begin(), row.end(), nullptr);
    }
}

void Board::printBoard() const {
    for (int y = 7; y >= 0; --y) {
        std::cout << y + 1 << " ";
        for (int x = 0; x < 8; ++x) {
            if (squares[y][x]) {
                std::cout << squares[y][x]->getSymbol() << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl;
}

bool Board::isWhiteTurn() const {
    return whiteTurn;
}

void Board::setWhiteTurn(bool whiteTurn) {
    this->whiteTurn = whiteTurn;
}

void Board::setPiece(const Position& pos, std::shared_ptr<Piece> piece) {
    squares[pos.y][pos.x] = piece;
}

std::shared_ptr<Piece> Board::removePiece(const Position& pos) {
    auto piece = squares[pos.y][pos.x];
    squares[pos.y][pos.x] = nullptr;
    return piece;
}

bool Board::movePiece(const Position& start, const Position& end) {
    auto piece = at(start);
    if (!piece || !piece->isValidMove(start, end, *this)) {
        return false;
    }

    Board testBoard(*this);
    testBoard.at(start).get()->move(start, end, testBoard);
    if (testBoard.isCheck(piece->getIsWhite())) {
        return false;
    }

    piece->move(start, end, *this);
    lastMove = {start, end};
    return true;
}

bool Board::isCheck(bool isWhite) const {
    Position kingPos;
    bool kingFound = false;

    // Find the king's position
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            auto piece = at(x, y);
            if (piece && piece->getSymbol() == (isWhite ? 'K' : 'k')) {
                kingPos = Position(x, y);
                kingFound = true;
                break;
            }
        }
        if (kingFound) break;
    }

    // Check if any opponent's piece can attack the king
    return canBeCaptured(kingPos, isWhite);
}

bool Board::isCheckmate(bool isWhite) const {
    if (!isCheck(isWhite)) {
        return false;
    }

    // Check if there is any valid move to get out of check
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Position start(x, y);
            auto piece = at(start);
            if (piece && piece->getIsWhite() == isWhite) {
                auto validMoves = piece->getValidMoves(start, *this);
                for (const auto& end : validMoves) {
                    Board testBoard(*this);
                    testBoard.movePiece(start, end);
                    if (!testBoard.isCheck(isWhite)) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool Board::isStalemate(bool isWhite) const {
    if (isCheck(isWhite)) {
        return false;
    }

    // Check if there are any valid moves available
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Position start(x, y);
            auto piece = at(start);
            if (piece && piece->getIsWhite() == isWhite) {
                auto validMoves = piece->getValidMoves(start, *this);
                for (const auto& end : validMoves) {
                    Board testBoard(*this);
                    testBoard.movePiece(start, end);
                    if (!testBoard.isCheck(isWhite)) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool Board::canBeCaptured(const Position& position, const bool isWhite) const {
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            auto piece = at(x, y);
            if (piece && piece->getIsWhite() != isWhite) {
                Position start(x, y);
                if (piece->isValidMove(start, position, *this)) {
                    return true;
                }
            }
        }
    }
    return false;
}

std::vector<playerAction> Board::getPossibleActions(bool isWhite) const {
    std::vector<playerAction> actions;

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Position start(x, y);
            auto piece = at(start);
            if (piece && piece->getIsWhite() == isWhite) {
                auto validMoves = piece->getValidMoves(start, *this);
                for (const auto& end : validMoves) {
                    Board testBoard(*this);
                    auto capturedPiece = testBoard.at(end);
                    bool isCapture = capturedPiece != nullptr;
                    testBoard.movePiece(start, end);
                    bool canBeCaptured = testBoard.canBeCaptured(end, isWhite);
                    bool isCheck = testBoard.isCheck(isWhite);
                    bool isCheckmate = testBoard.isCheckmate(!isWhite);

                    actions.push_back(playerAction(start, end, piece.get(), capturedPiece.get(), isCapture, canBeCaptured, isCheck, isCheckmate));
                }
            }
        }
    }

    return actions;
}

std::pair<Position,Position> Board::getLastMove() const {
    return lastMove;
}
