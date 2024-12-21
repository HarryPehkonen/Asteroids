#pragma once

// Window settings
inline constexpr int WINDOW_WIDTH = 800;
inline constexpr int WINDOW_HEIGHT = 600;
inline constexpr char WINDOW_TITLE[] = "Asteroids";
inline constexpr char FONT_PATH[] = "assets/fonts/PressStart2P-Regular.ttf";

// Physics/movement
inline constexpr float SHIP_ROTATION_SPEED = 180.0f;  // degrees per second
inline constexpr float SHIP_ACCELERATION = 300.0f;     // pixels per second^2
inline constexpr float SHIP_MAX_SPEED = 400.0f;       // pixels per second
inline constexpr float DRAG_COEFFICIENT = 1.00;      // velocity multiplier per frame

// Asteroid settings
inline constexpr float LARGE_ASTEROID_RADIUS = 40.0f;
inline constexpr float MEDIUM_ASTEROID_RADIUS = 20.0f;
inline constexpr float SMALL_ASTEROID_RADIUS = 10.0f;
inline constexpr int INITIAL_ASTEROID_COUNT = 4;

// Game rules
inline constexpr int POINTS_LARGE_ASTEROID = 20;
inline constexpr int POINTS_MEDIUM_ASTEROID = 50;
inline constexpr int POINTS_SMALL_ASTEROID = 100;
