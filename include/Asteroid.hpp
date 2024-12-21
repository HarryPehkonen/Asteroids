// include/Asteroid.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>
#include "GameObject.hpp"
#include "Constants.hpp"

class Asteroid : public GameObject {
public:
    enum class Size { Small, Medium, Large };
    
    Asteroid(Size size) : size(size) {
        initializeShape();
        initializeVelocity();
        rotationSpeed = generateRandomRotation();
    }
    
    void update(float deltaTime) override {
        // Update position based on velocity
        position += velocity * deltaTime;
        
        // Rotate the asteroid
        currentRotation += rotationSpeed * deltaTime;
        shape.setRotation(currentRotation);
        
        // Wrap around screen edges
        wrapPosition();
        
        // Update shape position
        shape.setPosition(position);
    }
    
    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
    }
    
    float getRadius() const {
        switch (size) {
            case Size::Large: return LARGE_ASTEROID_RADIUS;
            case Size::Medium: return MEDIUM_ASTEROID_RADIUS;
            case Size::Small: return SMALL_ASTEROID_RADIUS;
        }
        return 0.0f;  // Should never reach here
    }

    void setVelocity(const sf::Vector2f& vel) { velocity = vel; }
    const sf::Vector2f& getVelocity() const { return velocity; }
    Size getSize() const { return size; }

private:
    void initializeShape() {
        // Get radius based on size
        float radius = getRadius();
        
        // Generate random number of vertices (8-12 for large, 6-9 for medium, 4-6 for small)
        int vertices = (size == Size::Large) ? 8 + (rand() % 5) :
                      (size == Size::Medium) ? 6 + (rand() % 4) :
                                             4 + (rand() % 3);
        
        shape.setPointCount(vertices);
        
        // Generate points around a circle with random variation
        for (int i = 0; i < vertices; ++i) {
            float angle = (i * 2 * M_PI) / vertices;
            float randRadius = radius * (0.8f + (static_cast<float>(rand()) / RAND_MAX) * 0.4f);
            float x = std::cos(angle) * randRadius;
            float y = std::sin(angle) * randRadius;
            shape.setPoint(i, sf::Vector2f(x, y));
        }
        
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.0f);
        
        // Center the origin
        shape.setOrigin(0, 0);
    }
    
    void initializeVelocity() {
        // Base speed depends on asteroid size
        float baseSpeed = (size == Size::Large) ? 50.0f :
                         (size == Size::Medium) ? 75.0f :
                                                100.0f;
        
        // Add some random variation to speed
        float speedVariation = 0.5f;  // 50% variation
        float finalSpeed = baseSpeed * (1.0f + (static_cast<float>(rand()) / RAND_MAX - 0.5f) * speedVariation);
        
        // Generate random angle
        float angle = (static_cast<float>(rand()) / RAND_MAX) * 2.0f * M_PI;
        
        // Set velocity based on angle and speed
        velocity = sf::Vector2f(
            std::cos(angle) * finalSpeed,
            std::sin(angle) * finalSpeed
        );
    }
    
    float generateRandomRotation() {
        // Rotation speed also depends on size (smaller asteroids rotate faster)
        float baseRotation = (size == Size::Large) ? 30.0f :
                            (size == Size::Medium) ? 45.0f :
                                                   60.0f;
        
        // Add random variation and randomly choose direction
        float variation = 0.5f;  // 50% variation
        float finalRotation = baseRotation * (1.0f + (static_cast<float>(rand()) / RAND_MAX - 0.5f) * variation);
        
        // Randomly choose rotation direction
        return (rand() % 2 == 0) ? finalRotation : -finalRotation;
    }
    
    void wrapPosition() {
        if (position.x < 0) position.x = WINDOW_WIDTH;
        if (position.x > WINDOW_WIDTH) position.x = 0;
        if (position.y < 0) position.y = WINDOW_HEIGHT;
        if (position.y > WINDOW_HEIGHT) position.y = 0;
    }
    
    Size size;
    sf::ConvexShape shape;
    sf::Vector2f velocity;
    float rotationSpeed;        // Degrees per second
    float currentRotation = 0;  // Current rotation in degrees
};
