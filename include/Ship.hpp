// include/Ship.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "GameObject.hpp"
#include "Vector2D.hpp"
#include "Constants.hpp"

class Ship : public GameObject {
public:
    Ship() {
        // Create the ship shape
        shape.setPointCount(3);
        shape.setPoint(0, sf::Vector2f(0, -20));     // Top point
        shape.setPoint(1, sf::Vector2f(-15, 20));    // Bottom left
        shape.setPoint(2, sf::Vector2f(15, 20));     // Bottom right
        
        // Set initial properties
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1.0f);
        
        // Center the origin
        shape.setOrigin(0, 0);
        
        // Initialize movement properties
        velocity = sf::Vector2f(0.0f, 0.0f);
        rotation = 0.0f;
        thrusting = false;
    }
    
    void update(float deltaTime) override {
        // Handle rotation
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
            rotation -= SHIP_ROTATION_SPEED * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
            rotation += SHIP_ROTATION_SPEED * deltaTime;
        }
        
        // Handle thrust
        thrusting = sf::Keyboard::isKeyPressed(sf::Keyboard::K);
        if (thrusting) {
            // Convert rotation to radians for calculating direction
            float rotationRad = rotation * M_PI / 180.0f;
            
            // Calculate thrust direction based on current rotation
            sf::Vector2f thrustDir(
                std::sin(rotationRad),
                -std::cos(rotationRad)  // Negative because y-axis is down in SFML
            );
            
            // Apply acceleration in the thrust direction
            velocity += thrustDir * SHIP_ACCELERATION * deltaTime;
            
            // Limit speed
            float currentSpeed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
            if (currentSpeed > SHIP_MAX_SPEED) {
                velocity *= SHIP_MAX_SPEED / currentSpeed;
            }
        }
        
        // Apply drag
        velocity *= DRAG_COEFFICIENT;
        
        // Update position
        position += velocity * deltaTime;
        
        // Wrap around screen edges
        wrapPosition();
        
        // Update shape
        shape.setPosition(position);
        shape.setRotation(rotation);
    }
    
    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
        
        // Draw thrust flame when thrusting
        if (thrusting) {
            drawThrustFlame(window);
        }
    }
    
private:
    void wrapPosition() {
        if (position.x < 0) position.x = WINDOW_WIDTH;
        if (position.x > WINDOW_WIDTH) position.x = 0;
        if (position.y < 0) position.y = WINDOW_HEIGHT;
        if (position.y > WINDOW_HEIGHT) position.y = 0;
    }
    
    void drawThrustFlame(sf::RenderWindow& window) {
        sf::ConvexShape flame;
        flame.setPointCount(3);
        
        // Calculate flame points relative to ship's back
        flame.setPoint(0, sf::Vector2f(-8, 22));    // Left point
        flame.setPoint(1, sf::Vector2f(8, 22));     // Right point
        flame.setPoint(2, sf::Vector2f(0, 35));     // Bottom point
        
        flame.setFillColor(sf::Color::Yellow);
        flame.setOrigin(0, 0);
        flame.setPosition(position);
        flame.setRotation(rotation);
        
        window.draw(flame);
    }
    
    sf::ConvexShape shape;
    sf::Vector2f velocity;
    float rotation;
    bool thrusting;
};
