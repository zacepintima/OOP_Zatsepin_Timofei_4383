#pragma once
#include "Unit.h"

class Player : public Unit {
private:
    int score_;

public:
    Player(int startX, int startY);
    Player(int x, int y, int hp, int score); 

    void addScore(int points) noexcept;
    int  getScore() const noexcept;

    std::string getUnitType() const override;
};