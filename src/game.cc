#include <iostream>
#include <string>
#include "game.h"

Game::Game(Board board, std::unique_ptr<Player>&& whitePlayer, std::unique_ptr<Player>&& blackPlayer)
    : board(board), whitePlayer(std::move(whitePlayer)), blackPlayer(std::move(blackPlayer)), outcome(GameOutcome::Running), display(board) {}

void Game::runGame() {
    display.updateStatus("New game started!");
    while (!isGameOver()) {
        display.processEvents(); // Ensure the window processes events
        board.printBoard();
        display.drawBoard(board);
        display.processEvents(); // Keep processing events during the game

        if (board.isWhiteTurn()) {
            if (!whitePlayer->makeAction(board)) {
                std::cout << "Black wins!" << std::endl;
                display.updateStatus("Black wins!");
                outcome = GameOutcome::BlackWins;
                break;
            }
        } else {
            if (!blackPlayer->makeAction(board)) {
                std::cout << "White wins!" << std::endl;
                display.updateStatus("White wins!");
                outcome = GameOutcome::WhiteWins;
                break;
            }
        }

        display.updateStatus(" ");
        switchTurn();
    }
    display.processEvents(); // Final event processing before the game ends
}

GameOutcome Game::getOutcome() const {
    return outcome;
}

void Game::switchTurn() {
    board.setWhiteTurn(!board.isWhiteTurn());
}

bool Game::isGameOver() {
    // Check if the game is over
    if (board.isStalemate(true) || board.isCheckmate(true)) {
        std::cout << "Stalemate!" << std::endl;
        display.updateStatus("Stalemate!");
        outcome = GameOutcome::Stalemate;
        return true;
    }

    if (board.isCheckmate(true)) {
        std::cout << "Checkmate! Black wins!" << std::endl;
        display.updateStatus("Checkmate! Black wins!");
        outcome = GameOutcome::BlackWins;
        return true;
    }

    if (board.isCheckmate(false)) {
        std::cout << "Checkmate! White wins!" << std::endl;
        display.updateStatus("Checkmate! White wins!");
        outcome = GameOutcome::WhiteWins;
        return true;
    }

    if (board.isCheck(true)) {
        std::cout << "White is in check!" << std::endl;
        display.updateStatus("White is in check!");
    }

    if (board.isCheck(false)) {
        std::cout << "Black is in check!" << std::endl;
        display.updateStatus("Black is in check!");
    }

    return false;
}
