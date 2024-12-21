// include/GameState.hpp
#pragma once
#include <vector>
#include <memory>
#include "Ship.hpp"
#include "Asteroid.hpp"
#include "Constants.hpp"

class GameState {
public:
    GameState() {
        // Create ship in the center of the screen
        ship = std::make_unique<Ship>();
        ship->setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));

        // Create initial asteroids
        createInitialAsteroids();
    }

    void update(float deltaTime) {
        ship->update(deltaTime);
        for (auto& asteroid : asteroids) {
            asteroid->update(deltaTime);
        }
    }

    void draw(sf::RenderWindow& window) {
        ship->draw(window);
        for (auto& asteroid : asteroids) {
            asteroid->draw(window);
        }
    }

private:
    void createInitialAsteroids() {
        // Create a few large asteroids around the edges
        for (int i = 0; i < INITIAL_ASTEROID_COUNT; ++i) {
            auto asteroid = std::make_unique<Asteroid>(Asteroid::Size::Large);
            
            // Position asteroids around the edges of the screen
            float angle = (i * 2.0f * M_PI) / INITIAL_ASTEROID_COUNT;
            float distance = 200.0f; // Distance from center
            float x = (WINDOW_WIDTH / 2.0f) + std::cos(angle) * distance;
            float y = (WINDOW_HEIGHT / 2.0f) + std::sin(angle) * distance;
            
            asteroid->setPosition(sf::Vector2f(x, y));
            asteroids.push_back(std::move(asteroid));
        }
    }

    std::unique_ptr<Ship> ship;
    std::vector<std::unique_ptr<Asteroid>> asteroids;
};
