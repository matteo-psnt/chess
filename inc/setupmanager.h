#ifndef SETUPMANAGER_H
#define SETUPMANAGER_H

#include "board.h"
#include <string>

class SetupManager {
    Board board;
    bool validateSetup() const;
    void processSetupCommand(const std::string& command);
    std::shared_ptr<Piece> createPiece(char pieceChar);

public:
    SetupManager();
    void enterSetupMode();
    Board getBoard() const;
};

#endif // SETUPMANAGER_H
