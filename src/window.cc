#include "window.h"
#include <SFML/Graphics.hpp>
#include <iostream>

SFMLWindow::SFMLWindow(int width, int height) 
    : window(sf::VideoMode(width, height), "SFML Window") {

    // Set up colours.
    colours[White] = sf::Color::White;
    colours[Black] = sf::Color::Black;
    colours[Red] = sf::Color::Red;
    colours[Green] = sf::Color::Green;
    colours[Blue] = sf::Color::Blue;

    // Initialize the window with a white background
    window.clear(colours[White]);
    window.display();
}

SFMLWindow::~SFMLWindow() {
    window.close();
}

void SFMLWindow::fillRectangle(int x, int y, int width, int height, int colour) {
    sf::RectangleShape rect(sf::Vector2f(width, height));
    rect.setPosition(x, y);
    rect.setFillColor(colours[colour]);
    window.draw(rect);
    window.display();
}

void SFMLWindow::drawString(int x, int y, std::string msg, int colour) {
    sf::Font font;
    if (!font.loadFromFile("/System/Library/Fonts/Supplemental/arial.ttf")) {
        std::cerr << "Error loading font\n";
        return;
    }

    sf::Text text;
    text.setFont(font);
    text.setString(msg);
    text.setCharacterSize(24); // Character size in pixels
    text.setFillColor(colours[colour]);
    text.setPosition(x, y);

    window.draw(text);
    window.display();
}

void SFMLWindow::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}
