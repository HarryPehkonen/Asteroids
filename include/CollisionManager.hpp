// include/CollisionManager.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include "Ship.hpp"
#include "Asteroid.hpp"
#include "Bullet.hpp"

class CollisionManager {
public:
    // Check collision between ship and asteroid
    static bool checkCollision(const Ship& ship, const Asteroid& asteroid) {
        return getDistance(ship.getPosition(), asteroid.getPosition()) < 
               (ship.getRadius() + asteroid.getRadius());
    }
    
    // Check collision between bullet and asteroid
    static bool checkCollision(const Bullet& bullet, const Asteroid& asteroid) {
        return getDistance(bullet.getPosition(), asteroid.getPosition()) < 
               (bullet.getRadius() + asteroid.getRadius());
    }

private:
    // Helper function to calculate distance between two points
    static float getDistance(const sf::Vector2f& p1, const sf::Vector2f& p2) {
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};
