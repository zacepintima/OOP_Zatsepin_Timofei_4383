#include "Player.h"
#include "GameDefs.h"

Player::Player(int startX, int startY)
    : Unit(kPlayerHp, kPlayerDmg, startX, startY),
      score_(0)
{}

void Player::addScore(int points) noexcept {
    score_ += points;
}

int Player::getScore() const noexcept {
    return score_;
}

std::string Player::getUnitType() const {
    return "Player";
}

Player::Player(int x, int y, int hp, int score)
    : Unit(hp, kPlayerDmg, x, y), score_(score) {}