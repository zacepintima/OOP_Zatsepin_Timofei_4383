#pragma once
#include "Spell.h"
#include <vector>
#include <memory>

class Hand {
private:
    std::vector<std::unique_ptr<Spell>> spells_;
    int maxSize_;

public:
    std::vector<int> getSpellTypeIds() const;
    explicit Hand(int maxSize);

    bool addSpell(std::unique_ptr<Spell> spell);
    void removeSpell(int index);
    Spell* getSpell(int index) const;

    bool isFull()  const noexcept;
    bool isEmpty() const noexcept;
    int  getSize() const noexcept;
    int  getMaxSize() const noexcept;

    void print() const;
};