#pragma once
#include "Spell.h"

class DirectDamageSpell : public Spell {
public:
    bool cast(Game& game, int targetX, int targetY) override;
    std::string getName() const override;
    int getTypeId() const override;
};