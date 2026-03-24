#include "DirectDamageSpell.h"
#include "Game.h"
#include "GameDefs.h"
#include <iostream>
#include <cmath>
#include <algorithm>

bool DirectDamageSpell::cast(Game& game, int targetX, int targetY) {
    int px = game.getPlayerX();
    int py = game.getPlayerY();

    int dist = std::max(std::abs(px - targetX), std::abs(py - targetY));
    if (dist > kDirectSpellRange) {
        std::cout << "Target out of range! (max: " << kDirectSpellRange << ")\n";
        return false;
    }

    bool hit = game.dealDamageAt(targetX, targetY, kSpellDirectDmg);
    if (!hit) {
        std::cout << "No enemy at (" << targetX << ", " << targetY
                  << ")! Spell not cast.\n";
        return false;
    }

    std::cout << "Direct Damage hits (" << targetX << ", " << targetY
              << ") for " << kSpellDirectDmg << " dmg!\n";
    return true;
}

std::string DirectDamageSpell::getName() const {
    return "Direct Damage (dmg: " + std::to_string(kSpellDirectDmg)
         + ", range: " + std::to_string(kDirectSpellRange) + ")";
}

int DirectDamageSpell::getTypeId() const { return 0; }