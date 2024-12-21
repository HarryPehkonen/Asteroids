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
    }
    
    void update(float deltaTime) override {
        // Currently nothing to update
        (void)deltaTime; // Silence unused parameter warning
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

private:
    void initializeShape() {
        // Get radius based on size
        float radius = (size == Size::Large) ? LARGE_ASTEROID_RADIUS :
                      (size == Size::Medium) ? MEDIUM_ASTEROID_RADIUS :
                                             SMALL_ASTEROID_RADIUS;
        
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
    
    Size size;
    sf::ConvexShape shape;
    sf::Vector2f velocity;
};
