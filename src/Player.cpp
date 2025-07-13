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

#define PLAYER_SPEED 70.0f
#define PLAYER_MAX_ACCELERATION 400.0f

#define PLAYER_STATE_STUN_DURATION 0.2f
#define PLAYER_STATE_IFRAME_DURATION 0.8f
#define PLAYER_COLLISION_NUDGE 20.0f


Player::Player() : playerState(PLAYER_DEFAULT), timeStateEntered(0.0f) {};

// TODO: MAKE BULLETS SPAWNING FRAME INDEPENDENT


// ** SKILLS **
// - Player movement
// - Player rotationSPEED ?
// - Player shooting CD
// - Player BULLETS MULTIPLIER
// - Player health
// - Player damage
// - Player IFRAME DURATION
// - PLAYER BODY DAMAGE?
// - bullet penetration

void Player::init() 
{   
    rect = {250, 250, 8, 8};
    position = {rect.x, rect.y};
    speed = PLAYER_SPEED;
    health = 100.0f;
    damage = 10.0f;
    rotation = 0.0f;
    velocity = {0, 0};
    playerAccelerating = false;
    playerState = PLAYER_DEFAULT;
    timeStateEntered = 0.0f;
    origin = {rect.width / 2.0f, rect.height / 2.0f};

    // ? We animate two textures, one for acceleration and one for deacceleration
    // ? Then hide the deacceleration animation until we need it
    Animator::GetInstance().AddAnimation("accelerating", std::make_shared<Animation>(
        ResourceManager::GetInstance().GetTextureRef("player_acceleration"),
        ResourceManager::GetInstance().GetTextureRef("player_acceleration").width / 4,
        ResourceManager::GetInstance().GetTextureRef("player_acceleration").height,
        0.1f,
        true,
        rect,
        false,
        WHITE
    ));
    Animator::GetInstance().SetOrigin("accelerating", origin);
    Animator::GetInstance().AddAnimation("deaccelerating", std::make_shared<Animation>(
        ResourceManager::GetInstance().GetTextureRef("player_deacceleration"),
        ResourceManager::GetInstance().GetTextureRef("player_deacceleration").width / 3,
        ResourceManager::GetInstance().GetTextureRef("player_deacceleration").height,
        0.1f,
        true,
        rect,
        false,
        WHITE
    ));
    Animator::GetInstance().SetOrigin("deaccelerating", origin);
    Animator::GetInstance().Play("deaccelerating");


    playerAttackCooldown = Cooldown(0.1f); 
    playerAttackCooldown.startCooldown(); // ? prevents accidental shooting at the start of the game
    autoShoot = false;
    rect.width -= 4.0f;
    rect.height -= 4.0f;
};


void Player::update() {

#pragma region HandlePlayerState

    switch(playerState) 
    {
    case PLAYER_DEFAULT:
        // no check
        // todo: add all of update logic here
        break;
    case PLAYER_STUNNED:
        if ((GetTime() - timeStateEntered) > PLAYER_STATE_STUN_DURATION) 
        {
            playerState = PLAYER_IFRAME;
            timeStateEntered = GetTime();
        }
        break;
    case PLAYER_IFRAME:
        if ((GetTime() - timeStateEntered) > PLAYER_STATE_IFRAME_DURATION)
        {
            playerState = PLAYER_DEFAULT;
            Animator::GetInstance().SetTint("accelerating", WHITE);
            Animator::GetInstance().SetTint("deaccelerating", WHITE);
        }
        else
        {
            float seconds = GetTime() - timeStateEntered;
            int value = (int)(seconds * 6.0f);
            if (value % 2 == 0) 
            {
                Animator::GetInstance().SetTint("accelerating", RED);
                Animator::GetInstance().SetTint("deaccelerating", RED);
            } 
            else 
            {
                Animator::GetInstance().SetTint("accelerating", WHITE);
                Animator::GetInstance().SetTint("deaccelerating", WHITE);
            }
        }
        break;
    case PLAYER_DEAD:
        // no check for now
        // todo: pass the game over function
        // return "GameOver";
        break;
    }

#pragma endregion



#pragma region PlayerRotation

    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), utils::gameCamera); 

    Vector2 direction = Vector2Subtract(mouseWorldPos, position);

    if (Vector2Length(direction) == 0.0f) direction = {1, 0};
    else direction = Vector2Normalize(direction);

    // ? standard rotation
    float rotation = atan2f(direction.y, direction.x) * RAD2DEG + 90.0f; 

    // // ! SMOOTHING ROTATION WAS BUGGED ASF
    // // ? IT WORKS NOW? SOMEHOW IDEK
    // float targetRotation = atan2(direction.y, direction.x) * RAD2DEG + 90.0f;

    // // Normalize angles to avoid abrupt jumps
    // float deltaAngle = targetRotation - rotation;
    // deltaAngle = fmodf(deltaAngle + 180.0f, 360.0f) - 180.0f;
    // // deltaAngle = fmodf(deltaAngle + 540.0f, 360.0f) - 180.0f; // shortest path
    // // // ! PLAYER STILL JUMPS FROM 180 TO -180
    // // deltaAngle = Clamp(deltaAngle, -179.0f, 179.0f); // resolves the issue of the angle jumping from 180 to -180

    // // Calculate rotation speed based on velocity (minimum 3.0f, scales up) 
    // float baseRotationSpeed = 5.0f;
    // float maxRotationSpeed = 10.0f;
    // float speedFactor = Vector2Length(velocity) / speed; 
    // float rotationSpeed = baseRotationSpeed + speedFactor * (maxRotationSpeed - baseRotationSpeed);

    // // Apply smoothing rotation
    // rotation += deltaAngle * (1.0f - expf(-rotationSpeed * GetFrameTime()));

    Animator::GetInstance().SetRotation("accelerating", rotation);
    Animator::GetInstance().SetRotation("deaccelerating", rotation);

#pragma endregion


#pragma region PlayerMovement

    Vector2 facing_direction = Vector2Rotate((Vector2){0, -1}, rotation * DEG2RAD);
    // Vector2 target_velocity = Vector2Scale(facing_direction, (int)(IsKeyDown(KEY_W)||IsKeyDown(KEY_SPACE)) * speed);

    // 8 direction movement 
    Vector2 inputDirection = {0, 0};
    if (IsKeyDown(KEY_W)) inputDirection.y -= 1.0f;
    if (IsKeyDown(KEY_S)) inputDirection.y += 1.0f;
    if (IsKeyDown(KEY_A)) inputDirection.x -= 1.0f;
    if (IsKeyDown(KEY_D)) inputDirection.x += 1.0f;
    if (Vector2Length(inputDirection) > 0.0f) {
        inputDirection = Vector2Normalize(inputDirection);
    }
    Vector2 target_velocity = Vector2Scale(inputDirection, speed);

    velocity = Vector2MoveTowards(velocity, target_velocity, PLAYER_MAX_ACCELERATION * GetFrameTime());
    position = Vector2Add(position, Vector2Scale(velocity, GetFrameTime()));


    Animator::GetInstance().SetPosition("accelerating", position); 
    Animator::GetInstance().SetPosition("deaccelerating", position);


#pragma endregion 


#pragma region PlayerAnimation


    if (Vector2Length(velocity) > 65.0f) {
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
        
        Sound shootSound = ResourceManager::GetInstance().GetSound("player_shoot");
        PlaySound(shootSound);
    } 


    rect = {position.x - rect.width / 2, position.y - rect.height / 2, rect.width, rect.height};
};


void Player::takeDamage(float timeStateEntered, Vector2 entityPosition, float entitySize) 
{
    playerState = PLAYER_STUNNED;
    this->timeStateEntered = timeStateEntered;
    Vector2 nudgeDirection = Vector2Normalize(Vector2Subtract(position, entityPosition));
    velocity = Vector2Scale(nudgeDirection, PLAYER_COLLISION_NUDGE * entitySize); 
    Animator::GetInstance().SetTint("accelerating", RED);
    Animator::GetInstance().SetTint("deaccelerating", RED);
}


void Player::draw()
{
    // DrawRectangleRec(rect, WHITE); // ? for debugging purposes
    // no drawing since the animator class handles it
};