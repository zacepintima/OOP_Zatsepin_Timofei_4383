#pragma once
#include <string>

class Game;

class Spell {
public:
    virtual ~Spell() = default;
    virtual bool cast(Game& game, int targetX, int targetY) = 0;
    virtual std::string getName()  const = 0;
    virtual int getTypeId() const = 0; 
};