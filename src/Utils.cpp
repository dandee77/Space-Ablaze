#include "Utils.hpp"

namespace utils
{
    Camera2D gameCamera = {0};

    std::array<std::array<std::string, 2>, 8> augmentPool = {{
        {"Rapid Fire", "Increases attack\nspeed by 10%"},
        {"Trigger Happy", "Boosts fire rate\nfor faster shooting"},
        {"Scatter Shot", "Fires bullets in\nmultiple directions"},
        {"Speed Demon", "Increases player\nmovement speed"},
        {"Nano Repair", "Heals a portion\nof your HP instantly"},
        {"Piercing Rounds", "Bullets pierce through\nenemies"},
        {"Phase Shift", "Extends iframe duration\nafter damage"},
        {"Gyro Control", "Increases rotation\nspeed for faster turns"}
    }};
};