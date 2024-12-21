#!/bin/bash

# Install required libraries
echo "Installing SFML and Google Test libraries..."
sudo apt update
sudo apt install -y libsfml-dev googletest libgtest-dev

# Create main project files
touch CMakeLists.txt
touch .gitignore
touch README.md

# Create asset directories
mkdir -p assets/images/{asteroids,ship,effects}
mkdir -p assets/sounds
mkdir -p assets/fonts

# Create include directory and files
mkdir -p include
cd include
touch GameObject.hpp
touch Ship.hpp
touch Asteroid.hpp
touch Bullet.hpp
touch CollisionManager.hpp
touch ResourceManager.hpp
touch ScoreManager.hpp
touch GameState.hpp
touch Constants.hpp
touch Vector2D.hpp
touch Random.hpp
cd ..

# Create src directory and files
mkdir -p src
cd src
touch GameObject.cpp
touch Ship.cpp
touch Asteroid.cpp
touch Bullet.cpp
touch CollisionManager.cpp
touch ResourceManager.cpp
touch ScoreManager.cpp
touch GameState.cpp
touch Vector2D.cpp
touch Random.cpp
touch main.cpp
cd ..

# Create tests directory and files
mkdir -p tests
cd tests
touch CMakeLists.txt
touch ShipTest.cpp
touch AsteroidTest.cpp
touch BulletTest.cpp
touch CollisionManagerTest.cpp
touch Vector2DTest.cpp
cd ..

echo "Setup complete! Libraries installed and project structure created."
