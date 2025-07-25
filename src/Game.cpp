#include "Game.hpp"
#include <cmath>
#include <iostream>
#include <vector>
#include "Animator.hpp"
#include "Bullet.hpp"
#include "raymath.h" 
#include "BulletManager.hpp"
#include "Utils.hpp"
#include "AsteroidManager.hpp"
#include <algorithm>
#include <unordered_set>
#include <memory>
#include "DamageIndicatorManager.hpp"
#include "ConfettiManager.hpp"
#include "ExplosionConfettiManager.hpp"
#include "GameOverOverlay.hpp"
#include "DamageOverlay.hpp"


static bool CheckCollisions(Vector2 bulletPos, Vector2 shipPos, float shipSize) // circled hitbox so edit the draw hitbox
{
    return Vector2Distance(bulletPos, shipPos) <= shipSize;
}

// TODO: ENEMY PATTERN, STOP SPAWNING MOBS WHEN BOSS FIGJT
// TODO: GLITTERS WHEN AUGMENT SELECTION
// TODO: SPINNING COLORS ON THE RECTANGLE ON AUGMENTS BORDER]
// todo: cursor
static int prevGameTimer = 0;
static bool gameOverTextStarted = false;

Game::Game() 
{
    restartButton = nullptr;
    mainMenuButton = nullptr;
}

Game::~Game()
{
    if (restartButton) delete restartButton;
    if (mainMenuButton) delete mainMenuButton;
}

void Game::onSwitch()
{
    gameState = GAME_RUNNING;
    gameOverTextStarted = false;
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

    gameMusic = ResourceManager::GetInstance().GetMusic("game_music");
    PlayMusicStream(gameMusic);

    EnemyManager::GetInstance().reset();
    AsteroidManager::GetInstance().reset();
    BulletManager::GetInstance().reset();
    DamageIndicatorManager::GetInstance().reset();
    ConfettiManager::GetInstance().reset();
    ExplosionConfettiManager::GetInstance().reset();
    GameOverOverlay::GetInstance().reset();
    DamageOverlay::GetInstance().reset();

    playerBulletTexture = ResourceManager::GetInstance().GetTexture("player_bullet");
    enemyBulletTexture = ResourceManager::GetInstance().GetTexture("enemy_bullet");

    killCounter = KillCounter();
    killCounter.reset();
    gameTimer = GameTimer();
    gameTimer.reset();
    gameTimer.start();

    //? setup control guides keyboard keys animations
    Animator::GetInstance().AddAnimation("wasd_keys_tutorial",std::make_shared<Animation>(
        ResourceManager::GetInstance().GetTextureRef("wasd_keys"),
        ResourceManager::GetInstance().GetTextureRef("wasd_keys").width / 3,
        ResourceManager::GetInstance().GetTextureRef("wasd_keys").height,
        0.5f,
        true,
        Rectangle{0, 0, 16, 10},
        false,
        WHITE
    ));

    Animator::GetInstance().AddAnimation("e_key_tutorial", std::make_shared<Animation>(
        ResourceManager::GetInstance().GetTextureRef("e_key"),
        ResourceManager::GetInstance().GetTextureRef("e_key").width / 3,
        ResourceManager::GetInstance().GetTextureRef("e_key").height,
        0.5f,
        true,
        Rectangle{0, 0, 5, 5},
        false,
        WHITE
    ));

    Animator::GetInstance().AddAnimation("game_over_text", std::make_shared<Animation>(
        ResourceManager::GetInstance().GetTextureRef("game_over_text"),
        ResourceManager::GetInstance().GetTextureRef("game_over_text").width / 10,
        ResourceManager::GetInstance().GetTextureRef("game_over_text").height,
        0.5f,
        true,
        Rectangle{0, 0, 1800, 300},
        false,
        WHITE
    ));

    Animator::GetInstance().Play("wasd_keys_tutorial");
    Animator::GetInstance().Play("e_key_tutorial");

    for (int i = 0; i < MAX_SOUND_INSTANCES; i++) {
        scoreSounds[i] = LoadSound("assets/sounds/score_sound.wav");
    }

    // Initialize game over buttons
    if (restartButton) delete restartButton;
    if (mainMenuButton) delete mainMenuButton;
    restartButton = new Button(Rectangle{GAME_SCREEN_WIDTH/2.0f - 400, 1600, 800, 120}, "RESTART", 120, BUTTON_CENTERED);
    mainMenuButton = new Button(Rectangle{GAME_SCREEN_WIDTH/2.0f - 400, 1780, 800, 120}, "MAIN MENU", 120, BUTTON_CENTERED);

    augmentCards.clear();
}


void Game::restartGame() { //ts kinda useless ngl
    gameOverTextStarted = false;
    
    PlayMusicStream(gameMusic);
    
    EnemyManager::GetInstance().reset();
    AsteroidManager::GetInstance().reset();
    BulletManager::GetInstance().reset();
    DamageIndicatorManager::GetInstance().reset();
    ConfettiManager::GetInstance().reset();
    ExplosionConfettiManager::GetInstance().reset();
    GameOverOverlay::GetInstance().reset();
    DamageOverlay::GetInstance().reset();
    
    Animator::GetInstance().StopAll();
    // Clean up the killer display animation if it exists
    Animator::GetInstance().Remove("killer_display");

    playerEntity.init();
    
    killCounter.reset();
    gameTimer.reset();
    gameTimer.start();
    augmentCards.clear();
    
    Animator::GetInstance().Play("wasd_keys_tutorial");
    Animator::GetInstance().Play("e_key_tutorial");
    
    gameState = GAME_RUNNING;
}


std::string Game::update() 
{
    // if (IsKeyPressed(KEY_ENTER)) return "MainMenu"; 
    UpdateMusicStream(gameMusic);

    if (IsKeyPressed(KEY_P)) {
        if (gameState == GAME_RUNNING) {
            gameTimer.pause();
            gameState = GAME_PAUSED;
        } else if (gameState == GAME_PAUSED) {
            gameTimer.unpause();
            gameState = GAME_RUNNING;
        }
    }

    // std::cout << "Game Timer: " << (int)gameTimer.getElapsedTime() << " seconds" << std::endl;

    switch (gameState) {
    case GAME_RUNNING:
    {
#pragma region AugmentSelection

        if ((int)gameTimer.getElapsedTime() % 20 == 0 && (int)gameTimer.getElapsedTime() > 0 && (int)gameTimer.getElapsedTime() != prevGameTimer) {
            
            std::vector<int> usedIndices;
            
            float cardWidth = 1200.0f;
            float cardHeight = 300.0f;
            float gap = 75.0f;
            
            float centerX = GAME_SCREEN_WIDTH / 2.0f - cardWidth / 2.0f;
            float centerY = GAME_SCREEN_HEIGHT / 2.0f - cardHeight / 2.0f;
            
            std::vector<Vector2> cardPositions = {
                {centerX, centerY - cardHeight - gap},   
                {centerX, centerY},                       
                {centerX, centerY + cardHeight + gap}     
            };
            
            for (int i = 0; i < 3; i++) {
                int randomIndex;
                do {
                    randomIndex = rand() % utils::augmentPool.size();
                } while (std::find(usedIndices.begin(), usedIndices.end(), randomIndex) != usedIndices.end());
                
                usedIndices.push_back(randomIndex);
                
                augmentCards.push_back(Card(
                    cardPositions[i], 
                    {cardWidth, cardHeight}, 
                    utils::augmentPool[randomIndex][0], 
                    utils::augmentPool[randomIndex][1]
                ));
            }
            
            for (auto& card : augmentCards) {
                card.startIntroAnimation();
            }

            ConfettiManager::GetInstance().startConfetti();

            prevGameTimer = (int)gameTimer.getElapsedTime();
            gameTimer.pause();
            gameState = GAME_AUGMENT_SELECTION;
        }

#pragma endregion AugmentSelection

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
        DamageIndicatorManager::GetInstance().update();
        ConfettiManager::GetInstance().update();
        DamageOverlay::GetInstance().update();

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
                bool hitSomething = false;
                
                for (auto& [id, enemy] : em.getEnemies()) {
                    if (CheckCollisions(bm.getBullets()[i].getPosition(), 
                                    enemy->getPosition(), 
                                    enemy->getHitbox().width)) {
                        
                        int damage = playerEntity.getRandomDamage();
                        enemy->takeDamage(damage);
                        playerEntity.addDamageDealt(damage);
                        
                        if (bm.getBullets()[i].getKnockbackPower() > 0.0f && enemy->canReceiveKnockback()) {
                            enemy->applyKnockback(bm.getBullets()[i].getDirection(), 
                                                bm.getBullets()[i].getKnockbackPower());
                        }
                        
                        DamageIndicatorManager::GetInstance().addDamageIndicator(
                            enemy->getPosition(), damage, playerEntity.getMaxDamage());
            
                        
                        if (enemy->isDead()) {
                            killCounter.increment();
                            for (int j = 0; j < MAX_SOUND_INSTANCES; j++) {
                                if (!IsSoundPlaying(scoreSounds[j])) {
                                    PlaySound(scoreSounds[j]);
                                    break;
                                }
                            }
                            switch (enemy->getEnemyType()) {
                            case LOW_LEVEL_ENEMY:
                                playerEntity.addScore(10);
                                break;
                            case MID_LEVEL_ENEMY:
                                playerEntity.addScore(50);
                                break;
                            default:
                                playerEntity.addScore(5);
                                break;
                            }
                            enemiesToRemove.push_back(id);
                        }
                        
                        hitSomething = true;
                        break;
                    }
                }
                
                // Player bullets vs asteroids
                std::vector<std::string> asteroidsToRemove;
                if (!hitSomething) {
                    for (auto& [id, asteroid] : am.getAsteroids()) {
                        if (CheckCollisions(bm.getBullets()[i].getPosition(),
                                        asteroid->getPosition(),
                                        asteroid->getAsteroidSize())) {
                            for (int j = 0; j < MAX_SOUND_INSTANCES; j++) {
                                if (!IsSoundPlaying(scoreSounds[j])) {
                                    PlaySound(scoreSounds[j]);
                                    break;
                                }
                            }
                            killCounter.increment();
                            playerEntity.addScore(5);
                            asteroidsToRemove.push_back(id);
                            hitSomething = true;
                            break; 
                        }
                    }
                }
        

                if (hitSomething) {
   
                    bm.getBullets()[i].reducePiercePower();
                    
                    if (!bm.getBullets()[i].canPierce()) {
                        bm.removeBullet(i);
                        i--; 
                    }
                    
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
                    Vector2 bulletPosition = bm.getBullets()[i].getPosition(); 
                    bm.removeBullet(i);
                    playerEntity.takeDamage(GetTime(), bulletPosition, 3.0f);
                    if (playerEntity.getHealth() <= 0) {
                        deathInfo.killerType = KILLER_ENEMY_BULLET;
                        deathInfo.killerID = "enemy_bullet";
                        deathInfo.killerName = "Enemy Bullet";
                        deathInfo.killerTexture = ResourceManager::GetInstance().GetTexture("enemy_bullet");
                        gameState = GAME_OVER;
                        ExplosionConfettiManager::GetInstance().startExplosion(playerEntity.getPosition());
                        GameOverOverlay::GetInstance().startOverlay();
                        PlaySound(ResourceManager::GetInstance().GetSound("game_over"));
                        StopMusicStream(gameMusic);
                    }
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

    
        am.update(playerEntity.getPosition(), gameTimer); 
        // TraceLog(LOG_INFO, "Asteroid count: %d", asteroids.size());

#pragma endregion


        EnemyManager::GetInstance().update(playerEntity.getPosition(), gameTimer); //! <<***********************



#pragma region UpdateEntitiesCollision

        std::vector<std::string> enemiesToRemove;
        std::vector<std::string> asteroidsToRemove;
    
        for (auto& [enemyId, enemy] : EnemyManager::GetInstance().getEnemies()) {   
            
            // player to enemy collision
            if (CheckCollisions(enemy->getPosition(), playerEntity.getPosition(), playerEntity.getHitbox().width)
                && playerEntity.getPlayerState() == PLAYER_DEFAULT) {
                playerEntity.takeDamage(GetTime(), enemy->getPosition(), enemy->getHitbox().width);
                if (playerEntity.getHealth() <= 0) {
                    deathInfo.killerType = KILLER_ENEMY_COLLISION;
                    deathInfo.killerID = enemyId;
                    if (enemy->getEnemyType() == LOW_LEVEL_ENEMY) {
                        deathInfo.killerName = "Low Level Enemy";
                        // Create a temporary animation for the game over screen
                        Animator::GetInstance().AddAnimation("killer_display", std::make_shared<Animation>(
                            ResourceManager::GetInstance().GetTextureRef("low_level_enemy1"),
                            ResourceManager::GetInstance().GetTexture("low_level_enemy1").width / 10,
                            ResourceManager::GetInstance().GetTexture("low_level_enemy1").height,
                            0.1f,
                            true,
                            Rectangle{0, 0, 256, 256}, // Larger rectangle for scaling
                            false,
                            WHITE
                        ));
                        Animator::GetInstance().Play("killer_display");
                    } else if (enemy->getEnemyType() == MID_LEVEL_ENEMY) {
                        deathInfo.killerName = "Mid Level Enemy";
                        // Create a temporary animation for the game over screen
                        Animator::GetInstance().AddAnimation("killer_display", std::make_shared<Animation>(
                            ResourceManager::GetInstance().GetTextureRef("mid_level_enemy"),
                            ResourceManager::GetInstance().GetTexture("mid_level_enemy").width / 12,
                            ResourceManager::GetInstance().GetTexture("mid_level_enemy").height,
                            0.1f,
                            true,
                            Rectangle{0, 0, 256, 256}, // Larger rectangle for scaling
                            false,
                            WHITE
                        ));
                        Animator::GetInstance().Play("killer_display");
                    } else {
                        deathInfo.killerName = "Enemy";
                        deathInfo.killerTexture = ResourceManager::GetInstance().GetTexture("low_level_enemy1");
                    }
                    gameState = GAME_OVER;
                    ExplosionConfettiManager::GetInstance().startExplosion(playerEntity.getPosition());
                    GameOverOverlay::GetInstance().startOverlay();
                    PlaySound(ResourceManager::GetInstance().GetSound("game_over"));
                    StopMusicStream(gameMusic);
                }
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
                if (playerEntity.getHealth() <= 0) {
                    deathInfo.killerType = KILLER_ASTEROID;
                    deathInfo.killerID = asteroidId;
                    deathInfo.killerName = "Asteroid";
                    deathInfo.killerTexture = ResourceManager::GetInstance().GetTexture("asteroid");
                    gameState = GAME_OVER;
                    ExplosionConfettiManager::GetInstance().startExplosion(playerEntity.getPosition());
                    GameOverOverlay::GetInstance().startOverlay();
                    PlaySound(ResourceManager::GetInstance().GetSound("game_over"));
                    StopMusicStream(gameMusic);
                }
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
        break;
    }
    
    case GAME_AUGMENT_SELECTION:
    {
        // Update confetti effect
        ConfettiManager::GetInstance().update();
        
        for (auto& card : augmentCards) {
            card.update();
            if (card.isIntroComplete() && card.isClicked()) {
                PlaySound(ResourceManager::GetInstance().GetSound("button_click"));
                if (card.getTitle() == "Rapid Fire") {
                    playerEntity.increasePlayerAttackSpeed();
                } else if (card.getTitle() == "Trigger Happy") {
                    playerEntity.increaseTriggerHappyBurst();
                } else if (card.getTitle() == "Scatter Shot") {
                    playerEntity.increaseScatterShot();
                } else if (card.getTitle() == "Speed Demon") {
                    playerEntity.increasePlayerMovementSpeed();
                } else if (card.getTitle() == "Nano Repair") {
                    playerEntity.increasePlayerHealth();
                } else if (card.getTitle() == "Piercing Rounds") { 
                    playerEntity.increasePiercingRounds();
                } else if (card.getTitle() == "Phase Shift") {
                    playerEntity.increasePlayerIframeDuration();
                } else if (card.getTitle() == "Gyro Control") {
                    playerEntity.increasePlayerRotationSpeed();
                } else if (card.getTitle() == "Inertia Impact") {
                    playerEntity.increaseInertiaImpact();
                }
                // } else if (card.getTitle() == "Overdrive") {
                    
                // } else if (card.getTitle() == "Damage Amplifier") {
                   
                // }                
                card.startExitAnimation();
            }
            if (card.isExitComplete()) {
                augmentCards.clear();
                ConfettiManager::GetInstance().stopConfetti();
                gameTimer.unpause();
                gameState = GAME_RUNNING; 
            }
        }
        break; 
    }
    
    case GAME_PAUSED:
    {
        // cant draw here as it is an update function (a prob to solved latr)
        // const Texture2D& pauseTexture = ResourceManager::GetInstance().GetTextureRef("window");
        break;
    }
    
    case GAME_OVER:
    {
        //TODO: STOP THE GAME OVER SOUND AFTER CLICKING THE BUTTON
        gameTimer.pause();
        ExplosionConfettiManager::GetInstance().update();
        GameOverOverlay::GetInstance().update();
        
        if (GameOverOverlay::GetInstance().shouldShowText() && !gameOverTextStarted) {
            Animator::GetInstance().Play("game_over_text");
            gameOverTextStarted = true;
        }
        
        if (restartButton && restartButton->isClicked()) {
            this->restartGame();
            StopSound(ResourceManager::GetInstance().GetSound("game_over"));
            PlaySound(ResourceManager::GetInstance().GetSound("button_click"));
        }
        
        if (mainMenuButton && mainMenuButton->isClicked()) {
            StopSound(ResourceManager::GetInstance().GetSound("game_over"));
            PlaySound(ResourceManager::GetInstance().GetSound("button_click"));
            return "MainMenu";
        }
        
        Animator::GetInstance().Update();
        break;
    }
    
    default:
        TraceLog(LOG_ERROR, "Unknown game state: %d", gameState);
    }

    // std:: cout << "Player Health: " << playerEntity.getHealth() << std::endl;

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
        DamageIndicatorManager::GetInstance().draw();

#pragma region ControlsGuide

        if (gameTimer.getElapsedTime() > 0.4f && gameTimer.getElapsedTime() < 10.0f) {
            Vector2 wasdScreenPos = {1000, 1800}; // Top-left corner
            Vector2 eKeyScreenPos = {2000, 1850}; // Below WASD keys
            
            Vector2 wasdWorldPos = GetScreenToWorld2D(wasdScreenPos, utils::gameCamera);
            Vector2 eKeyWorldPos = GetScreenToWorld2D(eKeyScreenPos, utils::gameCamera);
            
            float elapsed = gameTimer.getElapsedTime();
            float alpha = 1.0f;
            
            //? fade in from 0.4s to 1.4s (1 second fade in)
            if (elapsed < 1.4f) {
                alpha = (elapsed - 0.4f) / 1.0f; // 0.0 to 1.0
                alpha = std::max(0.0f, std::min(1.0f, alpha));
            }
            //? fade out from 9.0s to 10.0s (1 second fade out)
            else if (elapsed > 9.0f) {
                alpha = (10.0f - elapsed) / 1.0f; // 1.0 to 0.0
                alpha = std::max(0.0f, std::min(1.0f, alpha));
            }
            //? apply alpha to animations
            Color animationTint = {255, 255, 255, (unsigned char)(alpha * 255)};
            Animator::GetInstance().SetTint("wasd_keys_tutorial", animationTint);
            Animator::GetInstance().SetTint("e_key_tutorial", animationTint);
            
            Animator::GetInstance().SetPosition("wasd_keys_tutorial", wasdWorldPos);
            Animator::GetInstance().SetPosition("e_key_tutorial", eKeyWorldPos);

            Font font = ResourceManager::GetInstance().GetFont("primary_font");
            
            Color textTint = {128, 128, 128, (unsigned char)(alpha * 255)}; 
            
            // WASD text
            Vector2 wasdTextScreenPos = {wasdScreenPos.x + 380, wasdScreenPos.y + 50}; 
            Vector2 wasdTextWorldPos = GetScreenToWorld2D(wasdTextScreenPos, utils::gameCamera);
            DrawTextEx(font, "MOVEMENT", wasdTextWorldPos, 5, 0, textTint);
            
            // E key text
            Vector2 eKeyTextScreenPos = {eKeyScreenPos.x + 160, eKeyScreenPos.y}; 
            Vector2 eKeyTextWorldPos = GetScreenToWorld2D(eKeyTextScreenPos, utils::gameCamera);
            DrawTextEx(font, "AUTO-SHOOT", eKeyTextWorldPos, 5, 0, textTint);
        }
        else if (gameTimer.getElapsedTime() > 10.0f) {
            Animator::GetInstance().Stop("wasd_keys_tutorial");
            Animator::GetInstance().Stop("e_key_tutorial");
        }

#pragma endregion ControlsGuide

        Animator::GetInstance().Update();
        Animator::GetInstance().Draw(); 

        // std::cout << "Player Position: (" << playerEntity.getPosition().x << ", " << playerEntity.getPosition().y << ")" << std::endl;
        if(gameState == GAME_OVER) ExplosionConfettiManager::GetInstance().draw();

    EndMode2D();

    if(gameState == GAME_AUGMENT_SELECTION) ConfettiManager::GetInstance().draw();

    if (!augmentCards.empty()) {
        for (auto& card : augmentCards) {
            card.draw();
        }
    }
    
    if(gameState == GAME_OVER) {
        GameOverOverlay::GetInstance().draw();
        
        if (GameOverOverlay::GetInstance().shouldShowText()) {
            float textY = GameOverOverlay::GetInstance().getTextYPosition() + 100;
            float textX = (GAME_SCREEN_WIDTH - 1800.0f) / 2.0f; 
            
            Animator::GetInstance().SetPosition("game_over_text", {textX, textY});
            Animator::GetInstance().Draw();
            
            Font font = ResourceManager::GetInstance().GetFont("primary_font");
            float detailsStartY = textY + 500; 
            float leftColumnX = GAME_SCREEN_WIDTH / 2.0f - 800;
            float rightColumnX = GAME_SCREEN_WIDTH / 2.0f + 150;
            float lineHeight = 80;
            
            DrawTextEx(font, TextFormat("Destroyed: %d", killCounter.getCount()), 
                      Vector2{leftColumnX, detailsStartY}, 80, 0, WHITE);
            
            DrawTextEx(font, TextFormat("Total Score: %d", playerEntity.getScore()), 
                      Vector2{leftColumnX, detailsStartY + lineHeight}, 80, 0, WHITE);
            
            DrawTextEx(font, TextFormat("Time Survived: %s", gameTimer.getElapsedTimeString().c_str()), 
                      Vector2{leftColumnX, detailsStartY + lineHeight * 2}, 80, 0, WHITE);
            
            DrawTextEx(font, TextFormat("Damage Dealt: %d", playerEntity.getTotalDamageDealt()), 
                      Vector2{leftColumnX, detailsStartY + lineHeight * 3}, 80, 0, WHITE);
            
            DrawTextEx(font, "Killed by:", 
                      Vector2{rightColumnX, detailsStartY}, 80, 0, WHITE);
            
            DrawTextEx(font, deathInfo.killerName.c_str(), 
                      Vector2{rightColumnX, detailsStartY + lineHeight}, 80, 0, RED);
            
            float killerImageY = detailsStartY + lineHeight * 2;
            Vector2 killerPos = {rightColumnX, killerImageY}; 
            
            if (deathInfo.killerType == KILLER_ENEMY_COLLISION) {
                Animator::GetInstance().SetPosition("killer_display", killerPos);
                Animator::GetInstance().Draw();
            } else {
                Rectangle killerRect = {rightColumnX, killerImageY, 256, 256};
                DrawTexturePro(deathInfo.killerTexture,
                              Rectangle{0, 0, (float)deathInfo.killerTexture.width, (float)deathInfo.killerTexture.height},
                              killerRect,
                              Vector2{0, 0},
                              0.0f,
                              WHITE);
            }
            
            if (restartButton) restartButton->draw();
            if (mainMenuButton) mainMenuButton->draw();
        }
    } 
    else {
#pragma region HealtBar
 
        const Texture2D& healthbarTexture = ResourceManager::GetInstance().GetTextureRef("health_bar");
        DrawTexturePro(healthbarTexture,
                    Rectangle{0, 0, (float)healthbarTexture.width, (float)healthbarTexture.height},
                    Rectangle{GAME_SCREEN_WIDTH - 1200, 150, 1000, 150},
                    Vector2{0, 0},
                    0.0f,
                    WHITE);

        const Texture2D& healthTexture = ResourceManager::GetInstance().GetTextureRef("health");
        float healthBarWidth = 1000.0f * (static_cast<float>(playerEntity.getHealth()) / 100.0f);
        DrawTexturePro(healthTexture,
                    Rectangle{0, 0, (float)healthTexture.width, (float)healthTexture.height},
                    Rectangle{GAME_SCREEN_WIDTH - 1200, 150, healthBarWidth, 150},
                    Vector2{0, 0},
                    0.0f,
                    WHITE);

#pragma endregion HealtBar

        killCounter.draw();
        gameTimer.draw(GAME_SCREEN_WIDTH);
        
        DamageOverlay::GetInstance().draw();
    }
}

void Game::onExit()
{
    Animator::GetInstance().StopAll();
    Animator::GetInstance().Remove("killer_display"); // Clean up killer animation
    StopMusicStream(gameMusic);
}
