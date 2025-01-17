#include "Treasure_Hunt.h"
#include <iostream>

void Treasure_Hunt::create_grid() {
        std:: string line;

        while(std::getline(std::cin, line)) {
            if (line[0] != '#') break;
        }

        if (line.empty() || (line[0] != 'M' && line[0] != 'L')) throw std::runtime_error("the file isn't in correct format!");

        int format = line[0]; //take in 'M' or 'L' for format

        size_t map_size; //the size of the map

        if (!(std::cin >> map_size) || map_size < 2) throw std::runtime_error("map is too small!"); 

        std::cin.ignore(); //ignore newline char

        if (format == 'M') {
            //read the map format
            std::string line;
            size_t row = 0;

            while(std::getline(std::cin, line)) {
                if (line.empty()) continue;

                for (size_t col = 0; col < map_size; ++col) {
                    char terrain;
                    std::cin >> terrain;
                    grid[row][col] = terrain;
                }

                row++;
            }
        } 
        else {
            //read the list format
            std::string line;

            while(std::getline(std::cin, line)) {
                if (line.empty()) continue;

                size_t row = static_cast<size_t>(line[0]);
                size_t col = static_cast<size_t>(line[1]);
                char terrain = line[2];

                grid[row][col] = terrain;   
            }

        }
        
}

