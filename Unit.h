#pragma once
#include <string>

class Unit {
protected:
    int hp_;
    int damage_;
    int x_;
    int y_;

public:
    Unit(int hp, int damage, int startX, int startY);
    virtual ~Unit() = default;

    virtual void moveTo(int newX, int newY);
    virtual void receiveDamage(int amount);

    bool isAlive() const noexcept;

    int getHp()     const noexcept { return hp_; }
    int getDamage() const noexcept { return damage_; }
    int getX()      const noexcept { return x_; }
    int getY()      const noexcept { return y_; }

    void setPosition(int x, int y) noexcept;

    virtual std::string getUnitType() const = 0;
};