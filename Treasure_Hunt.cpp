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

        map.resize(map_size, std::vector<Point>(map_size));

        std::cin.ignore(); //ignore newline char

        if (format == 'M') {
            //read the map format
            std::string line;
            size_t row = 0;

            while(std::getline(std::cin, line)) {
                if (line.empty()) continue;

                for (size_t col = 0; col < map_size; ++col) {
                    Point point('X', line[col], row, col, false);
                    map[row][col] = point; //fix

                    if (line[col] == '@') {
                        start.row = row;
                        start.col = col;
                    }

                   if (line[col] == '$') {
                        treasure.row = row;
                        treasure.col = col;
                    } 
                }

                row++;
            }
        } 
        else {
            //read the list format
            for(size_t row = 0; row < map.size(); ++row){
                for(size_t col = 0; col < map.size(); ++col) {
                    map[row][col].value = '.';
                    map[row][col].row = row;
                    map[row][col].col = col;
                }
            }
            std::string line;

            while(std::getline(std::cin, line)) {
                if (line.empty()) continue;

                size_t row = static_cast<size_t>(line[0] - '0');
                size_t col = static_cast<size_t>(line[2] - '0');
                char terrain = line[4];

                if (terrain == '@') {
                        start.value = '@';
                        start.row = row;
                        start.col = col;
                }

                if (terrain == '$') {
                        treasure.row = row;
                        treasure.col = col;
                }

                map[row][col].value = terrain;
                map[row][col].row = row;
                map[row][col].col = col;  
            }

        } 
}

bool Treasure_Hunt::is_valid_index(char c, size_t row, size_t col){
    if (c == 'N') return (row - 1 >= 0);

    if (c == 'E') return (col + 1 < map.size());

    if (c == 'S') return (row + 1 < map.size());

    if (c == 'W') return (col - 1 >= 0);

    return false;
}

void Treasure_Hunt::captain_search() {
    if(verbose) std::cout << "Treasure hunt started at: " << start.row << ","
                          << start.col << "\n";

    //Add the Starting Location (@ on the map) to the sail container.
    add_to_container('c', start);
    //2. If the sail container is empty, the hunt has ended (jump to Step 5). 
       //If not, set the “sail location” to the “next” available location in the sail container 
       //(where next is front for queue, top for stack) and remove it from the sail container.
    while(!captain_container.empty()) {
        Point curr = captain_container.back();
        map[curr.row][curr.col].discovered = true;
        captain_container.pop_back();

        for (auto direction : hunt_order) {
            if (is_valid_index(direction, curr.row, curr.col)) {
                Point next = curr;
                next.move(direction);
                Point &p = map[next.row][next.col];

                if (p.value == '.' && !p.discovered) {
                    p.discovered = true;
                    p.direction = direction;
                    add_to_container('c',p);

                }
                else if (p.value == 'o' && !p.discovered) {
                    p.direction = direction;
                    first_mate_search(p);
                    if (treasure.discovered) return;
                }
                else if (p.value == '$'){
                    p.discovered = true;
                    p.direction = direction;
                    return;
                }
                else continue;
            }
        }
        
        if(curr.value != '@') water_locations++;

    }
    std::cout << "Treasure hunt failed\n";
}

void Treasure_Hunt::first_mate_search(Point &point) {
    went_ashore++;

    if(verbose) std::cout << "Went ashore at: " << point.row << "," << point.col << "\n"
                          << "Searching island... "; 

    point.discovered = true;

    add_to_container('f',point); 

    while(!first_mate_container.empty()){
        Point curr = first_mate_container.back();
        map[curr.row][curr.col].discovered = true;
        first_mate_container.pop_back();
        land_locations++;

        for (auto direction : hunt_order) {
            if (is_valid_index(direction, curr.row, curr.col)) {
                curr.move(direction);
                if (map[curr.row][curr.col].value == 'o' && !map[curr.row][curr.col].discovered) {
                    map[curr.row][curr.col].discovered = true;
                    map[curr.row][curr.col].direction = direction;
                    add_to_container('f', map[curr.row][curr.col]);
                }
                else if (map[curr.row][curr.col].value == '$') {
                    treasure.discovered = true;
                    if(verbose) std::cout << "party found treasure at " << curr.row << "," << curr.col << ".\n";
                    return;
                }
                else continue;
            }
        }
    }
    
    if(verbose) std::cout << "party returned with no treasure\n";
    return;
}

void Treasure_Hunt::print_grid() {
    for (size_t row = 0; row < map.size(); ++row) {
        for (size_t col = 0; col < map.size(); ++col) {
            std::cout << map[row][col].value;
        }
        std::cout << "\n";
    }
}

void Treasure_Hunt::print_verbose() {
    std::cout << "Treasure hunt started at: " << start.row << "," 
              << start.col << "\n";
}

void Treasure_Hunt::add_to_container(char c, Point p){
    if (c == 'c') {
        if (captain_container_type == 'S') captain_container.push_back(p);
        else captain_container.push_front(p);
    }

    else {
        if (first_mate_container_type == 'S') first_mate_container.push_back(p);
        else first_mate_container.push_front(p);
    }
}


void Treasure_Hunt::print_stats(){
    std::cout << "--- STATS ---\n";
    std::cout << "Starting location: " << start.row << "," << start.col << "\n";
    std::cout << "Water locations investigated: " << water_locations << "\n";
    std::cout << "Land locations investigated: " << land_locations << "\n";
    std::cout << "Went ashore: " << went_ashore << "\n";
    if (treasure.discovered) {
        std::cout << "Path length: 7\n";
        std::cout << "Treasure location: " 
                  << treasure.row << "," << treasure.col << "\n";
    }
    std::cout << "--- STATS ---\n";
}

void Treasure_Hunt::hunt(){

    if (!treasure.discovered) std::cout << "No treasure found after investigating" << "5" << "locations.\n";
    else std::cout << "Treasure found at " << treasure.row << "," << treasure.col << " with path length " << "length.\n"; 

    if (stats) print_stats();

    //if (show_path) print_path();

}

void Treasure_Hunt::backtrace() {
    Point curr = treasure;

    while(curr.value != '@') {
        switch(curr.direction) {
            case 'N':
                curr = map[curr.row + 1][curr.col];
                break;
            case 'E':
                curr = map[curr.row][curr.col - 1];
                break;
            case 'S':
                curr = map[curr.row + 1][curr.col];
                break;
            case 'W':
                curr = map[curr.row][curr.col + 1];
                break;
        }
        path_length++;
    }
}