#include "Unit.h"
#include <stdexcept>

Unit::Unit(int hp, int damage, int startX, int startY)
    : hp_(hp), damage_(damage), x_(startX), y_(startY)
{
    if (hp <= 0) {
        throw std::invalid_argument("HP must be positive");
    }
}

void Unit::moveTo(int newX, int newY) {
    x_ = newX;
    y_ = newY;
}

void Unit::receiveDamage(int amount) {
    if (amount < 0) return;
    hp_ -= amount;
    if (hp_ < 0) hp_ = 0;
}

bool Unit::isAlive() const noexcept {
    return hp_ > 0;
}

void Unit::setPosition(int x, int y) noexcept {
    x_ = x;
    y_ = y;
}