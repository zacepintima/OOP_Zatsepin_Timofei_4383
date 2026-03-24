#pragma once
#include <fstream>
#include <string>
#include "GameExceptions.h"

class Game;

// RAII обёртка над файлом - закрывает файл автоматически при уничтожении
class FileHandle {
private:
    std::fstream stream_;
    std::string  path_;
public:
    FileHandle(const std::string& path, std::ios::openmode mode);
    ~FileHandle();
    std::fstream& get();
    bool isOpen() const;
};

class SaveManager {
private:
    std::string savePath_;
public:
    explicit SaveManager(const std::string& path = "savegame.txt");
    void save(const Game& game);
    void load(Game& game);
    bool saveExists() const;
};