#include "SaveManager.h"
#include "Game.h"
#include <iostream>

FileHandle::FileHandle(const std::string& path, std::ios::openmode mode)
    : path_(path) {
    stream_.open(path, mode);
}

FileHandle::~FileHandle() {
    if (stream_.is_open())
        stream_.close();
}

std::fstream& FileHandle::get()    { return stream_; }
bool          FileHandle::isOpen() const { return stream_.is_open(); }

SaveManager::SaveManager(const std::string& path) : savePath_(path) {}

bool SaveManager::saveExists() const {
    std::ifstream f(savePath_);
    return f.good();
}

void SaveManager::save(const Game& game) {
    FileHandle fh(savePath_, std::ios::out | std::ios::trunc);
    if (!fh.isOpen())
        throw SaveException("SaveManager::save",
            "cannot open '" + savePath_ + "' for writing");

    game.serialize(fh.get());

    if (fh.get().fail())
        throw SaveException("SaveManager::save",
            "write error in '" + savePath_ + "'");

    std::cout << "[Game saved to '" << savePath_ << "']\n";
}

void SaveManager::load(Game& game) {
    if (!saveExists())
        throw LoadException("SaveManager::load",
            "file '" + savePath_ + "' does not exist");

    FileHandle fh(savePath_, std::ios::in);
    if (!fh.isOpen())
        throw LoadException("SaveManager::load",
            "cannot open '" + savePath_ + "' for reading");

    game.deserialize(fh.get());
    std::cout << "[Game loaded from '" << savePath_ << "']\n";
}