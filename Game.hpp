#pragma once

#include "raylib.h"
#include "World.hpp"
#include "Player.hpp"

class Game {
public:
    Game();
    ~Game();

    void Run();

private:
    void Update(float dt);
    void Draw();

    Camera2D camera{};
    World world;
    Player player;
};