#pragma once
#include "Tile.h"
#include <vector>

class GameMap {
private:
    std::vector<std::vector<Tile>> grid_;
    int width_;
    int height_;

    void validateCoords(int x, int y) const;

public:
    GameMap(int width, int height);

    // Конструкторы копирования и перемещения
    GameMap(const GameMap& other);
    GameMap(GameMap&& other) noexcept;

    // Операторы присваивания
    GameMap& operator=(const GameMap& other);
    GameMap& operator=(GameMap&& other) noexcept;

    bool isInsideBounds(int x, int y) const noexcept;
    bool isTileWalkable(int x, int y) const;

    int getWidth()  const noexcept { return width_; }
    int getHeight() const noexcept { return height_; }
};