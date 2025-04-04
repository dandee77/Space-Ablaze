#include "Player.hpp"
#include "Animator.hpp"
#include "ResourceManager.hpp"
#include <memory>
#include <iostream>
#include "raymath.h"

#define GAME_SCREEN_WIDTH 3840
#define GAME_SCREEN_HEIGHT 2160

#define PLAYER_SPEED 200.0f
#define PLAYER_MAX_ACCELERATION 750.0f


Player::Player() {};


void Player::init() {
    // Initialize player properties here if needed
    rect = {250, 250, 5, 5};
    speed = PLAYER_SPEED;
    health = 100.0f;
    damage = 10.0f;
    texture = ResourceManager::GetInstance().GetTexture("player_acceleration");
    rotation = 0.0f;
    velocity = {0, 0};

    Animator::GetInstance().AddAnimation("accelerating", std::make_shared<Animation>(
        texture,
        texture.width / 4,
        texture.height,
        0.1f,
        true,
        rect,
        false,
        WHITE
    ));

    Animator::GetInstance().SetOrigin("accelerating", {2.5f, 2.5f});
    Animator::GetInstance().Play("accelerating");
};


bool Player::update() {

    // Handle player rotation based on mouse position
    Vector2 screenCenter = { GAME_SCREEN_WIDTH / 2.0f, GAME_SCREEN_HEIGHT / 2.0f };
    Vector2 mousePos = GetMousePosition();
    Vector2 direction = Vector2Subtract(mousePos, screenCenter);

    if (Vector2Length(direction) == 0.0f) direction = {1, 0};
    else direction = Vector2Normalize(direction);
    float targetRotation = atan2(direction.y, direction.x) * RAD2DEG + 90.0f;

    // Normalize angles to avoid abrupt jumps
    float deltaAngle = targetRotation - rotation;
    deltaAngle = fmodf(deltaAngle + 540.0f, 360.0f) - 180.0f; // shortest path

    // Calculate rotation speed based on velocity (minimum 5.0f, scales up)
    float baseRotationSpeed = 3.0f;
    float maxRotationSpeed = 10.0f;
    float speedFactor = Vector2Length(velocity) / speed; // Normalize velocity (0 to 1)
    float rotationSpeed = baseRotationSpeed + speedFactor * (maxRotationSpeed - baseRotationSpeed);

    // Apply smoothing rotation
    rotation += deltaAngle * (1.0f - expf(-rotationSpeed * GetFrameTime()));


    Animator::GetInstance().SetRotation("accelerating", rotation);

    // Handle player movement based on keyboard input
    Vector2 facing_direction = Vector2Rotate((Vector2){0, -1}, rotation * DEG2RAD);
    Vector2 target_velocity = Vector2Scale(facing_direction, (int)IsKeyDown(KEY_W) * speed);
    velocity = Vector2MoveTowards(velocity, target_velocity, PLAYER_MAX_ACCELERATION * GetFrameTime());
    rect.x += velocity.x * GetFrameTime();
    rect.y += velocity.y * GetFrameTime();


    // update player position
    Animator::GetInstance().SetPosition("accelerating", {rect.x, rect.y}); 


    return false; // No shooting logic for now
};


void Player::draw() {


};