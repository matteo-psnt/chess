#include "display.h"

GraphicalDisplay::GraphicalDisplay(Board board)
    : window(400, 450), board(board)
{
    int squareSize = 50;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            bool isWhiteSquare = (x + y) % 2 == 0;
            window.fillRectangle(x * squareSize, y * squareSize, squareSize, squareSize, isWhiteSquare ? SFMLWindow::White : SFMLWindow::Black);
            auto Piece = board.at(Position(x, y));
            if (Piece) {
                window.drawString(x * squareSize + 20, y * squareSize + 30, std::string(1, Piece->getSymbol()), isWhiteSquare ? SFMLWindow::Black : SFMLWindow::White);
            }
        }
    }
}

void GraphicalDisplay::drawBoard(const Board &newBoard)
{
    int squareSize = 50;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            auto oldPiece = board.at(Position(x, y));
            auto newPiece = newBoard.at(Position(x, y));
            if ((oldPiece && (!newPiece || oldPiece->getSymbol() != newPiece->getSymbol())) ||
                (!oldPiece && newPiece)) {
                bool isWhiteSquare = (x + y) % 2 == 0;
                window.fillRectangle(x * squareSize, y * squareSize, squareSize, squareSize, isWhiteSquare ? SFMLWindow::White : SFMLWindow::Black); // Clear the square
                if (newPiece) {
                    window.drawString(x * squareSize + 20, y * squareSize + 30, std::string(1, newPiece->getSymbol()), isWhiteSquare ? SFMLWindow::Black : SFMLWindow::White);
                }
            }
        }
    }
    board = newBoard;
}

void GraphicalDisplay::updateStatus(const std::string &status) {
    window.fillRectangle(0, 400, 400, 50, SFMLWindow::White);
    window.drawString(10, 430, status, SFMLWindow::Black);
}

void GraphicalDisplay::processEvents() {
    window.processEvents();
}
