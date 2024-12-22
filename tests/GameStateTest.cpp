// tests/GameStateTest.cpp
#include <gtest/gtest.h>
#include "GameState.hpp"

class GameStateTest : public ::testing::Test {
protected:
    void SetUp() override {
        gameState = std::make_unique<GameState>();
    }
    
    std::unique_ptr<GameState> gameState;
};

TEST_F(GameStateTest, InitialState) {
    EXPECT_FALSE(gameState->isGameOver());
    EXPECT_EQ(gameState->getScore(), 0);
    EXPECT_EQ(gameState->getAsteroidCount(), INITIAL_ASTEROID_COUNT);
    EXPECT_NE(gameState->getShip(), nullptr);
}

TEST_F(GameStateTest, ValidateInitialAsteroids) {
    // Update game state to initialize asteroids
    gameState->update(0.0f);

    // This test explicitly checks asteroid initialization
    const auto& asteroids = gameState->getAsteroids();
    
    // Check asteroid count
    EXPECT_EQ(asteroids.size(), INITIAL_ASTEROID_COUNT) 
        << "Expected " << INITIAL_ASTEROID_COUNT << " asteroids, but found " 
        << asteroids.size();
    
    // Verify each asteroid
    for (size_t i = 0; i < asteroids.size(); ++i) {
        ASSERT_NE(asteroids[i], nullptr) 
            << "Asteroid at index " << i << " is null";
        
        // Verify asteroid properties
        EXPECT_EQ(asteroids[i]->getSize(), Asteroid::Size::Large)
            << "Asteroid " << i << " is not large size";
        
        // Verify position is within bounds
        const auto& pos = asteroids[i]->getPosition();
        EXPECT_GE(pos.x, 0.0f) << "Asteroid " << i << " x position < 0";
        EXPECT_LE(pos.x, WINDOW_WIDTH) 
            << "Asteroid " << i << " x position > window width";
        EXPECT_GE(pos.y, 0.0f) << "Asteroid " << i << " y position < 0";
        EXPECT_LE(pos.y, WINDOW_HEIGHT) 
            << "Asteroid " << i << " y position > window height";
        
        // Verify velocity is non-zero
        const auto& vel = asteroids[i]->getVelocity();
        EXPECT_NE(vel.x, 0.0f) << "Asteroid " << i << " has zero x velocity";
        EXPECT_NE(vel.y, 0.0f) << "Asteroid " << i << " has zero y velocity";
    }
}

TEST_F(GameStateTest, GameOver) {
    // Update game state to initialize asteroids
    gameState->update(0.0f);

    EXPECT_FALSE(gameState->isGameOver());
    
    // Get the ship and first asteroid
    Ship* ship = gameState->getShip();
    ASSERT_NE(ship, nullptr);
    
    const auto& asteroids = gameState->getAsteroids();
    ASSERT_FALSE(asteroids.empty());
    ASSERT_NE(asteroids[0], nullptr);
    
    // Force collision by moving asteroid to ship's position
    auto& firstAsteroid = asteroids[0];
    firstAsteroid->setPosition(ship->getPosition());
    
    // Update should detect collision
    gameState->update(0.016f);
    
    EXPECT_TRUE(gameState->isGameOver());
}

TEST_F(GameStateTest, Reset) {
    // Update game state to initialize asteroids
    gameState->update(0.0f);

    // First cause game over
    Ship* ship = gameState->getShip();
    ASSERT_NE(ship, nullptr);
    
    const auto& asteroids = gameState->getAsteroids();
    ASSERT_FALSE(asteroids.empty());
    ASSERT_NE(asteroids[0], nullptr);
    
    auto& firstAsteroid = asteroids[0];
    firstAsteroid->setPosition(ship->getPosition());
    
    gameState->update(0.016f);
    EXPECT_TRUE(gameState->isGameOver());
    
    // Now reset
    gameState->reset();
    
    // Verify reset state
    EXPECT_FALSE(gameState->isGameOver());
    EXPECT_EQ(gameState->getScore(), 0);
    EXPECT_EQ(gameState->getAsteroidCount(), INITIAL_ASTEROID_COUNT);
    EXPECT_NE(gameState->getShip(), nullptr);
}
