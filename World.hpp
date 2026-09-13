#pragma once

#include <vector>
#include "raylib.h"
#include "Tile.hpp"

class World {
    public:
        World();

        void Generate();
        void Draw(const Camera2D& camera) const;

        Tile GetTile(int x, int y) const;
        void SetTile(int x, int y, TileType type);
        bool InBounds(int x, int y) const;

    private:
        std::vector<std::vector<Tile>> tiles; // tiles[y][x]

        Color GetColorForTile(TileType type) const;
};