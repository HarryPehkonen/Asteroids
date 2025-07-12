// include/GameState.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>
#include <vector>
#include "Ship.hpp"
#include "Asteroid.hpp"
#include "GameObjectManager.hpp"
#include "CollisionManager.hpp"
#include "Constants.hpp"

class GameState {
public:
    GameState();
    
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void reset();
    
    bool isGameOver() const { return !ship.has_value(); }
    bool isGameWon() const { return !isGameOver() && asteroidManager.count() == 0; }
    int getScore() const { return score; }
    size_t getAsteroidCount() const { return asteroidManager.count(); }
    
    const std::vector<std::unique_ptr<Asteroid>>& getAsteroids() const { 
        return asteroidManager.getObjects(); 
    }

    const Ship* getShip() const { return ship ? &*ship : nullptr; }
    Ship* getShip() { return ship ? &*ship : nullptr; }

private:
    std::optional<Ship> ship;
    GameObjectManager<Asteroid> asteroidManager;
    sf::Font font;
    int score{0};
    
    void createInitialAsteroids();
    void checkCollisions();
    void spawnSmallerAsteroids(const Asteroid& original);
    void handleGameOver();
    void handleWin();
    int getAsteroidPoints(Asteroid::Size size) const;
};
