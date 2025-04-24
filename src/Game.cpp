#include "Game.hpp"
#include <cmath>
#include <iostream>
#include "Animator.hpp"
#include "Bullet.hpp"
#include "raymath.h" 
#include "BulletManager.hpp"
#include "Utils.hpp"


static bool intersectBullet(Vector2 bulletPos, Vector2 shipPos, float shipSize)
{
    return Vector2Distance(bulletPos, shipPos) <= shipSize;
}

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

    playerBulletTexture = ResourceManager::GetInstance().GetTexture("player_bullet");
    enemyBulletTexture = ResourceManager::GetInstance().GetTexture("enemy_bullet");
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

    // playerEntity.update(camera);

    // Get player position and velocity
    Vector2 playerPos = playerEntity.getPosition();
    Vector2 playerVelocity = playerEntity.getVelocity();

    // Calculate lead offset based on velocity
    float leadStrength = 0.35f; // Tweak for how far ahead the camera looks
    Vector2 leadOffset = Vector2Scale(playerVelocity, leadStrength);

    // Target position is ahead of the player in movement direction
    Vector2 desiredCameraTarget = Vector2Add(playerPos, leadOffset);

    // Smooth camera follow with exponential smoothing
    float cameraLerpSpeed = 5.0f; // Higher = faster catch-up
    utils::gameCamera.target.x += (desiredCameraTarget.x - utils::gameCamera.target.x) * (1.0f - expf(-cameraLerpSpeed * deltaTime));
    utils::gameCamera.target.y += (desiredCameraTarget.y - utils::gameCamera.target.y) * (1.0f - expf(-cameraLerpSpeed * deltaTime));

#pragma endregion

    
    BulletManager& bm = BulletManager::GetInstance();
    EnemyManager& em = EnemyManager::GetInstance();
    for (int i = 0; i < bm.getBullets().size(); i++)
    {
        if (!bm.getBullets()[i].isEnemyBullet()) 
        {
            bool breakBothLoops = false;
            for (int e = 0; e < em.getEnemies().size(); e++)
            {
                if (intersectBullet(bm.getBullets()[i].getPosition(), em.getEnemies()[e]->getPosition(), em.getEnemies()[e]->getHitbox().width)) 
                {
                    bm.removeBullet(i);
                    em.removeEnemy(e);
                    i--;
                    breakBothLoops = true;
                    break;
                }
            }

            if (breakBothLoops) continue;
        }
        else
        {
            if (intersectBullet(bm.getBullets()[i].getPosition(), playerEntity.getPosition(), playerEntity.getHitbox().width)) 
            {
                bm.removeBullet(i);
                playerEntity.takeDamage(); 
                i--;
                continue;
            }
        }
    }


    EnemyManager::GetInstance().update(playerEntity.getPosition()); //! <<***********************

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
        
        Animator::GetInstance().Update();
        Animator::GetInstance().Draw(); 

        
        // std::cout << "Player Position: (" << playerEntity.getPosition().x << ", " << playerEntity.getPosition().y << ")" << std::endl;

    EndMode2D();
}

void Game::onExit()
{

}