#pragma once

enum class TileType {
    Air,
    Grass,
    Dirt,
    Stone,
};

struct Tile {
    TileType type = TileType::Air;
};

inline bool isSolid(TileType type) {
    return type != TileType::Air;
}