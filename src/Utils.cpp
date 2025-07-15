#include "Utils.hpp"

namespace utils
{
    Camera2D gameCamera = {0};

    std::array<std::array<std::string, 2>, 8> augmentPool = {{
        {"Rapid Fire", "Increases attack\nspeed by 50%"},
        {"Trigger Happy", "Boosts fire rate\nfor faster shooting"},
        {"Scatter Shot", "Fires bullets in\nmultiple directions"},
        {"Agility Boost", "Increases player\nmovement speed"},
        {"Iron Core", "Increases max\nhealth capacity"},
        {"Piercing Rounds", "Bullets pierce through\nenemies"},
        {"Phase Shift", "Extends iframe duration\nafter damage"},
        {"Gyro Control", "Increases rotation\nspeed for faster turns"}
    }};
};