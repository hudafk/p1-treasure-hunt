// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#include "Treasure_Hunt.h"
#include <iostream>

void Treasure_Hunt::create_map() {
        std:: string line;

        while(std::getline(std::cin, line)) {
            if (line[0] != '#') break;
        }

        if (line.empty() || (line[0] != 'M' && line[0] != 'L')) throw std::runtime_error("the file isn't in correct format!");

        char format = line[0]; //take in 'M' or 'L' for format

        size_t map_size; //the size of the map


        if (!(std::cin >> map_size) || map_size < 2) throw std::runtime_error("map is too small!"); 

        map.resize(map_size, std::vector<char>(map_size));

        std::cin.ignore(); //ignore newline char

        if (format == 'M') {
            //read the map format
            std::string line;
            size_t row = 0;

            while(std::getline(std::cin, line)) {
                if (line.empty()) continue;

                for (size_t col = 0; col < map_size; ++col) {
                    char terrain = line[col];
                    map[row][col] = terrain; //fix

                    if (terrain == '@') {
                        start.row = row;
                        start.col = col;
                        start.value = terrain;
                    }
                }

                row++;
            }
        } 
        else {
            //read the list format
            for(size_t row = 0; row < map.size(); ++row){
                for(size_t col = 0; col < map.size(); ++col) {
                    map[row][col] = '.';
                }
            }

            std::string line;

            while(std::getline(std::cin, line)) {
                if (line.empty()) continue;

                size_t row = static_cast<size_t>(line[0] - '0');
                size_t col = static_cast<size_t>(line[2] - '0');
                char terrain = line[4];

                map[row][col] = terrain;   
            }

        } 
}

bool Treasure_Hunt::is_valid_index(char c, std::pair<size_t, size_t> coordinate){
    if (c == 'N') return (coordinate.first - 1 >= 0);

    if (c == 'E') return (coordinate.second + 1 < map.size());

    if (c == 'S') return (coordinate.first + 1 < map.size());

    if (c == 'W') return (coordinate.second - 1 >= 0);

    return false;
}

void Treasure_Hunt::captain_search() {
    std::cout << "under construction";

    //Add the Starting Location (@ on the map) to the sail container.
    captain_container.push_back(start);
    //2. If the sail container is empty, the hunt has ended (jump to Step 5). 
       //If not, set the “sail location” to the “next” available location in the sail container 
       //(where next is front for queue, top for stack) and remove it from the sail container.
    while(!captain_container.empty()) {
        Point curr = captain_container.front();
        captain_container.pop_front();


    }
    //3.From the sail location, add any adjacent water locations that are not impassable
    //and have not already been discovered to the sail container. 
    //Discover new locations as dictated by the Hunt Order. 
    //The Captain will only add water locations to the sail container.
    //If the Captain discovers land, the First Mate will be immediately put ashore 
    //(before the captain examines other adjacent locations) 
    //to start a search party at that location using a separate container; 
    //jump to Step 1 of the First Mate’s Hunting. When the First Mate finishes hunting, 
    //if the treasure was found, jump to Step 5. If the treasure was not found, 
    //the Captain should continue investigating any remaining locations around the sail location.
    
    //4.Repeat from Step 2.

    //5.Report the outcome of the hunt (see Output Format).
    if(verbose) verbose();

    if(stats) stats();

    if(show_path) show_path();


}

void Treasure_Hunt::first_mate_search(std::pair<size_t, size_t> coordinate) {

    first_mate_container.push_back(coordinate); //push_front

    while(!first_mate_container.empty()){

        for (auto direction: hunt_order) { //ex "nesw" "esnw", etc.
            first_mate_container.pop_back();

            if (direction == 'N' && is_valid_index(direction, coordinate)) { //check if it is a valid index
                if (map[coordinate.first - 1][coordinate.second] == '$') {
                    treasure = std::pair(coordinate.first - 1, coordinate.second);
                    return;
                } //treasure found!
                else if (map[coordinate.first - 1][coordinate.second] == 'o') {
                    first_mate_container.push_back(std::pair(coordinate.first - 1, coordinate.second));
                } //more land found - add to the container
                else continue;
            } //NORTH

            if (direction == 'E' && is_valid_index(direction, coordinate)) { //check if it is a valid index
                if (map[coordinate.first][coordinate.second + 1] == '$') {
                    treasure = std::pair(coordinate.first, coordinate.second + 1);
                    return;
                } //treasure found!
                else if (map[coordinate.first][coordinate.second + 1] == 'o') {
                    first_mate_container.push_back(std::pair(coordinate.first, coordinate.second + 1));
                } //more land found - add to the container
                else continue;
            } //EAST

            if (direction == 'S' && is_valid_index(direction, coordinate)) { //check if it is a valid index
                if (map[coordinate.first + 1][coordinate.second] == '$') {
                    treasure = std::pair(coordinate.first + 1, coordinate.second);
                    return;
                } //treasure found!
                else if (map[coordinate.first + 1][coordinate.second] == 'o') {
                    first_mate_container.push_back(std::pair(coordinate.first + 1, coordinate.second));
                } //more land found - add to the container
                else continue;
            } //SOUTH

            if (direction == 'W' && is_valid_index(direction, coordinate)) { //check if it is a valid index
                if (map[coordinate.first][coordinate.second - 1] == '$') {
                    treasure = std::pair(coordinate.first, coordinate.second - 1);
                    return;
                } //treasure found!
                else if (map[coordinate.first][coordinate.second - 1] == 'o') {
                    first_mate_container.push_back(std::pair(coordinate.first, coordinate.second - 1));
                } //more land found - add to the container
                else continue;
            } //WEST
            
        }
    }
    return;
}


void Treasure_Hunt::print_grid() {
    for (size_t row = 0; row < map.size(); ++row) {
        for (size_t col = 0; col < map.size(); ++col) {
            std::cout << map[row][col];
        }
        std::cout << "\n";
    }
}

void verbose() {
    std::cout << "Treasure hunt started at: ";
}

