// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

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
                    grid[row][col] = terrain; //fix

                    if (terrain == '@') {
                        start.first = row;
                        start.second = col;
                    }
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

bool Treasure_Hunt::is_valid_index(char c, std::pair<size_t, size_t> coordinate){
    if (c == 'N') return (coordinate.first - 1 >= 0);

    if (c == 'E') return (coordinate.second + 1 < grid.size());

    if (c == 'S') return (coordinate.first + 1 < grid.size());

    if (c == 'W') return (coordinate.second - 1 >= 0);

    return false;
}

void Treasure_Hunt::captain_search() {
    std::cout << "under construction";

    //start the investigation loop from the starting point
    //
}

void Treasure_Hunt::first_mate_search(std::pair<size_t, size_t> coordinate) {

    first_mate_container.push_back(coordinate); //push_front

    while(!first_mate_container.empty()){

        for (auto direction: hunt_order) { //ex "nesw" "esnw", etc.
            first_mate_container.pop_back();

            if (direction == 'N' && is_valid_index(direction, coordinate)) { //check if it is a valid index
                if (grid[coordinate.first - 1][coordinate.second] == '$') {
                    treasure = std::pair(coordinate.first - 1, coordinate.second);
                    return;
                } //treasure found!
                else if (grid[coordinate.first - 1][coordinate.second] == 'o') {
                    first_mate_container.push_back(std::pair(coordinate.first - 1, coordinate.second));
                } //more land found - add to the container
                else continue;
            } //NORTH

            if (direction == 'E' && is_valid_index(direction, coordinate)) { //check if it is a valid index
                if (grid[coordinate.first][coordinate.second + 1] == '$') {
                    treasure = std::pair(coordinate.first, coordinate.second + 1);
                    return;
                } //treasure found!
                else if (grid[coordinate.first][coordinate.second + 1] == 'o') {
                    first_mate_container.push_back(std::pair(coordinate.first, coordinate.second + 1));
                } //more land found - add to the container
                else continue;
            } //EAST

            if (direction == 'S' && is_valid_index(direction, coordinate)) { //check if it is a valid index
                if (grid[coordinate.first + 1][coordinate.second] == '$') {
                    treasure = std::pair(coordinate.first + 1, coordinate.second);
                    return;
                } //treasure found!
                else if (grid[coordinate.first + 1][coordinate.second] == 'o') {
                    first_mate_container.push_back(std::pair(coordinate.first + 1, coordinate.second));
                } //more land found - add to the container
                else continue;
            } //SOUTH

            if (direction == 'W' && is_valid_index(direction, coordinate)) { //check if it is a valid index
                if (grid[coordinate.first][coordinate.second - 1] == '$') {
                    treasure = std::pair(coordinate.first, coordinate.second - 1);
                    return;
                } //treasure found!
                else if (grid[coordinate.first][coordinate.second - 1] == 'o') {
                    first_mate_container.push_back(std::pair(coordinate.first, coordinate.second - 1));
                } //more land found - add to the container
                else continue;
            } //WEST
            
        }
    }
    return;
}


