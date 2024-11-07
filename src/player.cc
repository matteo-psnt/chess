#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include "board.h"
#include "player.h"
#include "playeraction.h"

bool HumanPlayer::makeAction(Board& board) {
    std::string command;
    std::cout << "Enter your action " << (isWhite ? "(White)" : "(Black)") << ": ";
    if (!std::getline(std::cin, command)) {
        std::cout << std::endl;
        return false;
    }
    std::istringstream iss(command);
    std::string action;
    iss >> action;

    if (action == "resign") {
        std::cout << (isWhite ? "White" : "Black") << " resigns." << std::endl;
        return false;
    } else if (action == "move") {
        std::string from, to, promotion;
        iss >> from >> to >> promotion;

        if (from.length() != 2 || to.length() != 2 || from[0] < 'a' || from[0] > 'h' || from[1] < '1' || from[1] > '8' || to[0] < 'a' || to[0] > 'h' || to[1] < '1' || to[1] > '8') {
            std::cout << "Invalid position format. Use 'e2', 'd4', etc." << std::endl;
            return makeAction(board);
        }

        Position start(from[0] - 'a', from[1] - '1');
        Position end(to[0] - 'a', to[1] - '1');

        auto piece = board.at(start);
        if (!piece) {
            std::cout << "No piece at the starting position. Try again." << std::endl;
            return makeAction(board);
        }

        if (piece->getIsWhite() != isWhite) {
            std::cout << "Not your piece. Try again." << std::endl;
            return makeAction(board);
        }

        if (!board.movePiece(start, end)) {
            std::cout << "Invalid move. Try again." << std::endl;
            return makeAction(board);
        }

        if (piece->getSymbol() == (isWhite ? 'P' : 'p') && (end.y == 0 || end.y == 7)) {
            auto pawn = std::dynamic_pointer_cast<Pawn>(piece);
            pawn->promote(end, board, promotion);
        }

        return true;
    } else if (action == "print") {
        board.printBoard();
        return makeAction(board);
    } else {
        std::cout << "Unknown action. Try again." << std::endl;
        return makeAction(board);
    }
}

bool ComputerPlayer::makeAction(Board& board) {
    std::string command;
    std::cout << "Computer " << (isWhite ? "(White)" : "(Black)") << ": Enter command (resign or move): ";
    if (!std::getline(std::cin, command)) {
        std::cout << std::endl;
        return false;
    }
    std::istringstream iss(command);
    std::string action;
    iss >> action;

    if (action == "resign") {
        std::cout << (isWhite ? "White" : "Black") << " resigns." << std::endl;
        return false;
    } else if (action == "move") {
        auto actions = board.getPossibleActions(isWhite);
        std::srand(std::time(nullptr));

        playerAction randomMove;
        if (difficulty == 1) {
            // Level 1: Random legal moves
            randomMove = actions[std::rand() % actions.size()];
            board.movePiece(randomMove.start, randomMove.end);
        } else if (difficulty == 2) {
            // Level 2: Prefer capturing moves and checks over other moves
            std::vector<playerAction> captures, checks, others;
            for (const auto& action : actions) {
                if (action.isCapture) {
                    captures.push_back(action);
                } else if (action.isCheck) {
                    checks.push_back(action);
                } else {
                    others.push_back(action);
                }
            }
            if (!captures.empty()) {
                randomMove = captures[std::rand() % captures.size()];
                board.movePiece(randomMove.start, randomMove.end);
            } else if (!checks.empty()) {
                randomMove = checks[std::rand() % checks.size()];
                board.movePiece(randomMove.start, randomMove.end);
            } else {
                randomMove = others[std::rand() % others.size()];
                board.movePiece(randomMove.start, randomMove.end);
            }
        } else if (difficulty == 3) {
            // Level 3: Prefer avoiding capture, capturing moves, and checks
            std::vector<playerAction> captures, checks, safeMoves, others;
            for (const auto& action : actions) {
                if (action.isCapture) {
                    captures.push_back(action);
                } else if (action.isCheck) {
                    checks.push_back(action);
                } else if (!action.canBeCaptured) {
                    safeMoves.push_back(action);
                } else {
                    others.push_back(action);
                }
            }
            if (!safeMoves.empty()) {
                randomMove = safeMoves[std::rand() % safeMoves.size()];
                board.movePiece(randomMove.start, randomMove.end);
            } else if (!captures.empty()) {
                randomMove = captures[std::rand() % captures.size()];
                board.movePiece(randomMove.start, randomMove.end);
            } else if (!checks.empty()) {
                randomMove = checks[std::rand() % checks.size()];
                board.movePiece(randomMove.start, randomMove.end);
            } else {
                randomMove = others[std::rand() % others.size()];
                board.movePiece(randomMove.start, randomMove.end);
            }
        } else if (difficulty == 4) {
            // Level 4: Use action scores to select the best move with some randomness
            int totalScore = 0;
            for (const auto& action : actions) {
                totalScore += action.score + 1; // Add 1 to avoid zero scores
            }

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, totalScore - 1);

            int randomScore = dis(gen);
            int cumulativeScore = 0;

            for (const auto& action : actions) {
                cumulativeScore += action.score + 1;
                if (cumulativeScore > randomScore) {
                    randomMove = action;
                    break;
                }
            }

            board.movePiece(randomMove.start, randomMove.end);
        }

        auto piece = board.at(randomMove.start);
        if (piece && piece->getSymbol() == (isWhite ? 'P' : 'p') && (randomMove.end.y == 0 || randomMove.end.y == 7)) {
            auto pawn = std::dynamic_pointer_cast<Pawn>(piece);
            if (pawn) {
                pawn->promote(randomMove.end, board, "Q"); // Always promote to queen
            }
        }
        return true;
    } else {
        std::cout << "Unknown command. Try again." << std::endl;
        return makeAction(board);
    }
}
