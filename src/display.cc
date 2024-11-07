#include "display.h"

GraphicalDisplay::GraphicalDisplay(Board board)
    : window(400, 450), board(board)
{
    drawBoard(board);
}

void GraphicalDisplay::drawBoard(const Board &newBoard)
{
    window.clear(SFMLWindow::White);
    int squareSize = 50;

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            bool isWhiteSquare = (x + y) % 2 == 0;
            window.fillRectangle(x * squareSize, (7 - y) * squareSize, squareSize, squareSize, isWhiteSquare ? SFMLWindow::White : SFMLWindow::Black);

            auto newPiece = newBoard.at(Position(x, y));
            if (newPiece)
            {
                window.drawPeice(x * squareSize, (7 - y) * squareSize - 5, newPiece->getSymbol(), isWhiteSquare ? SFMLWindow::Black : SFMLWindow::White);
            }
        }
    }
    
    window.fillRectangle(0, 400, 400, 50, SFMLWindow::White);
    window.drawString(10, 410, statusMessage, SFMLWindow::Black);

    window.display();
    board = newBoard;
}

void GraphicalDisplay::updateStatus(const std::string &status) {
    statusMessage = status;
    drawBoard(board);
}

void GraphicalDisplay::processEvents()
{
    window.processEvents();
}
