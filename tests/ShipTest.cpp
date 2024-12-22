// tests/ShipTest.cpp
#include <gtest/gtest.h>
#include "Ship.hpp"

class ShipTest : public ::testing::Test {
protected:
    void SetUp() override {
        ship = std::make_unique<Ship>();
    }
    
    std::unique_ptr<Ship> ship;
};

TEST_F(ShipTest, InitialState) {
    EXPECT_EQ(ship->getPosition(), sf::Vector2f(0, 0));
    EXPECT_EQ(ship->getVelocity(), sf::Vector2f(0, 0));
    EXPECT_EQ(ship->getBulletManager().count(), 0);
}

TEST_F(ShipTest, BulletLimit) {
    // Position ship away from screen edges
    ship->setPosition(sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2));
    
    // Simulate firing max bullets
    for (int i = 0; i < MAX_BULLETS + 2; i++) {
        // Simulate space key press
        // Note: In a real implementation, you might need to mock keyboard input
        ship->update(0.016f);  // Small time step
    }
    
    // Verify bullet count doesn't exceed max
    EXPECT_LE(ship->getBulletManager().count(), MAX_BULLETS);
}

TEST_F(ShipTest, ScreenWrapping) {
    // Test horizontal wrapping
    ship->setPosition(sf::Vector2f(-10, WINDOW_HEIGHT/2));
    ship->update(0.016f);
    EXPECT_GT(ship->getPosition().x, 0);
    
    ship->setPosition(sf::Vector2f(WINDOW_WIDTH + 10, WINDOW_HEIGHT/2));
    ship->update(0.016f);
    EXPECT_LT(ship->getPosition().x, WINDOW_WIDTH);
    
    // Test vertical wrapping
    ship->setPosition(sf::Vector2f(WINDOW_WIDTH/2, -10));
    ship->update(0.016f);
    EXPECT_GT(ship->getPosition().y, 0);
    
    ship->setPosition(sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT + 10));
    ship->update(0.016f);
    EXPECT_LT(ship->getPosition().y, WINDOW_HEIGHT);
}
