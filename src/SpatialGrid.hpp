#pragma once

#include "raylib.h"
#include <unordered_map>
#include <vector>
#include <string>

class SpatialGrid {
public:
    SpatialGrid(float cellSize = 50.0f) : cellSize(cellSize) {}

    void clear();
    void addEntity(const std::string& entityId, Vector2 position);
    std::vector<Vector2> getNearbyPositions(Vector2 position, float radius, const std::string& excludeId = "");
    std::vector<Vector2> getAllPositions() const;

private:
    struct GridKey {
        int x, y;
        
        bool operator==(const GridKey& other) const {
            return x == other.x && y == other.y;
        }
    };
    
    struct GridKeyHash {
        std::size_t operator()(const GridKey& key) const {
            return std::hash<int>()(key.x) ^ (std::hash<int>()(key.y) << 1);
        }
    };
    
    struct EntityData {
        Vector2 position;
        GridKey gridKey;
    };
    
    float cellSize;
    std::unordered_map<GridKey, std::vector<std::string>, GridKeyHash> grid;
    std::unordered_map<std::string, EntityData> entities;
    
    GridKey getGridKey(Vector2 position) const;
    
    std::vector<GridKey> getIntersectingCells(Vector2 center, float radius) const;
};
