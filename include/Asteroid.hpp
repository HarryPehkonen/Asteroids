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
    
    explicit Asteroid(Size size) : size(size) {
        generateShape();
        generateRotationSpeed();
    }
    
    void update(float deltaTime) override {
        // Update position based on velocity
        position += velocity * deltaTime;
        
        // Update rotation
        currentRotation += rotationSpeed * deltaTime;
        shape.setRotation(currentRotation);
        
        // Wrap around screen edges
        wrapPosition(WINDOW_WIDTH, WINDOW_HEIGHT);
        
        // Update shape position
        shape.setPosition(position);
    }
    
    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
    }
    
    // Get the collision radius based on asteroid size
    float getRadius() const {
        switch (size) {
            case Size::Large: return LARGE_ASTEROID_RADIUS;
            case Size::Medium: return MEDIUM_ASTEROID_RADIUS;
            case Size::Small: return SMALL_ASTEROID_RADIUS;
        }
        return 0.0f;  // Should never reach here
    }

    Size getSize() const { return size; }

private:
    void generateShape() {
        // Get radius based on size
        float radius = getRadius();
        
        // Generate random number of vertices based on size
        int minVertices = (size == Size::Small) ? 6 : (size == Size::Medium) ? 8 : 10;
        int maxVertices = (size == Size::Small) ? 8 : (size == Size::Medium) ? 10 : 12;
        int vertices = minVertices + (rand() % (maxVertices - minVertices + 1));
        
        shape.setPointCount(vertices);
        
        // Generate random points around a circle
        for (int i = 0; i < vertices; ++i) {
            float angle = (i * 2 * M_PI) / vertices;
            
            // Add some randomness to the radius (between 80% and 120% of base radius)
            float radiusVariation = radius * (0.8f + (static_cast<float>(rand()) / RAND_MAX) * 0.4f);
            
            float x = std::cos(angle) * radiusVariation;
            float y = std::sin(angle) * radiusVariation;
            
            shape.setPoint(i, sf::Vector2f(x, y));
        }
        
        // Set appearance
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1.0f);
        shape.setOrigin(0, 0);
    }
    
    void generateRotationSpeed() {
        // Base rotation speed depends on size (smaller asteroids rotate faster)
        float baseSpeed = (size == Size::Large) ? 30.0f :
                         (size == Size::Medium) ? 45.0f :
                                                60.0f;
        
        // Add random variation (±50%)
        float variation = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * baseSpeed;
        rotationSpeed = baseSpeed + variation;
        
        // Randomly reverse rotation direction
        if (rand() % 2 == 0) {
            rotationSpeed = -rotationSpeed;
        }
    }
    
    const Size size;
    sf::ConvexShape shape;
    float rotationSpeed;        // Degrees per second
    float currentRotation = 0;  // Current rotation in degrees
};
