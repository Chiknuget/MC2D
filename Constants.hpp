#pragma once

namespace Constants {
    constexpr int ScreenWidth = 1280;
    constexpr int ScreenHeight = 720;

    constexpr int TileSize = 32;

    // World size in tiles (not pixels)
    constexpr int WorldWidth = 200;
    constexpr int WorldHeight = 80;

    // Player
    constexpr float PlayerWidth = 24.0f;
    constexpr float PlayerHeight = 44.0f;
    constexpr float MoveSpeed = 200.0f; // pixels/sec
    constexpr float JumpVelocity = -450.0f; // pixels/sec
    constexpr float Gravity = 900.0f; // pixels/sec^2
    constexpr float MaxFallSpeed = 800.0f; // pixels/sec

    // Camera
    constexpr float CameraZoom = 2.0f; // >1 = zoomed in, things look bigger
}