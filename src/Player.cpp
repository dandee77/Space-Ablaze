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

#define PLAYER_SPEED 50.0f
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
    playerScore = 0;
    rect = {250, 250, 8, 8};
    position = {rect.x, rect.y};
    speed = PLAYER_SPEED;
    damage = 10.0f;
    rotation = 0.0f;
    velocity = {0, 0};
    playerAccelerating = false;
    playerState = PLAYER_DEFAULT;
    timeStateEntered = 0.0f;
    origin = {rect.width / 2.0f, rect.height / 2.0f};
    
    // attributes
    attackSpeed = 0.5f; 
    health = 100;
    iframeDuration = PLAYER_STATE_IFRAME_DURATION;
    rotationSpeedMultiplier = 1.0f; 

    // trigger Happy burst fire initialization
    burstCount = 1; 
    currentBurstShot = 0;
    burstDelay = 0.1f;  // 100ms delay between burst shots
    isBurstActive = false; 

    // scatter shot initialization
    scatterCount = 1;  

    // piercing rounds initialization
    bulletPiercePower = 1; 

    // Damage system initialization
    minDamage = 7;
    maxDamage = 12; 

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

    playerAttackCooldown = Cooldown(attackSpeed); 
    playerAttackCooldown.startCooldown(); // ? prevents accidental shooting at the start of the game
    autoShoot = false;
    rect.width -= 4.0f;
    rect.height -= 4.0f;

    //? we have to load one by one because raylib uses a single sound instance
    //? and it doesn't support multiple instances of the same sound
    for (int i = 0; i < MAX_SHOOTING_SOUND_INSTANCES; i++) {
        shootingSounds[i] = LoadSound("assets/sounds/player_atk_sound.mp3");
    }
};

#pragma region PlayerAttributes
// TODO: passively increase each every minute for better skill balance
// decrease by 10% 
void Player::increasePlayerAttackSpeed() {
    attackSpeed *= 0.9f;
    playerAttackCooldown.updateCooldownDuration(attackSpeed);
    TraceLog(LOG_INFO, "Player attack speed increased to: %f", attackSpeed);
}

// increase health by 10
void Player::increasePlayerHealth() {
    if (health < 100) health += 10; 
    TraceLog(LOG_INFO, "Player health increased to: %d", health);
}

// increase speed by 10%
void Player::increasePlayerMovementSpeed() {
    speed *= 1.1f; // increase speed by 10%
    TraceLog(LOG_INFO, "Player speed increased to: %f", speed);
}

// increase iframe duration by 10%
void Player::increasePlayerIframeDuration() {
    iframeDuration *= 1.1f;
    TraceLog(LOG_INFO, "Player iframe duration increased to: %f", iframeDuration);
}

// increase rotation speed by 10%
void Player::increasePlayerRotationSpeed() {
    rotationSpeedMultiplier *= 1.1f;
    TraceLog(LOG_INFO, "Player rotation speed multiplier increased to: %f", rotationSpeedMultiplier);
}

// increase burst count by 1
void Player::increaseTriggerHappyBurst() {
    burstCount++;
    TraceLog(LOG_INFO, "Player burst count increased to: %d", burstCount);
}

// increase scatter count by 1
void Player::increaseScatterShot() {
    scatterCount++;
    TraceLog(LOG_INFO, "Player scatter count increased to: %d", scatterCount);
}

// increase bullet pierce power by 1
void Player::increasePiercingRounds() {
    bulletPiercePower++;
    TraceLog(LOG_INFO, "Player bullet pierce power increased to: %d", bulletPiercePower);
}

int Player::getRandomDamage() const {
    return GetRandomValue(minDamage, maxDamage);
}


void Player::fireScatterBullets(Vector2 baseDirection) {
    if (scatterCount == 1) {
        Bullet bullet = Bullet(position, baseDirection, false, bulletPiercePower);
        BulletManager::GetInstance().addBullet(bullet);
    } else {
      
        float baseAngle = atan2(baseDirection.y, baseDirection.x);
        float angleStep = 30.0f * DEG2RAD; 
        
        if (scatterCount % 2 == 1) {
            Bullet centerBullet = Bullet(position, baseDirection, false, bulletPiercePower);
            BulletManager::GetInstance().addBullet(centerBullet);
            
            for (int i = 1; i <= scatterCount / 2; i++) {
                float leftAngle = baseAngle - (angleStep * i);
                float rightAngle = baseAngle + (angleStep * i);
                
                Vector2 leftDir = {cos(leftAngle), sin(leftAngle)};
                Vector2 rightDir = {cos(rightAngle), sin(rightAngle)};
                
                Bullet leftBullet = Bullet(position, leftDir, false, bulletPiercePower);
                Bullet rightBullet = Bullet(position, rightDir, false, bulletPiercePower);
                
                BulletManager::GetInstance().addBullet(leftBullet);
                BulletManager::GetInstance().addBullet(rightBullet);
            }
        } else {
            for (int i = 0; i < scatterCount / 2; i++) {
                float leftAngle = baseAngle - (angleStep * (i + 0.5f));
                float rightAngle = baseAngle + (angleStep * (i + 0.5f));
                
                Vector2 leftDir = {cos(leftAngle), sin(leftAngle)};
                Vector2 rightDir = {cos(rightAngle), sin(rightAngle)};
                
                Bullet leftBullet = Bullet(position, leftDir, false, bulletPiercePower);
                Bullet rightBullet = Bullet(position, rightDir, false, bulletPiercePower);
                
                BulletManager::GetInstance().addBullet(leftBullet);
                BulletManager::GetInstance().addBullet(rightBullet);
            }
        }
    }

    for (int i = 0; i < MAX_SHOOTING_SOUND_INSTANCES; i++) {
        if (!IsSoundPlaying(shootingSounds[i])) {
            PlaySound(shootingSounds[i]);
            break;
        }
    }
}

// TODO: bullet pierce power, fire rate, attack multiplier


#pragma endregion PlayerAttributes


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
        if ((GetTime() - timeStateEntered) > iframeDuration)
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

    float targetRotation = atan2(direction.y, direction.x) * RAD2DEG + 90.0f;

    float deltaAngle = targetRotation - rotation;
    
    while (deltaAngle > 180.0f) deltaAngle -= 360.0f;
    while (deltaAngle < -180.0f) deltaAngle += 360.0f;
    
    float baseRotationSpeed = 4.0f * rotationSpeedMultiplier;
    float maxRotationSpeed = 8.0f * rotationSpeedMultiplier;
    float speedFactor = Vector2Length(velocity) / speed; 
    float rotationSpeed = baseRotationSpeed + speedFactor * (maxRotationSpeed - baseRotationSpeed);

    rotation += deltaAngle * (1.0f - expf(-rotationSpeed * GetFrameTime()));

    rotation = fmodf(rotation + 360.0f, 360.0f);

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


    if (Vector2Length(velocity) > 0) {
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


#pragma region PlayerShoot

    if (IsKeyPressed(KEY_E)) autoShoot = !autoShoot;    // burst fire 
    if (isBurstActive) {
        burstTimer += GetFrameTime();
        if (burstTimer >= burstDelay && currentBurstShot < burstCount) {
 
            fireScatterBullets(facing_direction);
            
            currentBurstShot++;
            burstTimer = 0.0f; 

            if (currentBurstShot >= burstCount) {
                isBurstActive = false;
                currentBurstShot = 0;
                playerAttackCooldown.startCooldown(); 
            }
        }
    }
   
    else if ((IsMouseButtonDown(MOUSE_BUTTON_LEFT) || autoShoot) && !playerAttackCooldown.isOnCooldown()) {
     
        fireScatterBullets(facing_direction);
        
        if (burstCount > 1) {
            isBurstActive = true;
            currentBurstShot = 1; 
            burstTimer = 0.0f;
        } else {
            
            playerAttackCooldown.startCooldown();
        }
    }

#pragma endregion PlayerShoot

    rect = {position.x - rect.width / 2, position.y - rect.height / 2, rect.width, rect.height};
};


void Player::takeDamage(float timeStateEntered, Vector2 entityPosition, float entitySize) 
{
    health -= 10; 
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