#pragma once
#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& msg)
        : std::runtime_error(msg) {}
};

class SaveException : public GameException {
public:
    SaveException(const std::string& where, const std::string& reason)
        : GameException("[SaveException] in " + where + ": " + reason) {}
};

class LoadException : public GameException {
public:
    LoadException(const std::string& where, const std::string& reason)
        : GameException("[LoadException] in " + where + ": " + reason) {}
};

class CorruptedSaveException : public LoadException {
public:
    CorruptedSaveException(const std::string& where, const std::string& detail)
        : LoadException(where, "corrupted save — " + detail) {}
};