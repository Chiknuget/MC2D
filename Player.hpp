#pragma once

#include "raylib.h"
#include "World.hpp"

class Player {
    public:
        explicit Player(Vector2 startPos);

        void Update(float dt, const World& world);
        void Draw() const;

        Rectangle GetRect() const;
        Vector2 GetPosition() const;

    private:
        Vector2 position; // top-left corner, in puxels
        Vector2 velocity;
        bool onGround;

        void MoveAndCollide(float dt, const World& world);
        bool CollidesAt(Vector2 pos, const World& world) const;
};