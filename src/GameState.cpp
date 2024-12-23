// src/GameState.cpp
#include "GameState.hpp"
#include <cmath>
#include <random>
#include "DebugUtils.hpp"
#include "Constants.hpp"

GameState::GameState() {
    if (!font.loadFromFile(FONT_PATH)) {
        // Just continue without font - handled in draw methods
    }
    reset();
}

void GameState::reset() {
    ship.emplace();  // Create new ship
    if (ship) {
        ship->setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));
    }
    
    asteroidManager.clear();
    createInitialAsteroids();  // This should create the initial asteroids
    score = 0;
}

void GameState::createInitialAsteroids() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> speedDist(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED);
    std::uniform_real_distribution<float> angleDist(0, 2 * M_PI);
    
    for (int i = 0; i < INITIAL_ASTEROID_COUNT; ++i) {
        // Create a new large asteroid
        auto asteroid = std::make_unique<Asteroid>(Asteroid::Size::Large);
        
        // Calculate spawn position around the edges
        float angle = (i * 2.0f * M_PI) / INITIAL_ASTEROID_COUNT;
        float distance = std::min(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.4f;
        sf::Vector2f spawnPos(
            (WINDOW_WIDTH / 2.0f) + std::cos(angle) * distance,
            (WINDOW_HEIGHT / 2.0f) + std::sin(angle) * distance
        );
        asteroid->setPosition(spawnPos);
        
        // Calculate velocity directed somewhat towards center
        float speedAngle = angleDist(gen);
        float speed = speedDist(gen);
        sf::Vector2f velocity(
            std::cos(speedAngle) * speed,
            std::sin(speedAngle) * speed
        );
        asteroid->setVelocity(velocity);
        
        // Add the asteroid to the manager
        size_t countBefore = asteroidManager.count();
        asteroidManager.spawn(std::move(asteroid));
        size_t countAfter = asteroidManager.count();
        LOG_VALUE("Asteroid count before", countBefore);
        LOG_VALUE("Asteroid count after", countAfter);
    }
}

void GameState::update(float deltaTime) {
    if (!ship) {
        handleGameOver();
        return;
    }

    ship->update(deltaTime);
    asteroidManager.update(deltaTime);
    checkCollisions();
}

void GameState::checkCollisions() {
    if (!ship) return;

    for (const auto& asteroid : asteroidManager.getObjects()) {
        if (!asteroid) continue;
        
        if (CollisionManager::checkCollision(*ship, *asteroid)) {
            ship.reset();
            return;
        }
    }
    
    auto& bullets = ship->getBulletManager().getObjects();
    for (const auto& bullet : bullets) {
        if (!bullet) continue;
        
        for (const auto& asteroid : asteroidManager.getObjects()) {
            if (!asteroid) continue;
            
            if (CollisionManager::checkCollision(*bullet, *asteroid)) {
                score += getAsteroidPoints(asteroid->getSize());
                spawnSmallerAsteroids(*asteroid);
                
                LOG("Bullet manager...");
                ship->getBulletManager().removeIf(
                    [bullet_ptr = bullet.get()](const Bullet& b) { 
                        return &b == bullet_ptr; 
                    });

                LOG("Asteroid manager...");
                asteroidManager.removeIf(
                    [asteroid_ptr = asteroid.get()](const Asteroid& a) { 
                        return &a == asteroid_ptr; 
                    });
                
                break;
            }
        }
    }
}

void GameState::spawnSmallerAsteroids(const Asteroid& original) {
    if (original.getSize() == Asteroid::Size::Small) return;
    
    Asteroid::Size newSize = (original.getSize() == Asteroid::Size::Large) ? 
        Asteroid::Size::Medium : Asteroid::Size::Small;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> angleDist(-M_PI/3, M_PI/3);
    
    sf::Vector2f origVel = original.getVelocity();
    float speed = std::sqrt(origVel.x * origVel.x + origVel.y * origVel.y) * 1.5f;
    float baseAngle = std::atan2(origVel.y, origVel.x);
    
    for (int i = 0; i < 2; ++i) {
        auto newAsteroid = std::make_unique<Asteroid>(newSize);
        newAsteroid->setPosition(original.getPosition());
        
        float spreadAngle = (i == 0) ? angleDist(gen) : -angleDist(gen);
        float finalAngle = baseAngle + spreadAngle;
        
        newAsteroid->setVelocity(sf::Vector2f(
            std::cos(finalAngle) * speed,
            std::sin(finalAngle) * speed
        ));
        
        asteroidManager.spawn(std::move(newAsteroid));
    }
}

void GameState::draw(sf::RenderWindow& window) {
    if (ship) {
        ship->draw(window);
    }
    asteroidManager.draw(window);
    
    // Draw score if font loaded
    if (!font.getInfo().family.empty()) {
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setPosition(10.f, 10.f);
        window.draw(scoreText);
    }
    
    if (isGameOver() && !font.getInfo().family.empty()) {
        sf::Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setCharacterSize(32);
        gameOverText.setFillColor(sf::Color::White);
        gameOverText.setString("Game Over!\nPress R to restart");
        
        sf::FloatRect bounds = gameOverText.getLocalBounds();
        gameOverText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        gameOverText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
        
        window.draw(gameOverText);
    }
}

void GameState::handleGameOver() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        reset();
    }
}

int GameState::getAsteroidPoints(Asteroid::Size size) const {
    switch (size) {
        case Asteroid::Size::Large: return POINTS_LARGE_ASTEROID;
        case Asteroid::Size::Medium: return POINTS_MEDIUM_ASTEROID;
        case Asteroid::Size::Small: return POINTS_SMALL_ASTEROID;
        default: return 0;
    }
}
