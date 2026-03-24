#include "GameLoop.h"
#include "GameExceptions.h"
#include <iostream>
#include <limits>

static int readInt(const std::string& prompt) {
    int v;
    while (true) {
        std::cout << prompt;
        std::cin >> v;
        if (std::cin.good()) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return v;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input, try again.\n";
    }
}

GameLoop::GameLoop() : appRunning_(true) {}

void GameLoop::showHelp() const {
    std::cout << "  W/A/S/D - move\n"
              << "  C       - cast spell\n"
              << "  F       - save game\n"
              << "  L       - load save\n"
              << "  H       - help\n"
              << "  Q       - quit\n";
}

void GameLoop::trySave() {
    try {
        saveManager_.save(game_);
    } catch (const SaveException& e) {
        std::cout << "Save failed: " << e.what() << "\n";
    }
}

void GameLoop::tryLoad() {
    try {
        saveManager_.load(game_);
    } catch (const LoadException& e) {
        std::cout << "Load failed: " << e.what() << "\n";
    }
}

void GameLoop::showStartMenu() {
    std::cout << "\n=== TACTICAL GAME ===\n"
              << "1 - New Game\n";
    if (saveManager_.saveExists())
        std::cout << "2 - Load Save\n";
    std::cout << "0 - Quit\n"
              << "Choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 2 && saveManager_.saveExists()) {
        tryLoad();
    } else if (choice == 0) {
        appRunning_ = false;
    }
    // 1 или другое — новая игра (уже создана в конструкторе)
}

void GameLoop::handleGameOver() {
    std::cout << "1 - Restart\n";
    if (saveManager_.saveExists())
        std::cout << "2 - Load Save\n";
    std::cout << "0 - Quit\n"
              << "Choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == 1) {
        game_ = Game();
        std::cout << "=== New game started! ===\n";
        showHelp();
    } else if (choice == 2 && saveManager_.saveExists()) {
        game_ = Game();
        tryLoad();
    } else {
        appRunning_ = false;
    }
}

void GameLoop::handleInput(char input) {
    switch (input) {
        case 'w': case 'W': game_.processPlayerMove(Direction::Up);    break;
        case 's': case 'S': game_.processPlayerMove(Direction::Down);  break;
        case 'a': case 'A': game_.processPlayerMove(Direction::Left);  break;
        case 'd': case 'D': game_.processPlayerMove(Direction::Right); break;
        case 'c': case 'C': {
            game_.printHand();
            if (!game_.isRunning()) break;
            int idx = readInt("Spell number: ");
            int tx  = readInt("Target X: ");
            int ty  = readInt("Target Y: ");
            game_.processCastSpell(idx, tx, ty);
            break;
        }
        case 'f': case 'F': trySave(); break;
        case 'l': case 'L': {
            std::cout << "Load save? Current progress will be lost. (y/n): ";
            char confirm;
            std::cin >> confirm;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (confirm == 'y' || confirm == 'Y') tryLoad();
            break;
        }
        case 'h': case 'H': showHelp(); break;
        case 'q': case 'Q': {
            std::cout << "Quit? (y/n): ";
            char confirm;
            std::cin >> confirm;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (confirm == 'y' || confirm == 'Y') appRunning_ = false;
            break;
        }
        default:
            std::cout << "Unknown key. Press H for help.\n";
            break;
    }
}

void GameLoop::run() {
    showStartMenu();
    if (!appRunning_) return;

    std::cout << "\n=== LEVEL START ===\n";
    showHelp();

    while (appRunning_) {
        if (!game_.isRunning()) {
            handleGameOver();
            if (appRunning_)
                std::cout << "\n=== NEW LEVEL START ===\n";
            continue;
        }

        game_.printMap();
        game_.printStatus();
        std::cout << "\n(W/A/S/D | C=spell | F=save | L=load | H=help | Q=quit): ";

        char input;
        std::cin >> input;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        handleInput(input);
    }

    std::cout << "\nThanks for playing!\n";
}