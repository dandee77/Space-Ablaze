#include "SpatialGrid.hpp"
#include "raymath.h"
#include <cmath>
#include <algorithm>

void SpatialGrid::clear() {
    grid.clear();
    entities.clear();
}

void SpatialGrid::addEntity(const std::string& entityId, Vector2 position) {
    GridKey key = getGridKey(position);
    
    // Remove entity from previous cell if it exists
    auto it = entities.find(entityId);
    if (it != entities.end()) {
        GridKey oldKey = it->second.gridKey;
        auto& oldCell = grid[oldKey];
        oldCell.erase(std::remove(oldCell.begin(), oldCell.end(), entityId), oldCell.end());
        if (oldCell.empty()) {
            grid.erase(oldKey);
        }
    }
    
    // Add entity to new cell
    grid[key].push_back(entityId);
    entities[entityId] = {position, key};
}

std::vector<Vector2> SpatialGrid::getNearbyPositions(Vector2 position, float radius, const std::string& excludeId) {
    std::vector<Vector2> nearbyPositions;
    std::vector<GridKey> intersectingCells = getIntersectingCells(position, radius);
    
    for (const GridKey& cellKey : intersectingCells) {
        auto cellIt = grid.find(cellKey);
        if (cellIt == grid.end()) continue;
        
        for (const std::string& entityId : cellIt->second) {
            if (entityId == excludeId) continue;
            
            auto entityIt = entities.find(entityId);
            if (entityIt == entities.end()) continue;
            
            Vector2 entityPos = entityIt->second.position;
            float distance = Vector2Distance(position, entityPos);
            
            if (distance <= radius) {
                nearbyPositions.push_back(entityPos);
            }
        }
    }
    
    return nearbyPositions;
}

std::vector<Vector2> SpatialGrid::getAllPositions() const {
    std::vector<Vector2> positions;
    positions.reserve(entities.size());
    
    for (const auto& [entityId, data] : entities) {
        positions.push_back(data.position);
    }
    
    return positions;
}

SpatialGrid::GridKey SpatialGrid::getGridKey(Vector2 position) const {
    return {
        static_cast<int>(std::floor(position.x / cellSize)),
        static_cast<int>(std::floor(position.y / cellSize))
    };
}

std::vector<SpatialGrid::GridKey> SpatialGrid::getIntersectingCells(Vector2 center, float radius) const {
    std::vector<GridKey> cells;
    
    // Calculate the bounding box of the circle in grid coordinates
    int minX = static_cast<int>(std::floor((center.x - radius) / cellSize));
    int maxX = static_cast<int>(std::floor((center.x + radius) / cellSize));
    int minY = static_cast<int>(std::floor((center.y - radius) / cellSize));
    int maxY = static_cast<int>(std::floor((center.y + radius) / cellSize));
    
    // Check each cell in the bounding box
    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            cells.push_back({x, y});
        }
    }
    
    return cells;
}
