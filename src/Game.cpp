#include "Game.hpp"
#include <cmath>
#include <iostream>
#include "Animator.hpp"
#include "Bullet.hpp"
#include "raymath.h" 
#include "BulletManager.hpp"
#include "Utils.hpp"
#include "AsteroidManager.hpp"
#include <algorithm>
#include <unordered_set>


static bool CheckCollisions(Vector2 bulletPos, Vector2 shipPos, float shipSize) // circled hitbox so edit the draw hitbox
{
    return Vector2Distance(bulletPos, shipPos) <= shipSize;
}

// ! TODO: KILL COUNTER N SCALING

// TODO: COLLISION OF ENTITIES AND PLAYER NUDGES
// TODO: ENEMY PATTERN, STOP SPAWNING MOBS WHEN BOSS FIGJT
// TODO: PLAYER STATE IFRAME
// TODO: GLITTERS WHEN AUGMENT SELECTION
// TODO: SPINNING COLORS ON THE RECTANGLE ON AUGMENTS BORDER]
// todo: cursor

Game::Game() 
{}

void Game::onSwitch()
{
    worldTileSize = 500.0f; 
    playerEntity = Player();
    playerEntity.init();
    utils::gameCamera = {0};
    utils::gameCamera.target = { playerEntity.getPosition().x, playerEntity.getPosition().y };
    utils::gameCamera.offset = {GAME_SCREEN_WIDTH/2.0f, GAME_SCREEN_HEIGHT/2.0f};
    utils::gameCamera.rotation = 0.0f;
    utils::gameCamera.zoom = 150.0f;

    backgroundLayers = {
        {ResourceManager::GetInstance().GetTexture("background1"), 0.2f, worldTileSize},  // 0.2
        {ResourceManager::GetInstance().GetTexture("background3"), 0.4f, worldTileSize},  // 0.4
        {ResourceManager::GetInstance().GetTexture("background4"), 0.7f, worldTileSize},  // 0.7
        {ResourceManager::GetInstance().GetTexture("background2"), 1.0f, worldTileSize}   // 1.0
    };

    EnemyManager::GetInstance().reset();
    AsteroidManager::GetInstance().reset();

    playerBulletTexture = ResourceManager::GetInstance().GetTexture("player_bullet");
    enemyBulletTexture = ResourceManager::GetInstance().GetTexture("enemy_bullet");

    killCounter = KillCounter();
    gameTimer = GameTimer();
    gameTimer.reset();
    gameTimer.start();
}


std::string Game::update() 
{
    if (IsKeyPressed(KEY_ENTER)) return "MainMenu"; 

#pragma region StartAnimation

    float zoomLerpSpeed = 7.5f; // Higher = faster decay
    float deltaTime = GetFrameTime();
    utils::gameCamera.zoom += (20.0f - utils::gameCamera.zoom) * (1.0f - expf(-zoomLerpSpeed * deltaTime));

#pragma endregion

    // handle playr shooting

#pragma region UpdatePlayer

    playerEntity.update();
    // ?  the enemies bullet shares the same bullet max range since it is relative to the player
    BulletManager::GetInstance().update(playerEntity.getPosition()); // ?  it only gets player position to calculate the distance to the bullets

#pragma endregion


#pragma region CameraMovement

    Vector2 playerPos = playerEntity.getPosition();
    Vector2 playerVelocity = playerEntity.getVelocity();

    float leadStrength = 0.35f; 
    Vector2 leadOffset = Vector2Scale(playerVelocity, leadStrength);

    Vector2 desiredCameraTarget = Vector2Add(playerPos, leadOffset);


    float cameraLerpSpeed = 5.0f; 
    utils::gameCamera.target.x += (desiredCameraTarget.x - utils::gameCamera.target.x) * (1.0f - expf(-cameraLerpSpeed * deltaTime));
    utils::gameCamera.target.y += (desiredCameraTarget.y - utils::gameCamera.target.y) * (1.0f - expf(-cameraLerpSpeed * deltaTime));

#pragma endregion

    
    BulletManager& bm = BulletManager::GetInstance();
    EnemyManager& em = EnemyManager::GetInstance();
    AsteroidManager& am = AsteroidManager::GetInstance();


#pragma region UpdateBulletsCollision


    for (int i = 0; i < bm.getBullets().size(); i++) {
        if (!bm.getBullets()[i].isEnemyBullet()) {
            // Player bullets vs enemies
            std::vector<std::string> enemiesToRemove;
            for (auto& [id, enemy] : em.getEnemies()) {
                if (CheckCollisions(bm.getBullets()[i].getPosition(), 
                                  enemy->getPosition(), 
                                  enemy->getHitbox().width)) {
                    enemiesToRemove.push_back(id);
                    killCounter.increment();
                    break;
                }
            }
            
            // Player bullets vs asteroids
            std::vector<std::string> asteroidsToRemove;
            for (auto& [id, asteroid] : am.getAsteroids()) {
                if (CheckCollisions(bm.getBullets()[i].getPosition(),
                                   asteroid->getPosition(),
                                   asteroid->getAsteroidSize())) {
                    asteroidsToRemove.push_back(id);
                    killCounter.increment();
                    break; 
                }
            }
    

            if (!enemiesToRemove.empty() || !asteroidsToRemove.empty()) {
                bm.removeBullet(i);
                i--; 
                
                for (auto& id : enemiesToRemove) {
                    em.removeEnemy(id);
                }
                for (auto& id : asteroidsToRemove) {
                    am.removeAsteroid(id);
                }
            }
        }
        else {
            // Enemy bullets vs player
            if (CheckCollisions(bm.getBullets()[i].getPosition(),
                               playerEntity.getPosition(),
                               playerEntity.getHitbox().width) &&
                               playerEntity.getPlayerState() == PLAYER_DEFAULT) {
                bm.removeBullet(i);
                playerEntity.takeDamage(GetTime(), bm.getBullets()[i].getPosition(), 3.0f);
                i--; 
                continue;
            }
            
            // Enemy bullets vs asteroids
            std::vector<std::string> asteroidsToRemove;
            for (auto& [id, asteroid] : am.getAsteroids()) {
                if (CheckCollisions(bm.getBullets()[i].getPosition(),
                                  asteroid->getPosition(),
                                  asteroid->getAsteroidSize())) {
                    asteroidsToRemove.push_back(id);
                    break; 
                }
            }
    
            if (!asteroidsToRemove.empty()) {
                bm.removeBullet(i);
                i--;
                
                for (auto& id : asteroidsToRemove) {
                    am.removeAsteroid(id);
                }
            }
        }
    }
    

#pragma endregion



#pragma region UpdateAsteroids

   
    am.update(playerEntity.getPosition());
    // TraceLog(LOG_INFO, "Asteroid count: %d", asteroids.size());

#pragma endregion



    EnemyManager::GetInstance().update(playerEntity.getPosition()); //! <<***********************



#pragma region UpdateEntitiesCollision

    std::vector<std::string> enemiesToRemove;
    std::vector<std::string> asteroidsToRemove;
 
    for (auto& [enemyId, enemy] : EnemyManager::GetInstance().getEnemies()) {   
        
        // player to enemy collision
        if (CheckCollisions(enemy->getPosition(), playerEntity.getPosition(), playerEntity.getHitbox().width)
            && playerEntity.getPlayerState() == PLAYER_DEFAULT) {
            playerEntity.takeDamage(GetTime(), enemy->getPosition(), enemy->getHitbox().width);
            enemiesToRemove.push_back(enemyId);
            continue; 
        }
        
        // enemy to asteroid collision
        for (auto& [asteroidId, asteroid] : AsteroidManager::GetInstance().getAsteroids()) {
            if (CheckCollisions(enemy->getPosition(), asteroid->getPosition(), asteroid->getAsteroidSize())) {
                enemiesToRemove.push_back(enemyId);
                asteroidsToRemove.push_back(asteroidId);
                break; 
            }
        }
    }
    
    // player to asteroid collision
    for (auto& [asteroidId, asteroid] : AsteroidManager::GetInstance().getAsteroids()) {
        if (CheckCollisions(asteroid->getPosition(), playerEntity.getPosition(), playerEntity.getHitbox().width) 
            && playerEntity.getPlayerState() == PLAYER_DEFAULT) {
            playerEntity.takeDamage(GetTime(), asteroid->getPosition(), asteroid->getAsteroidSize());
            asteroidsToRemove.push_back(asteroidId);
        }
    }
    
    // asteroid to asteroid collision
    std::unordered_set<std::string> collidedAsteroids;
    for (auto& [id1, asteroid1] : AsteroidManager::GetInstance().getAsteroids()) {
        if (collidedAsteroids.count(id1)) continue; 
        
        for (auto& [id2, asteroid2] : AsteroidManager::GetInstance().getAsteroids()) {
            if (id1 == id2 || collidedAsteroids.count(id2)) continue;
            
            if (CheckCollisions(asteroid1->getPosition(), asteroid2->getPosition(), asteroid1->getAsteroidSize())) {
                collidedAsteroids.insert(id1);
                collidedAsteroids.insert(id2);
                break; 
            }
        }
    }
    
    std::sort(asteroidsToRemove.begin(), asteroidsToRemove.end());
    asteroidsToRemove.erase(std::unique(asteroidsToRemove.begin(), asteroidsToRemove.end()), asteroidsToRemove.end());
    
    for (const auto& id : collidedAsteroids) {
        if (std::find(asteroidsToRemove.begin(), asteroidsToRemove.end(), id) == asteroidsToRemove.end()) {
            asteroidsToRemove.push_back(id);
        }
    }
    
    for (const auto& id : enemiesToRemove) {
        em.removeEnemy(id);
    }
    
    for (const auto& id : asteroidsToRemove) {
        am.removeAsteroid(id);
    }
    
#pragma endregion

    killCounter.update();

    return "Game";
}



void Game::DrawLayer(const BackgroundLayer& layer, const Vector2& viewCenter) 
{
    Vector2 parallaxOffset = {
        viewCenter.x * (1.0f - layer.parallaxFactor),
        viewCenter.y * (1.0f - layer.parallaxFactor)
    };

    int startX = static_cast<int>(std::floor((viewCenter.x - parallaxOffset.x) / layer.size));
    int startY = static_cast<int>(std::floor((viewCenter.y - parallaxOffset.y) / layer.size));

    for (int y = -1; y <= 1; y++) { 
        for (int x = -1; x <= 1; x++) {
            Vector2 tilePos = {
                (startX + x) * layer.size + parallaxOffset.x,
                (startY + y) * layer.size + parallaxOffset.y
            };

            DrawTextureV(layer.texture, tilePos, WHITE); // TODO: MAKE IT RED WHEN THE BOSS APPEARS
        }
    }
}


void Game::DrawParallaxBackground()
{
    Vector2 viewCenter = utils::gameCamera.target;
    
    for (const auto& layer : backgroundLayers) {
        DrawLayer(layer, viewCenter);
    }
}

void Game::draw()
{
    BeginMode2D(utils::gameCamera);

        DrawParallaxBackground();
        BulletManager::GetInstance().draw(playerBulletTexture, enemyBulletTexture);

        // ? no draw function for the player as it is handled in the animator class
        playerEntity.draw();

        EnemyManager::GetInstance().draw();
        AsteroidManager::GetInstance().draw();
        
        Animator::GetInstance().Update();
        Animator::GetInstance().Draw(); 


        // std::cout << "Player Position: (" << playerEntity.getPosition().x << ", " << playerEntity.getPosition().y << ")" << std::endl;

        

    EndMode2D();

    killCounter.draw();
    gameTimer.draw(GAME_SCREEN_WIDTH);
}

void Game::onExit()
{

}
