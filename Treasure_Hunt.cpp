// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
#include "Treasure_Hunt.h"
#include <iostream>

void Treasure_Hunt::create_map() {
        bool is_start = false;
        bool is_treasure = false;

        std:: string line;

        while(std::getline(std::cin, line)) {
            if (line[0] != '#') break;
        }

        if (line.empty() || (line[0] != 'M' && line[0] != 'L')) {
            std::cerr << ("Unknown option");
            exit(1);
        }

        char format = line[0]; //take in 'M' or 'L' for format

        size_t map_size; //the size of the map


        if (!(std::cin >> map_size) || map_size < 2) {
            std::cerr << ("map is too small!"); 
            exit(1);
        }

        map.resize(map_size, std::vector<Point>(map_size));

        std::cin.ignore(); //ignore newline char

        if (format == 'M') {
            //read the map format
            std::string line;
            size_t row = 0;

            while(std::getline(std::cin, line)) {
                if (line.empty()) continue;

                for (size_t col = 0; col < map_size; ++col) {
                    if(line[col] != '#' && line[col] != '.' && line[col] != 'o' 
                        && line[col] != '$' && line[col] != '@') {
                            std::cerr << "Invalid terrain type";
                            exit(1);
                    }

                    Point point('X', line[col], row, col, false);
                    map[row][col] = point; //fix

                    if (line[col] == '@') {
                        is_start = true;
                        start.row = row;
                        start.col = col;
                    }

                   if (line[col] == '$') {
                        is_treasure = true;
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

                if (int(line[0] - '0') < 0 || int(line[0] - '0') >= static_cast<int>(map_size) 
                    || int(line[2] - '0') < 0 || int(line[2] - '0') >= static_cast<int>(map_size) ) {
                        std::cerr << "Invalid coordinates in list mode input";
                        exit(1);
                }

                size_t row = static_cast<size_t>(line[0] - '0');
                size_t col = static_cast<size_t>(line[2] - '0');
                char terrain = line[4];

                if (terrain == '@') {
                        start.value = '@';
                        start.row = row;
                        start.col = col;
                        start.discovered = true;
                        is_start = true;
                }

                if (terrain == '$') {
                        treasure.row = row;
                        treasure.col = col;
                        is_treasure = true;
                }

                map[row][col].value = terrain;
                map[row][col].row = row;
                map[row][col].col = col;  
            }
        } 
        if(!is_treasure) {
            std::cerr << "Map does not have treasure";
            if(!is_start) std::cerr << "\nMap does not have a start location";
            exit(1);
        }
        else if (!is_start) {
            std::cerr << "Map does not have a start location";
            exit(1);
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
                        treasure.discovered = true;
                        treasure.direction = direction;
                        treasure.value = '$';
                        map[next.row][next.col].value = treasure.value;
                        map[next.row][next.col].discovered = true;
                        map[next.row][next.col].direction = direction;
                        land_locations++;
                        went_ashore++;
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
                    treasure.value = '$';
                    map[next.row][next.col].value = treasure.value;
                    treasure.direction = direction;
                    map[next.row][next.col].discovered = true;
                    map[next.row][next.col].direction = direction;
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
    if(path_type == 'M') {
        for (size_t row = 0; row < map.size(); ++row) {
            for (size_t col = 0; col < map.size(); ++col) {
                std::cout << map[row][col].value;
            }
            std::cout << "\n";
        }
    } 
    else { //path_type == 'L'
        std::cout << "Sail:\n";
        Point curr;

        while (!sail_path.empty()) {
            curr = sail_path.back();
            std::cout << curr.row << "," << curr.col << "\n";
            sail_path.pop_back();
        }

        std::cout << "Search:\n";
        curr = search_path.back();
        while (!search_path.empty()) {
            curr = search_path.back();
            std::cout << curr.row << "," << curr.col << "\n";
            search_path.pop_back();
        }

    }
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

    if (treasure.discovered) backtrace();

    if (stats) print_stats();
    
    if (show_path) print_path();  

    if (!treasure.discovered) std::cout << "No treasure found after investigating" << "5" << "locations.";
    else std::cout << "Treasure found at " << treasure.row << "," << treasure.col << " with path length " << path_length << ".\n"; 
}

void Treasure_Hunt::backtrace() {
    Point curr = treasure;
    if(show_path) {
        if(path_type == 'M') map[curr.row][curr.col].value = 'X';
        else search_path.push_back(map[curr.row][curr.col]);

    }

    while(curr.value != '@') {
        switch(curr.direction) {
            case 'N':
               if(show_path) {
                if (path_type == 'M') {
                    if (map[curr.row + 1][curr.col].value != '@') {
                        if (map[curr.row + 1][curr.col].direction == 'E' 
                        || map[curr.row + 1][curr.col].direction == 'W') map[curr.row + 1][curr.col].value = '+';
                        else map[curr.row + 1][curr.col].value = '|';
                    }
               } else //path_typ == 'L'
                    if (map[curr.row + 1][curr.col].value == 'o') search_path.push_back(map[curr.row + 1][curr.col]);
                    else sail_path.push_back(map[curr.row + 1][curr.col]);
               }
                curr = map[curr.row + 1][curr.col];
                break;
            case 'E':
                if(show_path) {
                    if (path_type == 'M') {
                        if (map[curr.row][curr.col - 1].value != '@') {
                            if (map[curr.row][curr.col - 1].direction == 'N' 
                                || map[curr.row][curr.col - 1].direction == 'S') map[curr.row][curr.col - 1].value = '+';
                            else map[curr.row][curr.col - 1].value = '-';
                        }
                    } else //path_type == 'L'
                        if (map[curr.row + 1][curr.col].value == 'o') search_path.push_back(map[curr.row + 1][curr.col]);
                        else sail_path.push_back(map[curr.row][curr.col - 1]);
                }
                curr = map[curr.row][curr.col - 1];
                break;
            case 'S':
                if(show_path) {
                    if (path_type == 'M') {
                        if (map[curr.row - 1][curr.col].value != '@') {
                            if (map[curr.row - 1][curr.col].direction == 'E' 
                                || map[curr.row - 1][curr.col].direction == 'W') map[curr.row - 1][curr.col].value = '+';
                                else map[curr.row - 1][curr.col].value = '|';
                        } else
                        if (map[curr.row - 1][curr.col].value == 'o') search_path.push_back(map[curr.row + 1][curr.col]);
                        else sail_path.push_back(map[curr.row - 1][curr.col]);
                    }
                }
                curr = map[curr.row - 1][curr.col];
                break;
            case 'W':
                if(show_path) {
                    if(path_type == 'M') {
                        if (map[curr.row][curr.col + 1].value != '@') {
                            if (map[curr.row][curr.col + 1].direction == 'N' 
                                || map[curr.row][curr.col + 1].direction == 'S') map[curr.row][curr.col + 1].value = '+';
                                else map[curr.row][curr.col + 1].value = '-';
                        }
                    } else 
                     if (map[curr.row][curr.col + 1].value == 'o') search_path.push_back(map[curr.row][curr.col + 1]);
                     else sail_path.push_back(map[curr.row][curr.col + 1]);
                }
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