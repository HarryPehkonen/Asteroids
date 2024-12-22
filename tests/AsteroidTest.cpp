// tests/AsteroidTest.cpp
#include <gtest/gtest.h>
#include "Asteroid.hpp"

class AsteroidTest : public ::testing::Test {
protected:
    void SetUp() override {
        largeAsteroid = std::make_unique<Asteroid>(Asteroid::Size::Large);
        mediumAsteroid = std::make_unique<Asteroid>(Asteroid::Size::Medium);
        smallAsteroid = std::make_unique<Asteroid>(Asteroid::Size::Small);
    }
    
    std::unique_ptr<Asteroid> largeAsteroid;
    std::unique_ptr<Asteroid> mediumAsteroid;
    std::unique_ptr<Asteroid> smallAsteroid;
};

TEST_F(AsteroidTest, Sizes) {
    EXPECT_EQ(largeAsteroid->getRadius(), LARGE_ASTEROID_RADIUS);
    EXPECT_EQ(mediumAsteroid->getRadius(), MEDIUM_ASTEROID_RADIUS);
    EXPECT_EQ(smallAsteroid->getRadius(), SMALL_ASTEROID_RADIUS);
}

TEST_F(AsteroidTest, Movement) {
    sf::Vector2f initialPos(100, 100);
    sf::Vector2f velocity(50, 50);
    float deltaTime = 1.0f;
    
    largeAsteroid->setPosition(initialPos);
    largeAsteroid->setVelocity(velocity);
    
    largeAsteroid->update(deltaTime);
    
    sf::Vector2f expectedPos = initialPos + velocity * deltaTime;
    EXPECT_FLOAT_EQ(largeAsteroid->getPosition().x, expectedPos.x);
    EXPECT_FLOAT_EQ(largeAsteroid->getPosition().y, expectedPos.y);
}

TEST_F(AsteroidTest, ScreenWrapping) {
    // Test horizontal wrapping
    largeAsteroid->setPosition(sf::Vector2f(-10, WINDOW_HEIGHT/2));
    largeAsteroid->update(0.016f);
    EXPECT_GT(largeAsteroid->getPosition().x, 0);
    
    largeAsteroid->setPosition(sf::Vector2f(WINDOW_WIDTH + 10, WINDOW_HEIGHT/2));
    largeAsteroid->update(0.016f);
    EXPECT_LT(largeAsteroid->getPosition().x, WINDOW_WIDTH);
    
    // Test vertical wrapping
    largeAsteroid->setPosition(sf::Vector2f(WINDOW_WIDTH/2, -10));
    largeAsteroid->update(0.016f);
    EXPECT_GT(largeAsteroid->getPosition().y, 0);
    
    largeAsteroid->setPosition(sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT + 10));
    largeAsteroid->update(0.016f);
    EXPECT_LT(largeAsteroid->getPosition().y, WINDOW_HEIGHT);
}
