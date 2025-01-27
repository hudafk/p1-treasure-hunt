// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
#include "Treasure_Hunt.h"
#include <iostream>
#include <sstream>
#include <vector>

void Treasure_Hunt::create_map() {

        bool is_start = false;
        bool is_treasure = false;
        
        std:: string line;

        while(std::getline(std::cin, line)) {
            if (line[0] != '#') break;
        }

        if (line.empty() || (line[0] != 'M' && line[0] != 'L')) {
            std::cerr << ("Unknown option") << std::endl;
            exit(1);
        }

        char format = line[0]; //take in 'M' or 'L' for format

        int map_size; //the size of the map

        if (!(std::cin >> map_size) || map_size < 2) {

            std::cerr << ("Map is too small!") << std::endl; 

            exit(1);
        }

        map.resize(static_cast<size_t>(map_size), std::vector<Point>(static_cast<size_t>(map_size)));

        std::cin.ignore(); //ignore newline char

        if (format == 'M') { //read the map format

            std::string line;
            size_t row = 0;

            while(std::getline(std::cin, line)) {

                if (line.empty()) continue;

                for (size_t col = 0; col < static_cast<size_t>(map_size); ++col) {

                    if(line[col] != '#' && line[col] != '.' && line[col] != 'o' 

                        && line[col] != '$' && line[col] != '@') {

                            std::cerr << "Invalid terrain type" << std::endl;

                            exit(1);
                    }

                    Point point(' ', line[col]);

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

        else { //read the list format
            for(size_t row = 0; row < map.size(); ++row){
                for(size_t col = 0; col < map.size(); ++col) {
                    map[row][col].value = '.';
                }
            }

            std::string line;

            while(std::getline(std::cin, line)) {
                if (line.empty()) continue;

                std::istringstream line_stream(line);

                int r;
                int c;
                char terrain;

                line_stream >> r >> c >> terrain;

                if (r < 0 || r >= static_cast<int>(map_size)

                    || c < 0 || c >= static_cast<int>(map_size)) {

                        std::cerr << "Invalid coordinates in list mode input" << std::endl;
                        exit(1);
                }

                size_t row = static_cast<size_t>(r);
                size_t col = static_cast<size_t>(c);

                if (terrain == '@') {
                    if (is_start) {
                        std::cerr << "Already have a start location" << std::endl;
                        exit(1);
                    }
                        start.row = row;
                        start.col = col;
                        is_start = true;
                }

                if (terrain == '$') {
                    if (is_treasure) {
                        std::cerr << "Already have a treasure location" << std::endl;
                        exit(1);
                    }
                        treasure.row = row;
                        treasure.col = col;
                        is_treasure = true;
                }

                map[row][col].value = terrain;
            }
        } 

        if(!is_treasure) {
            std::cerr << "Map does not have treasure" << std::endl;
            exit(1);
        }

        if (!is_start) {
            std::cerr << "Map does not have a start location" << std::endl;
            exit(1);
        }
}

bool Treasure_Hunt::is_valid_index(char c, size_t row, size_t col){
    if (c == 'N') return (static_cast<int>(row) - 1 >= 0);

    if (c == 'E') return (col + 1 < map.size());

    if (c == 'S') return (row + 1 < map.size());

    if (c == 'W') return (static_cast<int>(col) - 1 >= 0);

    return false;
}

void Treasure_Hunt::captain_search() {

    std::deque<Coordinate> c_container; //captain container

    if (captain_container_type == 'S') c_container.push_back(start);
    else c_container.push_front(start);

    if(verbose) std::cout << "Treasure hunt started at: " << start.row << ","
                          << start.col << "\n";
        
    while (!c_container.empty()){

        Coordinate c = c_container.back();

        c_container.pop_back();
        water_locations++;

        for (auto direction : hunt_order) {

            Coordinate next = c;

            if (is_valid_index(direction, next.row, next.col)) {

                move(next, direction);

                if (map[next.row][next.col].value == '.' && map[next.row][next.col].direction == ' ') {
                    map[next.row][next.col].direction = direction;
                    if (captain_container_type == 'S') c_container.push_back(next);
                    else c_container.push_front(next);
                }

                else if (map[next.row][next.col].value == 'o' && map[next.row][next.col].direction == ' ') {
                    map[next.row][next.col].direction = direction;
                    first_mate_search(next);
                    if (map[treasure.row][treasure.col].direction != ' ') {
                        land_locations++;
                        return;
                    }
                }
                else if (map[next.row][next.col].value == '$'){
                    if(verbose) std::cout << "Went ashore at: " << next.row << "," << next.col << "\n"
                          << "Searching island... party found treasure at " << next.row << "," << next.col << ".\n"; 
                    map[treasure.row][treasure.col].direction = direction;
                    map[treasure.row][treasure.col].value = '$';
        
                    land_locations++;
                    went_ashore++;
                    return;
                }
                else continue;
            }
        }
    }
        
    if (verbose) std::cout << "Treasure hunt failed\n";
}

void Treasure_Hunt::first_mate_search(Coordinate c) { //resturcture
    
    went_ashore++;

    std::deque<Coordinate> f_container;

    if (first_mate_container_type == 'S') f_container.push_back(c);

    else f_container.push_front(c);

    if(verbose) std::cout << "Went ashore at: " << c.row << "," << c.col << "\n"
                          << "Searching island... "; 

    while(!f_container.empty()){

        Coordinate curr = f_container.back();
        c = curr;
        land_locations++;
        f_container.pop_back();

        for (auto direction : hunt_order) {
            if (is_valid_index(direction, curr.row, curr.col)) {
                Coordinate next = c;
                move(next, direction);
                if (map[next.row][next.col].value == 'o' && map[next.row][next.col].direction == ' ') {
                    map[next.row][next.col].direction = direction;
                    if (first_mate_container_type == 'S') f_container.push_back(next);
                    else f_container.push_front(next);
                }
                else if (map[next.row][next.col].value == '$') {
                    map[treasure.row][treasure.col].value = '$';
                    map[treasure.row][treasure.col].direction = direction;
                    
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

void Treasure_Hunt::print_stats(){
    
    std::cout << "--- STATS ---\n";

    std::cout << "Starting location: " << start.row << "," << start.col << "\n";

    std::cout << "Water locations investigated: " << water_locations << "\n";

    std::cout << "Land locations investigated: " << land_locations << "\n";

    std::cout << "Went ashore: " << went_ashore << "\n";

    if (map[treasure.row][treasure.col].direction != ' ') {

        std::cout << "Path length: " << path_length << "\n";

        std::cout << "Treasure location: " << treasure.row << "," << treasure.col << "\n";
    }

    std::cout << "--- STATS ---\n";
}

void Treasure_Hunt::hunt(){

    captain_search();

    if (map[treasure.row][treasure.col].direction == ' ') {
        if(stats) print_stats();
        std::cout << "No treasure found after investigating " << water_locations + land_locations << " locations.";
        return;
    }

    if (show_path) print_path();
        
    else {
        backtrace();
        if (stats) print_stats();
    } 

    std::cout << "Treasure found at " << treasure.row << "," << treasure.col << " with path length " << path_length << ".\n"; 

}

void Treasure_Hunt::backtrace() {

    Coordinate c = treasure; //starting from the treasure

    while(map[c.row][c.col].value != '@') { //while we've yet to reach the starting point
        switch(map[c.row][c.col].direction) {
            case 'N': //If the direction at the current coordinate map[row][col].direction --> the previous thing in the path went north to get to the current coordinate
                 //move coordinate south 
                c.row++;
                break;
            case 'E': //If the prev coordinate came to curr from the east --> curr must go west
                c.col--;
                break;
            case 'S':
                c.row--;
                break;
            case 'W': //If the prev coordinate came to curr from the west --> curr must go east
                c.col++;
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

void Treasure_Hunt::print_path() {

    //point.row = coordinate.row
    //point.col = coordinate.col


    std::deque<Coordinate> sail_path;  //DONT NEED TO STORE POINT. COORDINATE INSTEAD
    std::deque<Coordinate> search_path;


    Coordinate c = treasure;
    //curr = map[c.row][c.col]
    //starting from the treasure

    if(show_path) {
        if(path_type == 'M') map[c.row][c.col].value = 'X'; //set the treasure's value = 'X'
        else search_path.push_back(c); //add it to search container
    }

    while(map[c.row][c.col].value != '@') { //while we've yet to reach the starting point
        switch(map[c.row][c.col].direction) {
            case 'N': //If the direction at the current coordinate map[row][col].direction --> the previous thing in the path went north to get to the current coordinate
               //first move the coordinate south
               //check the new coordinates direction and change accordingly
               c.row++;
               if(show_path) { 
                if (path_type == 'M') {
                    if (map[c.row][c.col].value != '@') { //check that the prev is not the start (south means we add 1 to row)
                        if (map[c.row][c.col].direction == 'E' || map[c.row][c.col].direction == 'W') map[c.row][c.col].value = '+';
                        else map[c.row][c.col].value = '|';
                    }
               } else //path_type == 'L'
                    if (map[c.row][c.col].value == 'o') search_path.push_back(c); //if we go south and find land 
                    else if(map[c.row][c.col].value == '.') sail_path.push_back(c);
               }
                break;
            case 'E': //If the prev coordinate came to curr from the east --> curr must go west
                c.col--; //move coordinate east
                if(show_path) {
                    if (path_type == 'M') {
                        if (map[c.row][c.col].value != '@') { //if the prev coordinate's val isn't the start
                            if (map[c.row][c.col].direction == 'N' || map[c.row][c.col].direction == 'S') map[c.row][c.col].value = '+';
                            else map[c.row][c.col].value = '-';
                        }
                    } else //path_type == 'L'
                        if (map[c.row][c.col].value == 'o') search_path.push_back(c); //add the prev coordinate
                        else if (map[c.row][c.col].value == '.') sail_path.push_back(c);
                }
                break;
            case 'S':
                c.row--; //move north
                if(show_path) { //If the prev coordinate came to curr from the South --> curr must go North
                    if (path_type == 'M') {
                        if (map[c.row][c.col].value != '@') { //check prev value isnt start
                            if (map[c.row][c.col].direction == 'E' 
                                || map[c.row][c.col].direction == 'W') map[c.row][c.col].value = '+';
                                else map[c.row][c.col].value = '|';
                        } 
                    } else
                        if (map[c.row][c.col].value == 'o') search_path.push_back(c);
                        else if (map[c.row][c.col].value == '.') sail_path.push_back(c);
                }
                break;
            case 'W': //If the prev coordinate came to curr from the west --> curr must go east
                c.col++;//move east
                if(show_path) {
                    if(path_type == 'M') {
                        if (map[c.row][c.col].value != '@') {
                            if (map[c.row][c.col].direction == 'N' || map[c.row][c.col].direction == 'S') map[c.row][c.col].value = '+';
                            else map[c.row][c.col].value = '-';
                        }
                    } else 
                     if (map[c.row][c.col].value == 'o') search_path.push_back(c);
                     else if (map[c.row][c.col].value == '.') sail_path.push_back(c);
                }
                break;
        }
        path_length++; 
    }

    if(show_path && path_type == 'L') sail_path.push_back(start);

    if(stats) print_stats();

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
        Coordinate c;

        while (!sail_path.empty()) {
            c = sail_path.back();
            std::cout << c.row << "," << c.col << "\n";
            sail_path.pop_back();
        }

        std::cout << "Search:\n";
        c = search_path.back();

        while (!search_path.empty()) {
            c = search_path.back();
            std::cout << c.row << "," << c.col << "\n";
            search_path.pop_back();
        }

    }
}