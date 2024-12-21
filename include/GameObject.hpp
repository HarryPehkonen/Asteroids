// include/GameObject.hpp
#pragma once
#include <SFML/Graphics.hpp>

class GameObject {
public:
    virtual ~GameObject() = default;
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    
    void setPosition(const sf::Vector2f& pos) { position = pos; }
    const sf::Vector2f& getPosition() const { return position; }

protected:
    sf::Vector2f position;
};
