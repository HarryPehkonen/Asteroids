# tests/CMakeLists.txt
find_package(GTest REQUIRED)
enable_testing()

# Add test executable with only the tests we still need
add_executable(gamestate_test 
    GameStateTest.cpp
    ShipTest.cpp
    AsteroidTest.cpp
    BulletTest.cpp
)

# Link against GTest and our game library
target_link_libraries(gamestate_test
    PRIVATE
    GTest::GTest
    GTest::Main
    asteroids_lib
)

# Add the test to CTest
add_test(NAME gamestate_test COMMAND gamestate_test)
