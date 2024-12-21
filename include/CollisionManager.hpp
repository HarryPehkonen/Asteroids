// include/CollisionManager.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include "Ship.hpp"
#include "Asteroid.hpp"

class CollisionManager {
public:
    static bool checkCollision(const Ship& ship, const Asteroid& asteroid);
    
private:
    static float getDistance(const sf::Vector2f& p1, const sf::Vector2f& p2);
};
