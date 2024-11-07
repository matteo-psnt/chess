#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "window.h"
#include "board.h"
#include <string>

class GraphicalDisplay {
    SFMLWindow window;
    Board board;

public:
    GraphicalDisplay(Board board);
    void drawBoard(const Board &newBoard);
    void updateStatus(const std::string &status);
    void processEvents();
};

#endif
