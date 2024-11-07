#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class SFMLWindow {
    sf::RenderWindow window;
    sf::Color colours[5];

public:
    SFMLWindow(int width=500, int height=500);  // Constructor; displays the window.
    ~SFMLWindow();                              // Destructor; closes the window.
    SFMLWindow(const SFMLWindow&) = delete;
    SFMLWindow &operator=(const SFMLWindow&) = delete;

    enum {White=0, Black, Red, Green, Blue}; // Available colours.

    void fillRectangle(int x, int y, int width, int height, int colour=Black);
    void drawString(int x, int y, std::string msg, int colour=Black);
    void processEvents();
    bool isOpen() const;
};

#endif
