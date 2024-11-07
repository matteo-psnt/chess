#include "window.h"
#include <SFML/Graphics.hpp>
#include <iostream>

SFMLWindow::SFMLWindow(int width, int height) 
    : window(sf::VideoMode(width, height), "Chess Game") {

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
}

std::unordered_map<char, char> pieceSymbolMap = {
    {'K', 'k'}, // White King
    {'Q', 'q'}, // White Queen
    {'R', 'r'}, // White Rook
    {'B', 'b'}, // White Bishop
    {'N', 'h'}, // White Knight
    {'P', 'p'}, // White Pawn
    {'k', 'l'}, // Black King
    {'q', 'w'}, // Black Queen
    {'r', 't'}, // Black Rook
    {'b', 'n'}, // Black Bishop
    {'n', 'j'}, // Black Knight
    {'p', 'o'}  // Black Pawn
};

void SFMLWindow::drawPeice(int x, int y, char piece, int colour) {
    sf::Font font;
    if (!font.loadFromFile("./assets/CHEQ_TT.TTF")) {
        std::cerr << "Error loading font\n";
        return;
    }

    sf::Text text;
    text.setFont(font);
    
    // Use mapped character for the piece
    auto it = pieceSymbolMap.find(piece);
    if (it != pieceSymbolMap.end()) {
        text.setString(std::string(1, it->second));
    } else {
        text.setString(std::string(1, piece));
    }

    text.setCharacterSize(50);
    text.setFillColor(colours[colour]);
    text.setPosition(x, y);

    window.draw(text);
}

void SFMLWindow::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void SFMLWindow::clear(int colour) {
    window.clear(colours[colour]);
}

void SFMLWindow::display() {
    window.display();
}
