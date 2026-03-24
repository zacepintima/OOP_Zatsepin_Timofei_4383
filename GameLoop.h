#pragma once
#include "Game.h"
#include "SaveManager.h"

class GameLoop {
private:
    Game        game_;
    SaveManager saveManager_;
    bool        appRunning_;

    void showStartMenu();
    void handleInput(char input);
    void handleGameOver();
    void trySave();
    void tryLoad();
    void showHelp() const;

public:
    GameLoop();
    void run();
};