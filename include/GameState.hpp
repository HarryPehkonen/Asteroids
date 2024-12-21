// include/GameState.hpp
#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Ship.hpp"
#include "Asteroid.hpp"
#include "CollisionManager.hpp"
#include "Constants.hpp"

class GameState {
public:
    GameState() {
        if (!loadResources()) {
            throw std::runtime_error("Failed to load game resources!");
        }
        reset();
    }

    void reset() {
        // Create ship in the center of the screen
        ship = std::make_unique<Ship>();
        ship->setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));

        // Clear and recreate asteroids
        asteroids.clear();
        createInitialAsteroids();

        // Reset score
        score = 0;
    }

    void update(float deltaTime) {
        updateGameLogic(deltaTime);
        handleCollisions();
    }

    void draw(sf::RenderWindow& window) {
        // Draw all asteroids
        for (auto& asteroid : asteroids) {
            asteroid->draw(window);
        }

        // Draw UI elements
        drawUI(window);

        // Draw ship if alive
        if (ship->isAlive()) {
            ship->draw(window);
        }
    }

    bool isGameOver() const {
        return !ship->isAlive();
    }

private:
    std::unique_ptr<Ship> ship;
    std::vector<std::unique_ptr<Asteroid>> asteroids;
    sf::Font font;
    int score = 0;

    bool loadResources() {
        return font.loadFromFile(FONT_PATH);
    }

    void updateGameLogic(float deltaTime) {
        if (isGameOver()) {
            handleGameOver();
            return;
        }

        ship->update(deltaTime);
        
        for (auto& asteroid : asteroids) {
            asteroid->update(deltaTime);
        }
    }

    void handleCollisions() {
        if (!ship->isAlive()) return;

        for (auto& asteroid : asteroids) {
            if (CollisionManager::checkCollision(*ship, *asteroid)) {
                ship->destroy();
                break;
            }
        }
    }

    void handleGameOver() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            reset();
        }
    }

    void drawUI(sf::RenderWindow& window) {
        // Draw score
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setPosition(10.f, 10.f);
        window.draw(scoreText);

        // Draw game over text if applicable
        if (isGameOver()) {
            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setCharacterSize(32);
            gameOverText.setFillColor(sf::Color::White);
            gameOverText.setString("Game Over!\nPress R to restart");
            
            // Center the text
            sf::FloatRect textBounds = gameOverText.getLocalBounds();
            gameOverText.setOrigin(
                textBounds.width / 2.f,
                textBounds.height / 2.f
            );
            gameOverText.setPosition(
                WINDOW_WIDTH / 2.f,
                WINDOW_HEIGHT / 2.f
            );
            
            window.draw(gameOverText);
        }
    }

    void createInitialAsteroids() {
        // Create initial asteroids positioned around the edges
        for (int i = 0; i < INITIAL_ASTEROID_COUNT; ++i) {
            auto asteroid = std::make_unique<Asteroid>(Asteroid::Size::Large);
            
            // Calculate position on a circle around the center
            float angle = (i * 2.0f * M_PI) / INITIAL_ASTEROID_COUNT;
            float distance = std::min(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.4f;
            float x = (WINDOW_WIDTH / 2.0f) + std::cos(angle) * distance;
            float y = (WINDOW_HEIGHT / 2.0f) + std::sin(angle) * distance;
            
            asteroid->setPosition(sf::Vector2f(x, y));
            
            // Give asteroid a random velocity
            float speedFactor = 50.0f;  // Adjust this to change asteroid speed
            float velocityAngle = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;
            sf::Vector2f velocity(
                std::cos(velocityAngle) * speedFactor,
                std::sin(velocityAngle) * speedFactor
            );
            asteroid->setVelocity(velocity);
            
            asteroids.push_back(std::move(asteroid));
        }
    }
};
