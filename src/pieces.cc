#include "pieces.h"
#include "board.h"

// Piece implementation
Piece::Piece(char symbol, bool isWhite, int value) : symbol(symbol), isWhite(isWhite), value(value) {}
Piece::~Piece() {}

void Piece::move(const Position& start, const Position& end, Board& board) {
    board.setPiece(end, board.removePiece(start));
}

char Piece::getSymbol() const { return symbol; }
bool Piece::getIsWhite() const { return isWhite; }
int Piece::getValue() const { return value; }

std::vector<Position> Piece::getValidMoves(const Position& start, const Board& board) const {
    std::vector<Position> validMoves;
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            Position end(x, y);
            if (isValidMove(start, end, board)) {
                Board testBoard(board);
                testBoard.movePiece(start, end);
                if (!testBoard.isCheck(isWhite)) {
                    validMoves.push_back(end);
                }
            }
        }
    }
    return validMoves;
}

bool isWithinBounds(const Position& pos) {
    return pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8;
}

bool isPathClear(const Board& board, const Position& start, const Position& end) {
    int dx = (end.x > start.x) ? 1 : (end.x < start.x) ? -1 : 0;
    int dy = (end.y > start.y) ? 1 : (end.y < start.y) ? -1 : 0;
    int x = start.x + dx;
    int y = start.y + dy;
    while (x != end.x || y != end.y) {
        if (board.at(x, y)) return false;
        x += dx;
        y += dy;
    }
    return true;
}

// King implementation
King::King(bool isWhite) : Piece(isWhite ? 'K' : 'k', isWhite, 100), hasMoved(false) {}

inline bool isRookInPositionForCastling(const Board& board, int x, int y, bool isWhite) {
    auto piece = board.at(x, y);
    return piece && piece->getSymbol() == (isWhite ? 'R' : 'r') && !dynamic_cast<Rook*>(piece.get())->getHasMoved();
}

inline bool isCastleSafe(const Board& board, const Position& start, const Position& end, bool isWhite) {
    int direction = (end.x == 6) ? 1 : -1;

    // Check all positions between the start and end positions (inclusive of end position)
    for (int x = start.x; x != end.x + direction; x += direction) {
        Position currentPos(x, start.y);
        Board testBoard = board; // Copy constructor for Board
        testBoard.setPiece(currentPos, testBoard.removePiece(start));
        if (testBoard.isCheck(isWhite)) {
            return false;
        }
    }

    return true;
}


bool King::isValidMove(const Position& start, const Position& end, const Board& board) const {
    if (!isWithinBounds(end) || (start == end)) return false;
    if (board.at(end) && board.at(end)->getIsWhite() == isWhite) return false;

    int dx = abs(start.x - end.x);
    int dy = abs(start.y - end.y);

        if (dx <= 1 && dy <= 1) {
        return true;
    }

    // Check for castling
    if (!hasMoved && dy == 0 && dx == 2) {
        if (end.x == 6) { // King-side castling
            if (isRookInPositionForCastling(board, 7, start.y, isWhite) && isPathClear(board, start, end) && isCastleSafe(board, start, end, isWhite)) {
                return true;
            }
        } else if (end.x == 2) { // Queen-side castling
            if (isRookInPositionForCastling(board, 0, start.y, isWhite) && isPathClear(board, start, end) && !board.at(1, start.y) && isCastleSafe(board, start, end, isWhite)) {
                return true;
            }
        }
    }

    return false;
}

void King::move(const Position& start, const Position& end, Board& board) {
    if (abs(start.x - end.x) == 2) {
        // Castling move
        if (end.x == 6) { // King-side castling
            Rook *rook = dynamic_cast<Rook*>(board.at(7, end.y).get());
            rook->move(Position(7, end.y), Position(5, end.y), board);
        } else if (end.x == 2) { // Queen-side castling
            Rook *rook = dynamic_cast<Rook*>(board.at(0, end.y).get());
            rook->move(Position(0, end.y), Position(3, end.y), board);
        }
    }

    // Move the king
    Piece::move(start, end, board);

    // Mark the king as having moved
    hasMoved = true;
}

// Queen implementation
Queen::Queen(bool isWhite) : Piece(isWhite ? 'Q' : 'q', isWhite, 9) {}

bool Queen::isValidMove(const Position& start, const Position& end, const Board& board) const {
    if (!isWithinBounds(end)) return false;
    if (board.at(end) && board.at(end)->getIsWhite() == isWhite) return false;

    int dx = abs(start.x - end.x);
    int dy = abs(start.y - end.y);

    // Rook-like move (horizontal or vertical)
    if (start.x == end.x || start.y == end.y) {
        return isPathClear(board, start, end);
    }

    // Bishop-like move (diagonal)
    if (dx == dy) {
        return isPathClear(board, start, end);
    }

    // If not a valid rook-like or bishop-like move, return false
    return false;
}

// Rook implementation
Rook::Rook(bool isWhite) : Piece(isWhite ? 'R' : 'r', isWhite, 5), hasMoved(false) {}

bool Rook::isValidMove(const Position& start, const Position& end, const Board& board) const {
    if (!isWithinBounds(end)) return false;
    if (board.at(end) && board.at(end)->getIsWhite() == isWhite) return false;

    // Rook can only move horizontally or vertically
    if (start.x == end.x || start.y == end.y) {
        return isPathClear(board, start, end);
    }

    // If not a valid rook move, return false
    return false;
}

void Rook::move(const Position& start, const Position& end, Board& board) {
    hasMoved = true;
    Piece::move(start, end, board);
}

bool Rook::getHasMoved() const { return hasMoved; }

// Bishop implementation
Bishop::Bishop(bool isWhite) : Piece(isWhite ? 'B' : 'b', isWhite, 3) {}

bool Bishop::isValidMove(const Position& start, const Position& end, const Board& board) const {
    if (!isWithinBounds(end)) return false;
    if (board.at(end) && board.at(end)->getIsWhite() == isWhite) return false;

    int dx = abs(start.x - end.x);
    int dy = abs(start.y - end.y);

    // Bishop can only move diagonally
    if (dx == dy) {
        return isPathClear(board, start, end);
    }

    // If not a valid bishop move, return false
    return false;
}

// Knight implementation
Knight::Knight(bool isWhite) : Piece(isWhite ? 'N' : 'n', isWhite, 3) {}

bool Knight::isValidMove(const Position& start, const Position& end, const Board& board) const {
    if (!isWithinBounds(end)) return false;
    if (board.at(end) && board.at(end)->getIsWhite() == isWhite) return false;

    int dx = abs(start.x - end.x);
    int dy = abs(start.y - end.y);

    // Knight moves in an "L" shape: 2 squares in one direction and 1 square in the perpendicular direction
    if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
        return true;
    }

    // If not a valid knight move, return false
    return false;
}

// Pawn implementation
Pawn::Pawn(bool isWhite) : Piece(isWhite ? 'P' : 'p', isWhite, 1), hasMoved(false) {}

bool Pawn::isValidMove(const Position& start, const Position& end, const Board& board) const {
    if (!isWithinBounds(end)) return false;
    if (board.at(end) && board.at(end)->getIsWhite() == isWhite) return false;

    int dx = end.x - start.x;
    int dy = end.y - start.y;  // Correcting the use of y for vertical movement

    // Determine direction based on color
    int forward = isWhite ? 1 : -1;

    // Normal move (one square forward)
    if (dy == forward && dx == 0 && !board.at(end)) {
        return true;
    }

    // First move (two squares forward)
    if (!hasMoved && dy == 2 * forward && dx == 0 && !board.at(end) && !board.at(end.x, start.y + forward)) {
        return true;
    }

    // Capturing move (diagonal forward)
    if (dy == forward && abs(dx) == 1 && board.at(end) && board.at(end)->getIsWhite() != isWhite) {
        return true;
    }

    // En passant move
    auto lastMove = board.getLastMove();
    if (dy == forward && abs(dx) == 1 && !board.at(end) && 
        board.at(end.x, start.y) && 
        board.at(end.x, start.y)->getIsWhite() != isWhite && 
        board.at(end.x, start.y)->getSymbol() == (isWhite ? 'p' : 'P') &&
        lastMove.first == Position(end.x, start.y + (2 * forward)) &&
        lastMove.second == Position(end.x, start.y)) {
        return true;
    }
    
    // If not a valid pawn move, return false
    return false;
}


void Pawn::move(const Position& start, const Position& end, Board& board) {
    // En passant move
    if (abs(start.x - end.x) == 1 && abs(start.y - end.y) == 1 && !board.at(end)) {
        board.removePiece(Position(end.x, start.y));
    }

    // Move the pawn
    Piece::move(start, end, board);

    // Mark the pawn as having moved
    hasMoved = true;
}

void Pawn::promote(const Position &end, Board &board, const std::string& promotion) {
    if (promotion == "Q") {
        board.setPiece(end, std::make_shared<Queen>(isWhite));
    } else if (promotion == "R") {
        board.setPiece(end, std::make_shared<Rook>(isWhite));
    } else if (promotion == "B") {
        board.setPiece(end, std::make_shared<Bishop>(isWhite));
    } else if (promotion == "N") {
        board.setPiece(end, std::make_shared<Knight>(isWhite));
    } else {
        std::cout << "Invalid promotion piece. Defaulting to Queen." << std::endl;
        board.setPiece(end, std::make_shared<Queen>(isWhite));
    }
}

std::shared_ptr<Piece> King::clone() const {
    return std::make_shared<King>(*this);
}

std::shared_ptr<Piece> Queen::clone() const {
    return std::make_shared<Queen>(*this);
}

std::shared_ptr<Piece> Rook::clone() const {
    return std::make_shared<Rook>(*this);
}

std::shared_ptr<Piece> Bishop::clone() const {
    return std::make_shared<Bishop>(*this);
}

std::shared_ptr<Piece> Knight::clone() const {
    return std::make_shared<Knight>(*this);
}

std::shared_ptr<Piece> Pawn::clone() const {
    return std::make_shared<Pawn>(*this);
}