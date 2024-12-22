// tests/BulletTest.cpp
#include <gtest/gtest.h>
#include "Bullet.hpp"

class BulletTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create bullet moving straight up
        startPos = sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
        bullet = std::make_unique<Bullet>(startPos, 0.0f);  // 0 degrees = straight up
    }
    
    std::unique_ptr<Bullet> bullet;
    sf::Vector2f startPos;
};

TEST_F(BulletTest, InitialState) {
    EXPECT_EQ(bullet->getPosition(), startPos);
    EXPECT_FALSE(bullet->hasExpired());
    EXPECT_FALSE(bullet->isOffScreen());
}

TEST_F(BulletTest, Movement) {
    float deltaTime = 0.016f;  // 60 FPS
    bullet->update(deltaTime);
    
    // Bullet should have moved up (negative y in SFML)
    EXPECT_EQ(bullet->getPosition().x, startPos.x);
    EXPECT_LT(bullet->getPosition().y, startPos.y);
}

TEST_F(BulletTest, Expiration) {
    // Update many times to make bullet travel far
    for (int i = 0; i < 1000; i++) {
        bullet->update(0.016f);
        if (bullet->hasExpired()) break;
    }
    
    EXPECT_TRUE(bullet->hasExpired());
}

TEST_F(BulletTest, OffScreen) {
    // Set position just above top of screen
    bullet->setPosition(sf::Vector2f(WINDOW_WIDTH/2, -10));
    EXPECT_TRUE(bullet->isOffScreen());
    
    // Test other screen boundaries
    bullet->setPosition(sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT + 10));
    EXPECT_TRUE(bullet->isOffScreen());
    
    bullet->setPosition(sf::Vector2f(-10, WINDOW_HEIGHT/2));
    EXPECT_TRUE(bullet->isOffScreen());
    
    bullet->setPosition(sf::Vector2f(WINDOW_WIDTH + 10, WINDOW_HEIGHT/2));
    EXPECT_TRUE(bullet->isOffScreen());
    
    // Test in-screen position
    bullet->setPosition(sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2));
    EXPECT_FALSE(bullet->isOffScreen());
}
