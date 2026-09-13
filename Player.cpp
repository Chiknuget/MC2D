#include "Player.hpp"
#include "Constants.hpp"
#include <cmath>

Player::Player(Vector2 startPos)
    : position(startPos), velocity{ 0.0f, 0.0f }, onGround(false) {
}

Rectangle Player::GetRect() const {
    return Rectangle{ position.x, position.y, Constants::PlayerWidth, Constants::PlayerHeight };
}

Vector2 Player::GetPosition() const {
    return position;
}

bool Player::CollidesAt(Vector2 pos, const World& world) const {
    // Use a small epsilon rather than a whole pixel for the trailing edges
    // (right/bottom). A whole-pixel gap let the player sink almost a full
    // pixel into the ground before a collision was detected on it, which
    // produced a repeating sink-then-snap oscillation once resting.
    constexpr float epsilon = 0.01f;

    int left   = (int)floorf(pos.x / Constants::TileSize);
    int right  = (int)floorf((pos.x + Constants::PlayerWidth - epsilon) / Constants::TileSize);
    int top    = (int)floorf(pos.y / Constants::TileSize);
    int bottom = (int)floorf((pos.y + Constants::PlayerHeight - epsilon) / Constants::TileSize);

    for (int y = top; y <= bottom; y++) {
        for (int x = left; x <= right; x++) {
            if (isSolid(world.GetTile(x, y).type)) {
                return true;
            }
        }
    }
    return false;
}

void Player::Update(float dt, const World& world) {
    // Horizontal input
    velocity.x = 0.0f;
    if (IsKeyDown(KEY_A)) velocity.x -= Constants::MoveSpeed;
    if (IsKeyDown(KEY_D)) velocity.x += Constants::MoveSpeed;

    // Gravity. Reset to zero first when grounded so it doesn't quietly
    // accumulate while the player is just standing still.
    if (onGround) {
        velocity.y = 0.0f;
    }
    velocity.y += Constants::Gravity * dt;
    if (velocity.y > Constants::MaxFallSpeed) velocity.y = Constants::MaxFallSpeed;

    // Jump (only from the ground)
    if (onGround && IsKeyPressed(KEY_SPACE)) {
        velocity.y = Constants::JumpVelocity;
        onGround = false;
    }

    MoveAndCollide(dt, world);
}

void Player::MoveAndCollide(float dt, const World& world) {
    // Resolve X and Y separately so sliding along one axis while blocked on
    // the other works correctly (classic AABB-vs-tilemap approach).

    // --- X axis ---
    Vector2 newPos = position;
    newPos.x += velocity.x * dt;

    if (CollidesAt(newPos, world)) {
        if (velocity.x > 0.0f) {
            int tileX = (int)floorf((newPos.x + Constants::PlayerWidth) / Constants::TileSize);
            newPos.x = tileX * Constants::TileSize - Constants::PlayerWidth;
        } else if (velocity.x < 0.0f) {
            int tileX = (int)floorf(newPos.x / Constants::TileSize) + 1;
            newPos.x = tileX * Constants::TileSize;
        }
        velocity.x = 0.0f;
    }
    position.x = newPos.x;

    // --- Y axis ---
    newPos = position;
    newPos.y += velocity.y * dt;

    if (CollidesAt(newPos, world)) {
        if (velocity.y > 0.0f) {
            // Falling and hit something: land on top of it.
            int tileY = (int)floorf((newPos.y + Constants::PlayerHeight) / Constants::TileSize);
            newPos.y = tileY * Constants::TileSize - Constants::PlayerHeight;
        } else if (velocity.y < 0.0f) {
            // Moving up and hit a ceiling.
            int tileY = (int)floorf(newPos.y / Constants::TileSize) + 1;
            newPos.y = tileY * Constants::TileSize;
        }
        velocity.y = 0.0f;
    }
    position.y = newPos.y;

    // Ground check: an independent probe just 1 pixel below the player's
    // feet. This keeps "onGround" stable every frame instead of depending
    // on whether gravity happened to push the player into the floor by
    // enough to register a collision on that exact frame -- that dependency
    // was the source of both the camera stutter and the missed jumps.
    Vector2 probe = { position.x, position.y + 1.0f };
    onGround = CollidesAt(probe, world);
}

void Player::Draw() const {
    DrawRectangleRec(GetRect(), RED);
}