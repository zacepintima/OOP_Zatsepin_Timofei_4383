#include "Enemy.h"
#include "GameDefs.h"
#include <cmath>

Enemy::Enemy(int startX, int startY)
    : Unit(kEnemyHp, kEnemyDmg, startX, startY)
{}

std::pair<int, int> Enemy::nextStepToward(int targetX, int targetY) const noexcept {
    int dx = targetX - x_;
    int dy = targetY - y_;

    int stepX = 0, stepY = 0;

    if (std::abs(dx) >= std::abs(dy)) {
        stepX = (dx > 0) ? 1 : (dx < 0 ? -1 : 0);
    } else {
        stepY = (dy > 0) ? 1 : (dy < 0 ? -1 : 0);
    }

    return { x_ + stepX, y_ + stepY };
}

std::string Enemy::getUnitType() const {
    return "Enemy";
}

Enemy::Enemy(int x, int y, int hp)
    : Unit(hp, kEnemyDmg, x, y) {}