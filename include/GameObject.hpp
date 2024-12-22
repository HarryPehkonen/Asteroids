// include/GameObject.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include "DebugUtils.hpp"

class GameObject {
public:
    // Virtual destructor for proper cleanup of derived classes
    virtual ~GameObject() = default;
    
    // Core functionality every game object must implement
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    
    // Position management
    void setPosition(const sf::Vector2f& pos) { position = pos; }
    const sf::Vector2f& getPosition() const { return position; }
    
    // Velocity management - common to most game objects
    void setVelocity(const sf::Vector2f& vel) { velocity = vel; }
    const sf::Vector2f& getVelocity() const { return velocity; }

protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
    
    // Utility function for wrapping objects around screen edges
    void wrapPosition(float screenWidth, float screenHeight) {
        if (position.x < 0) position.x = screenWidth;
        if (position.x > screenWidth) position.x = 0;
        if (position.y < 0) position.y = screenHeight;
        if (position.y > screenHeight) position.y = 0;
    }
};
