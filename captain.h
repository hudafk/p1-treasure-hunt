#ifndef CAPTAIN_H
#define CAPTAIN_H

#include <iostream>
#include <vector>
#include <deque>


enum class Color { Red, Green, Blue };

void printFavoriteColor() {
    Color favorite_color = Color::Red;

    switch (favorite_color) {
        case Color::Red:
            std::cout << "red" << std::endl;
            break;
        case Color::Green:
            std::cout << "green" << std::endl;
            break;
        case Color::Blue:
            std::cout << "blue" << std::endl;
            break;
    }
}



#endif 