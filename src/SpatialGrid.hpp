#pragma once

#include "raylib.h"
#include <unordered_map>
#include <vector>
#include <string>

// Spatial hash grid for fast neighbor queries
class SpatialGrid {
public:
    SpatialGrid(float cellSize = 50.0f) : cellSize(cellSize) {}
    
    // Clear all entities from the grid
    void clear();
    
    // Add an entity to the grid at the given position
    void addEntity(const std::string& entityId, Vector2 position);
    
    // Get all entities within a radius of the given position
    std::vector<Vector2> getNearbyPositions(Vector2 position, float radius, const std::string& excludeId = "");
    
    // Get all entity positions in the grid (for debugging)
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
            // Combine hash of x and y coordinates
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
    
    // Convert world position to grid coordinates
    GridKey getGridKey(Vector2 position) const;
    
    // Get all grid cells that intersect with a circle
    std::vector<GridKey> getIntersectingCells(Vector2 center, float radius) const;
};
