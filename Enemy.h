#pragma once
#include "Unit.h"
#include <utility>

class Enemy : public Unit {
public:
    Enemy(int startX, int startY);
    Enemy(int x, int y, int hp);

    // Возвращает координаты следующего шага в сторону цели
    std::pair<int, int> nextStepToward(int targetX, int targetY) const noexcept;

    std::string getUnitType() const override;
};