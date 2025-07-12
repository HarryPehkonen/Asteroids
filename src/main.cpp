// src/main.cpp
#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "Constants.hpp"
#include "GameState.hpp"

class Game {
public:
    Game() : window(sf::VideoMode::getFullscreenModes()[0], WINDOW_TITLE, sf::Style::Fullscreen) {

        if (!font.loadFromFile(FONT_PATH)) {
            throw std::runtime_error("Failed to load font!");
        }

        // Set up debug text
        debugText.setFont(font);
        debugText.setCharacterSize(24);
        debugText.setFillColor(sf::Color::White);
        debugText.setPosition(10.f, 10.f);
        // debugText.setString("Press Esc to exit");

        // Initialize game state
        gameState = std::make_unique<GameState>();
    }

    void run() {
        sf::Clock clock;
        
        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();
            
            processEvents();
            update(deltaTime);
            render();
        }
    }

private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text debugText;
    std::unique_ptr<GameState> gameState;

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && 
                 event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
        }
    }

    void update(float deltaTime) {
        gameState->update(deltaTime);
    }

    void render() {
        window.clear(sf::Color::Black);
        
        // Draw game objects
        gameState->draw(window);
        
        // Draw UI
        window.draw(debugText);
        
        window.display();
    }
};

int main() {
    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
