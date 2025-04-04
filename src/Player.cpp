#include "Player.hpp"
#include "Animator.hpp"
#include "ResourceManager.hpp"
#include <memory>
#include <iostream>
#include "raymath.h"

#define GAME_SCREEN_WIDTH 3840
#define GAME_SCREEN_HEIGHT 2160


Player::Player() {};


void Player::init() {
    // Initialize player properties here if needed
    rect = {250, 250, 10, 10};
    speed = 5.0f;
    health = 100.0f;
    damage = 10.0f;
    texture = ResourceManager::GetInstance().GetTexture("player_acceleration");
    rotation = 0.0f;

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

    Animator::GetInstance().SetOrigin("accelerating", {5, 5});
    Animator::GetInstance().Play("accelerating");
};


bool Player::update() {

    Vector2 movement = {0, 0};
    if (IsKeyDown(KEY_W)) movement.y -= speed;
    if (IsKeyDown(KEY_S)) movement.y += speed;
    if (IsKeyDown(KEY_A)) movement.x -= speed;
    if (IsKeyDown(KEY_D)) movement.x += speed;

    rect.x += movement.x;
    rect.y += movement.y;


    // Handle player rotation based on mouse position
    Vector2 screenCenter = { GAME_SCREEN_WIDTH / 2.0f, GAME_SCREEN_HEIGHT / 2.0f };
    Vector2 mousePos = GetMousePosition();
    Vector2 direction = Vector2Subtract(mousePos, screenCenter);

    if (Vector2Length(direction) == 0.0f) direction = {1, 0};
    else direction = Vector2Normalize(direction);
    rotation = atan2(direction.y, direction.x) * RAD2DEG + 90.0f;

    Animator::GetInstance().SetRotation("accelerating", rotation);

    // update player position
    Animator::GetInstance().SetPosition("accelerating", {rect.x, rect.y}); 


    return false; // No shooting logic for now
};


void Player::draw() {


};