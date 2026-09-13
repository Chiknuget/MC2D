#include "Game.hpp"
#include "Constants.hpp"
#include <cmath>

namespace {
    Vector2 ComputeSpawnPosition() {
        // Spawn above the flat surface (surfaceY row) so the player falls
        // onto the ground when the game starts.
        int surfaceY = Constants::WorldHeight / 2;
        float spawnX = (Constants::WorldWidth / 2) * (float)Constants::TileSize;
        float spawnY = (surfaceY - 5) * (float)Constants::TileSize;
        return Vector2{ spawnX, spawnY };
    }
}

Game::Game()
    : world(), player(ComputeSpawnPosition())
{
    InitWindow(Constants::ScreenWidth, Constants::ScreenHeight, "2D Minecraft-like - Step 3: Player + Collision");
    SetTargetFPS(60);

    camera.offset = { (float)Constants::ScreenWidth / 2.0f, (float)Constants::ScreenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = Constants::CameraZoom;
    camera.target = player.GetPosition();
}

Game::~Game() {
    CloseWindow();
}

void Game::Run() {
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Update(dt);
        Draw();
    }
}

void Game::Update(float dt) {
    player.Update(dt, world);

    // Camera follows the player's center.
    Vector2 playerCenter = {
        player.GetPosition().x + Constants::PlayerWidth / 2.0f,
        player.GetPosition().y + Constants::PlayerHeight / 2.0f
    };
    // Round to whole pixels so tile edges don't shimmer as the camera moves smoothly.
    camera.target = playerCenter;
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(SKYBLUE);

    BeginMode2D(camera);
    world.Draw(camera);
    player.Draw();
    EndMode2D();

    DrawFPS(10, 10);
    DrawText("A/D: move   Space: jump", 10, 34, 20, DARKGRAY);

    EndDrawing();
}