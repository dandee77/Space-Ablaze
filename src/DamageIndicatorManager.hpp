#pragma once
#include "DamageIndicator.hpp"
#include <vector>
#include <memory>

class DamageIndicatorManager
{
public:
    static DamageIndicatorManager& GetInstance() {
        static DamageIndicatorManager instance;
        return instance;
    }
    
    void addDamageIndicator(Vector2 position, int damage, int maxDamage, const std::string& enemyID);
    void update();
    void draw();
    void reset();

private:
    DamageIndicatorManager() = default;
    std::vector<std::unique_ptr<DamageIndicator>> indicators;
};
