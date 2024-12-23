// include/Ship.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "GameObject.hpp"
#include "GameObjectManager.hpp"
#include "Bullet.hpp"
#include "Constants.hpp"

class Ship : public GameObject {
public:
    Ship() {

        float scale = SHIP_SCALE;

        // Create the ship shape
        shape.setPointCount(3);
        shape.setPoint(0, sf::Vector2f(0.0f * scale, -20.0f * scale));     // Top point
        shape.setPoint(1, sf::Vector2f(-15.0f * scale, 20.0f * scale));    // Bottom left
        shape.setPoint(2, sf::Vector2f(15.0f * scale, 20.0f * scale));     // Bottom right
        
        // Set initial properties
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1.0f);
        
        // Center the origin
        shape.setOrigin(0, 0);
        
        // Initialize movement properties
        rotation = 0.0f;
        thrusting = false;
    }
    
    void update(float deltaTime) override {
        handleRotation(deltaTime);
        handleThrust(deltaTime);
        handleShooting();
        
        // Update position based on velocity
        position += velocity * deltaTime;
        wrapPosition(WINDOW_WIDTH, WINDOW_HEIGHT);
        
        // Update shape position and rotation
        shape.setPosition(position);
        shape.setRotation(rotation);
        
        // Update bullets and remove expired ones
        bulletManager.update(deltaTime);
        
        // Remove expired or off-screen bullets
        bulletManager.removeIf([](const Bullet& bullet) {
            return bullet.hasExpired() || bullet.isOffScreen();
        });
    }
    
    void draw(sf::RenderWindow& window) override {
        // Draw the ship
        window.draw(shape);
        
        // Draw thrust flame when thrusting
        if (thrusting) {
            drawThrustFlame(window);
        }
        
        // Draw all bullets
        bulletManager.draw(window);
    }
    
    float getRadius() const { return 20.0f * SHIP_SCALE; }
    
    // Bullet manager access
    const GameObjectManager<Bullet>& getBulletManager() const { return bulletManager; }
    GameObjectManager<Bullet>& getBulletManager() { return bulletManager; }

private:
    void handleRotation(float deltaTime) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
            rotation -= SHIP_ROTATION_SPEED * deltaTime;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
            rotation += SHIP_ROTATION_SPEED * deltaTime;
        }
    }
    
    void handleThrust(float deltaTime) {
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
    }
    
    void handleShooting() {
        static bool wasSpacePressed = false;
        bool isSpacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        
        // Only shoot on initial key press, not hold
        if (isSpacePressed && !wasSpacePressed) {
            fireBullet();
        }
        
        wasSpacePressed = isSpacePressed;
    }
    
    void fireBullet() {
        if (bulletManager.count() >= MAX_BULLETS) return;
        
        // Calculate bullet start position (at ship's nose)
        float rotationRad = rotation * M_PI / 180.0f;
        sf::Vector2f bulletPos = position + sf::Vector2f(
            std::sin(rotationRad) * 20.0f,  // 20 pixels from center (ship height)
            -std::cos(rotationRad) * 20.0f
        );
        
        // Create and spawn new bullet
        auto bullet = std::make_unique<Bullet>(bulletPos, rotation);
        bulletManager.spawn(std::move(bullet));
    }
    
    void drawThrustFlame(sf::RenderWindow& window) {
        sf::ConvexShape flame;
        flame.setPointCount(3);
        float scale = SHIP_SCALE;
        
        // Calculate flame points relative to ship's back
        flame.setPoint(0, sf::Vector2f(-8.0f * scale, 22.0f * scale));    // Left point
        flame.setPoint(1, sf::Vector2f(8.0f * scale, 22.0f * scale));     // Right point
        flame.setPoint(2, sf::Vector2f(0.0f * scale, 35.0f * scale));     // Bottom point
        
        flame.setFillColor(sf::Color::Yellow);
        flame.setOrigin(0, 0);
        flame.setPosition(position);
        flame.setRotation(rotation);
        
        window.draw(flame);
    }
    
    sf::ConvexShape shape;
    float rotation;
    bool thrusting;
    
    // Bullet management using double buffer system
    GameObjectManager<Bullet> bulletManager;
};
