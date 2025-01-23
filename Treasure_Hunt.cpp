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
                        start.discovered = true;
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
        add_to_container('c', start);
}

bool Treasure_Hunt::is_valid_index(char c, size_t row, size_t col){
    if (c == 'N') return (static_cast<int>(row) - 1 >= 0);

    if (c == 'E') return (col + 1 < map.size());

    if (c == 'S') return (row + 1 < map.size());

    if (c == 'W') return (static_cast<int>(col) - 1 >= 0);

    return false;
}

void Treasure_Hunt::captain_search(Coordinate c) {
    if(verbose) std::cout << "Treasure hunt started at: " << start.row << ","
                          << start.col << "\n";
        
        while (!captain_container.empty()){
            c.row = captain_container.back().row;
            c.col = captain_container.back().col;
            captain_container.pop_back();
            water_locations++;

            for (auto direction : hunt_order) {
                Coordinate next = c;
                if (is_valid_index(direction, next.row, next.col)) {
                    move(next, direction);
                    //Point &point = map[p.row][p.col];
                    if (map[next.row][next.col].value == '.' && !map[next.row][next.col].discovered) {
                        map[next.row][next.col].discovered = true;
                        map[next.row][next.col].direction = direction;
                        add_to_container('c', map[next.row][next.col]);

                    }

                    else if (map[next.row][next.col].value == 'o' && !map[next.row][next.col].discovered) {
                        map[next.row][next.col].direction = direction;
                        map[next.row][next.col].discovered = true;
                        first_mate_search(next);
                        if (treasure.discovered) {
                            land_locations++;
                            return;
                        }
                    }
                    else if (map[next.row][next.col].value == '$'){
                        map[next.row][next.col].discovered = true;
                        map[next.row][next.col].direction = direction;
                        land_locations++;
                        return;
                    }
                    else continue;
                }
            }
    }
        
    std::cout << "Treasure hunt failed\n";
}

void Treasure_Hunt::first_mate_search(Coordinate c) {
    went_ashore++;

    if(verbose) std::cout << "Went ashore at: " << c.row << "," << c.col << "\n"
                          << "Searching island... "; 

    add_to_container('f', map[c.row][c.col]); 

    while(!first_mate_container.empty()){
        Point curr = first_mate_container.back();
        c.row = curr.row;
        c.col = curr.col;
        land_locations++;
        first_mate_container.pop_back();

        for (auto direction : hunt_order) {
            if (is_valid_index(direction, curr.row, curr.col)) {
                Coordinate next = c;
                move(next, direction);
                if (map[next.row][next.col].value == 'o' && !map[next.row][next.col].discovered) {
                    map[next.row][next.col].discovered = true;
                    map[next.row][next.col].direction = direction;
                    add_to_container('f', map[next.row][next.col]);
                }
                else if (map[next.row][next.col].value == '$') {
                    treasure.discovered = true;
                    treasure.direction = direction;
                    if(verbose) std::cout << "party found treasure at " << next.row << "," << next.col << ".\n";
                    return;
                }
                else continue;
            }
        }
    }
    
    if(verbose) std::cout << "party returned with no treasure.\n";
    return;
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

void Treasure_Hunt::print_path() {
    std::cout << "under construction\n";
    return;
}

void Treasure_Hunt::print_stats(){
    std::cout << "--- STATS ---\n";
    std::cout << "Starting location: " << start.row << "," << start.col << "\n";
    std::cout << "Water locations investigated: " << water_locations << "\n";
    std::cout << "Land locations investigated: " << land_locations << "\n";
    std::cout << "Went ashore: " << went_ashore << "\n";
    if (treasure.discovered) {
        std::cout << "Path length: " << path_length << "\n";
        std::cout << "Treasure location: " 
                  << treasure.row << "," << treasure.col << "\n";
    }
    std::cout << "--- STATS ---\n";
}

void Treasure_Hunt::hunt(){

    Coordinate c;
    c.row = captain_container.back().row;
    c.col = captain_container.back().col;

    captain_search(c);

    backtrace();

    if (!treasure.discovered) std::cout << "No treasure found after investigating" << "5" << "locations.\n";
    else std::cout << "Treasure found at " << treasure.row << "," << treasure.col << " with path length " << path_length << ".\n"; 

    if (stats) print_stats();
    
    if (show_path) print_path();  
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

void Treasure_Hunt::move(Coordinate &c, char direction) {
       switch (direction) {
        case 'N':
            c.row--;
            break;
        case 'E':
            c.col++;
            break;
        case 'S':
            c.row++;
            break;
        case 'W':
            c.col--;
            break;
        default:
            std::cout << "invalid character!\n";
       }

}