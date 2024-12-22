// include/Bullet.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.hpp"
#include "Constants.hpp"

class Bullet : public GameObject {
public:
    Bullet(const sf::Vector2f& startPos, float rotation) {
        position = startPos;
        
        // Convert rotation to radians for direction calculation
        float rotationRad = rotation * M_PI / 180.0f;
        
        // Calculate velocity based on rotation
        velocity = sf::Vector2f(
            std::sin(rotationRad) * BULLET_SPEED,
            -std::cos(rotationRad) * BULLET_SPEED  // Negative because y-axis is down in SFML
        );
        
        // Initialize the shape
        shape.setRadius(2.0f);
        shape.setFillColor(sf::Color::White);
        shape.setOrigin(2.0f, 2.0f);  // Center the origin
        shape.setPosition(position);
        
        // Calculate distance this bullet can travel
        float shortestAxis = std::min(WINDOW_WIDTH, WINDOW_HEIGHT);
        maxDistance = shortestAxis * 0.75f;  // Bullet travels 75% of shortest screen dimension
    }
    
    void update(float deltaTime) override {
        // Update position based on velocity
        position += velocity * deltaTime;
        shape.setPosition(position);
        
        // Update total distance traveled
        float movement = BULLET_SPEED * deltaTime;
        distanceTraveled += movement;
    }
    
    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
    }
    
    float getRadius() const { return 2.0f; }
    
    // Returns true if bullet has exceeded its maximum travel distance
    bool hasExpired() const {
        return distanceTraveled >= maxDistance;
    }
    
    // Returns true if bullet is off screen
    bool isOffScreen() const {
        return position.x < 0 || position.x > WINDOW_WIDTH ||
               position.y < 0 || position.y > WINDOW_HEIGHT;
    }

private:
    sf::CircleShape shape;
    float distanceTraveled = 0.0f;
    float maxDistance;
    
    static constexpr float BULLET_SPEED = 500.0f;  // pixels per second
};
