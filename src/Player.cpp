#include "Player.hpp"
#include "Animator.hpp"
#include "ResourceManager.hpp"
#include <memory>
#include <iostream>
#include "raymath.h"
#include <cmath>
#include "Utils.hpp"

#define GAME_SCREEN_WIDTH 3840
#define GAME_SCREEN_HEIGHT 2160

#define PLAYER_SPEED 100.0f
#define PLAYER_MAX_ACCELERATION 600.0f


Player::Player() {};


void Player::init() 
{   
    rect = {250, 250, 8, 8};
    position = {rect.x, rect.y};
    speed = PLAYER_SPEED;
    health = 100.0f;
    damage = 10.0f;
    texture = ResourceManager::GetInstance().GetTexture("player_acceleration");
    rotation = 0.0f;
    velocity = {0, 0};
    playerAccelerating = false;
    origin = {rect.width / 2.0f, rect.height / 2.0f};


    // ? We animate two textures, one for acceleration and one for deacceleration
    // ? Then hide the deacceleration animation until we need it
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
    Animator::GetInstance().SetOrigin("accelerating", origin);

    texture = ResourceManager::GetInstance().GetTexture("player_deacceleration");
    Animator::GetInstance().AddAnimation("deaccelerating", std::make_shared<Animation>(
        texture,
        texture.width / 3,
        texture.height,
        0.1f,
        true,
        rect,
        false,
        WHITE
    ));
    Animator::GetInstance().SetOrigin("deaccelerating", origin);
    Animator::GetInstance().Play("deaccelerating");


    playerAttackCooldown = Cooldown(0.2f); 
    playerAttackCooldown.startCooldown(); // ? prevents accidental shooting at the start of the game
    autoShoot = false;
};

void Player::update() {

#pragma region PlayerRotation

    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), utils::gameCamera); // Pass camera here

    Vector2 direction = Vector2Subtract(mouseWorldPos, position);

    if (Vector2Length(direction) == 0.0f) direction = {1, 0};
    else direction = Vector2Normalize(direction);

    float targetRotation = atan2(direction.y, direction.x) * RAD2DEG + 90.0f;

    // Normalize angles to avoid abrupt jumps
    float deltaAngle = targetRotation - rotation;
    deltaAngle = fmodf(deltaAngle + 540.0f, 360.0f) - 180.0f; // shortest path
    deltaAngle = Clamp(deltaAngle, -180.0f, 180.0f); // resolves the issue of the angle jumping from 180 to -180

    // Calculate rotation speed based on velocity (minimum 3.0f, scales up)
    float baseRotationSpeed = 3.0f;
    float maxRotationSpeed = 10.0f;
    float speedFactor = Vector2Length(velocity) / speed; // Normalize velocity (0 to 1)
    float rotationSpeed = baseRotationSpeed + speedFactor * (maxRotationSpeed - baseRotationSpeed);

    // Apply smoothing rotation
    rotation += deltaAngle * (1.0f - expf(-rotationSpeed * GetFrameTime()));

    Animator::GetInstance().SetRotation("accelerating", rotation);
    Animator::GetInstance().SetRotation("deaccelerating", rotation);

#pragma endregion



#pragma region PlayerMovement

    // Handle player movement based on keyboard input
    Vector2 facing_direction = Vector2Rotate((Vector2){0, -1}, rotation * DEG2RAD);
    Vector2 target_velocity = Vector2Scale(facing_direction, (int)(IsKeyDown(KEY_W)||IsKeyDown(KEY_SPACE)) * speed);

    // 8 direction movement (WASD)
    // Vector2 inputDirection = {0, 0};
    // if (IsKeyDown(KEY_W)) inputDirection.y -= 1.0f;
    // if (IsKeyDown(KEY_S)) inputDirection.y += 1.0f;
    // if (IsKeyDown(KEY_A)) inputDirection.x -= 1.0f;
    // if (IsKeyDown(KEY_D)) inputDirection.x += 1.0f;
    // if (Vector2Length(inputDirection) > 0.0f) {
    //     inputDirection = Vector2Normalize(inputDirection);
    // }
    // Vector2 target_velocity = Vector2Scale(inputDirection, speed);

    velocity = Vector2MoveTowards(velocity, target_velocity, PLAYER_MAX_ACCELERATION * GetFrameTime());
    position = Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));

    // update player position
    Animator::GetInstance().SetPosition("accelerating", position); 
    Animator::GetInstance().SetPosition("deaccelerating", position);


#pragma endregion 


#pragma region PlayerAnimation

    // Handle player animation based on velocity
    if (Vector2Length(velocity) > 95.0f) {
        if (!playerAccelerating) {
            Animator::GetInstance().Play("accelerating");
            playerAccelerating = true;
        }
    } else {
        if (playerAccelerating) {
            Animator::GetInstance().Stop("accelerating");
            playerAccelerating = false;
        }
    }


#pragma endregion


    if (IsKeyPressed(KEY_E)) autoShoot = !autoShoot;

    if ((IsMouseButtonDown(MOUSE_BUTTON_LEFT) || autoShoot) && !playerAttackCooldown.isOnCooldown())
    {
        Bullet bullet = Bullet(position, facing_direction, false);
        BulletManager::GetInstance().addBullet(bullet);
        playerAttackCooldown.startCooldown();
    } 
};


void Player::draw(const Texture2D& texture)
{
    // no drawing since the animator class handles it
};