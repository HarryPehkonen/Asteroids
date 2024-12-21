// include/Ship.hpp
#pragma once
#include <SFML/Graphics.hpp>
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
        shape.setOutlineThickness(2.0f);
        
        // Center the origin
        shape.setOrigin(0, 0);
    }
    
    void update(float deltaTime) override {
        // Currently nothing to update
        (void)deltaTime; // Silence unused parameter warning
        shape.setPosition(position);
    }
    
    void draw(sf::RenderWindow& window) override {
        window.draw(shape);
    }
    
private:
    sf::ConvexShape shape;
};
