#include "setupmanager.h"
#include "pieces.h"
#include <iostream>
#include <sstream>
#include <cctype>

SetupManager::SetupManager() : board(Board()) {}

void SetupManager::enterSetupMode() {
    std::string command;
    std::cout << "Entering setup mode. Enter 'done' when finished." << std::endl;
    while (true) {
        board.printBoard();
        std::cout << "Setup command: ";
        std::getline(std::cin, command);
        if (command == "done") {
            if (validateSetup()) {
                std::cout << "Setup complete." << std::endl;
                break;
            }
        } else {
            processSetupCommand(command);
        }
    }
}

std::shared_ptr<Piece> SetupManager::createPiece(char pieceChar) {
    bool isWhite = std::isupper(pieceChar);
    char pieceType = std::tolower(pieceChar);

    switch (pieceType) {
        case 'k':
            return std::make_shared<King>(isWhite);
        case 'q':
            return std::make_shared<Queen>(isWhite);
        case 'r':
            return std::make_shared<Rook>(isWhite);
        case 'b':
            return std::make_shared<Bishop>(isWhite);
        case 'n':
            return std::make_shared<Knight>(isWhite);
        case 'p':
            return std::make_shared<Pawn>(isWhite);
        default:
            return nullptr;
    }
}

void SetupManager::processSetupCommand(const std::string& command) {
    std::istringstream iss(command);
    char action;
    iss >> action;

    if (action == '+') {
        char piece;
        std::string position;
        iss >> piece >> position;

        // Validate position format
        if (position.length() != 2 || position[0] < 'a' || position[0] > 'h' || position[1] < '1' || position[1] > '8') {
            std::cout << "Invalid position format. Use 'e2', 'd4', etc." << std::endl;
            return;
        }

        // Validate piece
        std::string validPieces = "KQRBNPkqrbnp";
        if (validPieces.find(piece) == std::string::npos) {
            std::cout << "Invalid piece. Use 'K', 'Q', 'R', 'B', 'N', 'P' for white and 'k', 'q', 'r', 'b', 'n', 'p' for black." << std::endl;
            return;
        }

        int x = position[0] - 'a';
        int y = position[1] - '1';
        
        // Add piece to board
        board.setPiece(Position(x, y), createPiece(piece));
        std::cout << "Piece added." << std::endl;

    } else if (action == '-') {
        std::string position;
        iss >> position;

        // Validate position format
        if (position.length() != 2 || position[0] < 'a' || position[0] > 'h' || position[1] < '1' || position[1] > '8') {
            std::cout << "Invalid position format. Use 'e2', 'd4', etc." << std::endl;
            return;
        }

        int x = position[0] - 'a';
        int y = position[1] - '1';
        
        // Remove piece from board
        board.removePiece(Position(x, y));
        std::cout << "Piece removed." << std::endl;

    } else if (action == '=') {
        std::string color;
        iss >> color;

        // Validate color
        if (color == "white") {
            board.setWhiteTurn(true);
            std::cout << "Set to white start." << std::endl;
        } else if (color == "black") {
            board.setWhiteTurn(false);
            std::cout << "Set to black start." << std::endl;
        } else {
            std::cout << "Invalid color. Use 'white' or 'black'." << std::endl;
        }

    } else {
        std::cout << "Invalid setup command." << std::endl;
    }
}


bool SetupManager::validateSetup() const {
    int whiteKingCount = 0;
    int blackKingCount = 0;

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            auto piece = board.at(x, y);
            if (piece) {
                if (piece->getSymbol() == 'K') whiteKingCount++;
                if (piece->getSymbol() == 'k') blackKingCount++;
                if (piece->getSymbol() == 'P' || piece->getSymbol() == 'p') {
                    if (y == 0 || y == 7) {
                        std::cout << "Pawns cannot be on the first or last rank of the board." << std::endl;
                        return false;
                    }
                }
            }
        }
    }

    if (whiteKingCount != 1 || blackKingCount != 1) {
        std::cout << "There must be exactly one white and one black king." << std::endl;
        return false;
    }

    if (board.isCheckmate(true) || board.isCheckmate(false)) {
        std::cout << "Neither player can be in checkmate at the start of the game." << std::endl;
        return false;
    }

    return true;
}

Board SetupManager::getBoard() const {
    return board;
}
