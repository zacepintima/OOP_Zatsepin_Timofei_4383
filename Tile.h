#pragma once

class Tile {
public:
    Tile() noexcept = default;
    bool isWalkable() const noexcept;
};