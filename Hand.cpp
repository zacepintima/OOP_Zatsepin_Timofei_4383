#include "Hand.h"
#include <iostream>
#include <stdexcept>

Hand::Hand(int maxSize) : maxSize_(maxSize) {
    if (maxSize <= 0) {
        throw std::invalid_argument("Hand size must be positive");
    }
}

bool Hand::addSpell(std::unique_ptr<Spell> spell) {
    if (isFull()) return false;
    spells_.push_back(std::move(spell));
    return true;
}

void Hand::removeSpell(int index) {
    if (index < 0 || index >= static_cast<int>(spells_.size())) {
        throw std::out_of_range("Invalid spell index");
    }
    spells_.erase(spells_.begin() + index);
}

Spell* Hand::getSpell(int index) const {
    if (index < 0 || index >= static_cast<int>(spells_.size())) {
        return nullptr;
    }
    return spells_[index].get();
}

bool Hand::isFull() const noexcept {
    return static_cast<int>(spells_.size()) >= maxSize_;
}

bool Hand::isEmpty() const noexcept {
    return spells_.empty();
}

int Hand::getSize() const noexcept {
    return static_cast<int>(spells_.size());
}

int Hand::getMaxSize() const noexcept {
    return maxSize_;
}

void Hand::print() const {
    if (isEmpty()) {
        std::cout << "  [Hand is empty]\n";
        return;
    }
    for (int i = 0; i < static_cast<int>(spells_.size()); i++) {
        std::cout << "  [" << i << "] " << spells_[i]->getName() << "\n";
    }
}

std::vector<int> Hand::getSpellTypeIds() const {
    std::vector<int> ids;
    for (const auto& spell : spells_)
        ids.push_back(spell->getTypeId());
    return ids;
}