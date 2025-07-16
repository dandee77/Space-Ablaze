#include "DamageIndicatorManager.hpp"
#include <algorithm>

void DamageIndicatorManager::addDamageIndicator(Vector2 position, int damage, int maxDamage)
{
    indicators.push_back(std::make_unique<DamageIndicator>(position, damage, maxDamage));
}

void DamageIndicatorManager::update()
{
    for (auto& indicator : indicators) {
        indicator->update();
    }
    
    indicators.erase(
        std::remove_if(indicators.begin(), indicators.end(),
            [](const std::unique_ptr<DamageIndicator>& indicator) {
                return indicator->isFinished();
            }),
        indicators.end()
    );
}

void DamageIndicatorManager::draw()
{
    for (auto& indicator : indicators) {
        indicator->draw();
    }
}

void DamageIndicatorManager::reset()
{
    indicators.clear();
}
