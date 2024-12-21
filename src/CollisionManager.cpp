// src/CollisionManager.cpp
#include "CollisionManager.hpp"
#include <cmath>

bool CollisionManager::checkCollision(const Ship& ship, const Asteroid& asteroid) {
    // Simple circle collision detection
    float distance = getDistance(ship.getPosition(), asteroid.getPosition());
    return distance < (ship.getRadius() + asteroid.getRadius());
}

float CollisionManager::getDistance(const sf::Vector2f& p1, const sf::Vector2f& p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}
