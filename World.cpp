#include "World.hpp"
#include "Constants.hpp"
#include <algorithm>

World::World() {
    tiles.resize(Constants::WorldHeight, std::vector<Tile>(Constants::WorldWidth));
    Generate();
}

void World::Generate() {
    // Placeholder terrain: flat grass surface with dirt then stone below.
    // This gets replaced by noise-based generation in a later step.
    int surfaceY = Constants::WorldHeight / 2;

    for (int x = 0; x < Constants::WorldWidth; x++) {
        for (int y = 0; y < Constants::WorldHeight; y++) {
            if (y < surfaceY) {
                tiles[y][x].type = TileType::Air;
            } else if (y == surfaceY) {
                tiles[y][x].type = TileType::Grass;
            } else if (y < surfaceY + 4) {
                tiles[y][x].type = TileType::Dirt;
            } else {
                tiles[y][x].type = TileType::Stone;
            }
        }
    }
}

bool World::InBounds(int x, int y) const {
    return x >= 0 && x < Constants::WorldWidth && y >= 0 && y < Constants::WorldHeight;
}

Tile World::GetTile(int x, int y) const {
    if (!InBounds(x, y)) return Tile{ TileType::Air };
    return tiles[y][x];
}

void World::SetTile(int x, int y, TileType type) {
    if (!InBounds(x, y)) return;
    tiles[y][x].type = type;
}

Color World::GetColorForTile(TileType type) const {
    switch (type) {
        case TileType::Grass: return GREEN;
        case TileType::Dirt:  return BROWN;
        case TileType::Stone: return GRAY;
        default:              return BLANK; // Air is never drawn
    }
}

void World::Draw(const Camera2D& camera) const {
    // Only draw tiles visible on screen instead of the whole world every frame.
    Vector2 topLeft = GetScreenToWorld2D({ 0, 0 }, camera);
    Vector2 bottomRight = GetScreenToWorld2D(
        { (float)Constants::ScreenWidth, (float)Constants::ScreenHeight }, camera);

    int minX = (int)(topLeft.x / Constants::TileSize) - 1;
    int maxX = (int)(bottomRight.x / Constants::TileSize) + 1;
    int minY = (int)(topLeft.y / Constants::TileSize) - 1;
    int maxY = (int)(bottomRight.y / Constants::TileSize) + 1;

    minX = std::max(minX, 0);
    minY = std::max(minY, 0);
    maxX = std::min(maxX, Constants::WorldWidth - 1);
    maxY = std::min(maxY, Constants::WorldHeight - 1);

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            TileType type = tiles[y][x].type;
            if (type == TileType::Air) continue;

            Color color = GetColorForTile(type);
            int px = x * Constants::TileSize;
            int py = y * Constants::TileSize;

            DrawRectangle(px, py, Constants::TileSize, Constants::TileSize, color);
            DrawRectangleLines(px, py, Constants::TileSize, Constants::TileSize, Fade(BLACK, 0.15f));
        }
    }
}