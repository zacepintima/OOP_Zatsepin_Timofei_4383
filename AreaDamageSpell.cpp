#include "AreaDamageSpell.h"
#include "Game.h"
#include "GameDefs.h"
#include <iostream>
#include <cmath>
#include <algorithm>

bool AreaDamageSpell::cast(Game& game, int targetX, int targetY) {
    int px = game.getPlayerX();
    int py = game.getPlayerY();

    int dist = std::max(std::abs(px - targetX), std::abs(py - targetY));
    if (dist > kAreaSpellRange) {
        std::cout << "Target out of range! (max: " << kAreaSpellRange << ")\n";
        return false;
    }

    // Используется всегда — даже если никого нет в области
    std::cout << "Area Damage hits 2x2 at (" << targetX << ", " << targetY
              << ") for " << kSpellAreaDmg << " dmg!\n";
    game.dealDamageInArea(targetX, targetY, kSpellAreaDmg);
    return true;
}

std::string AreaDamageSpell::getName() const {
    return "Area Damage 2x2 (dmg: " + std::to_string(kSpellAreaDmg)
         + ", range: " + std::to_string(kAreaSpellRange) + ")";
}

int AreaDamageSpell::getTypeId() const { return 1; }