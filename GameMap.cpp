#include "GameMap.h"
#include "GameDefs.h"
#include <stdexcept>

GameMap::GameMap(int width, int height)
    : width_(width), height_(height)
{
    if (width  < kMinMapSize || width  > kMaxMapSize ||
        height < kMinMapSize || height > kMaxMapSize) {
        throw std::invalid_argument("Map size must be between 10 and 25");
    }
    grid_.assign(height, std::vector<Tile>(width));
}

// --- Конструктор копирования (глубокая копия через vector) ---
GameMap::GameMap(const GameMap& other)
    : width_(other.width_), height_(other.height_), grid_(other.grid_)
{}

// --- Конструктор перемещения ---
GameMap::GameMap(GameMap&& other) noexcept
    : width_(other.width_), height_(other.height_),
      grid_(std::move(other.grid_))
{
    other.width_  = 0;
    other.height_ = 0;
}

// --- Оператор копирования ---
GameMap& GameMap::operator=(const GameMap& other) {
    if (this != &other) {
        width_  = other.width_;
        height_ = other.height_;
        grid_   = other.grid_;
    }
    return *this;
}

// --- Оператор перемещения ---
GameMap& GameMap::operator=(GameMap&& other) noexcept {
    if (this != &other) {
        width_  = other.width_;
        height_ = other.height_;
        grid_   = std::move(other.grid_);
        other.width_  = 0;
        other.height_ = 0;
    }
    return *this;
}

bool GameMap::isInsideBounds(int x, int y) const noexcept {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

void GameMap::validateCoords(int x, int y) const {
    if (!isInsideBounds(x, y)) {
        throw std::out_of_range("Coordinates out of map bounds");
    }
}

bool GameMap::isTileWalkable(int x, int y) const {
    validateCoords(x, y);
    return grid_[y][x].isWalkable();
}