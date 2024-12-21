#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

class Game {
public:
    Game() : window(sf::VideoMode::getFullscreenModes()[0], "Asteroids", sf::Style::Fullscreen) {

        // Disable joystick detection to prevent warnings
        window.setJoystickThreshold(100);
        /// sf::Joystick::update();  // Force initial update

        if (!font.loadFromFile("assets/fonts/PressStart2P-Regular.ttf")) {
            throw std::runtime_error("Failed to load font!");
        }

        // Set up debug text
        debugText.setFont(font);
        debugText.setCharacterSize(24);
        debugText.setFillColor(sf::Color::White);
        debugText.setPosition(10.f, 10.f);
        debugText.setString("Press any key to exit");
    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text debugText;

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                event.type == sf::Event::KeyPressed) {
                window.close();
            }
        }
    }

    void update() {
        // Will add game logic here later
    }

    void render() {
        window.clear(sf::Color::Black);
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
