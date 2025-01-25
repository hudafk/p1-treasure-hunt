// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
#include "Treasure_Hunt.h"
#include <iostream>
#include <sstream>

void Treasure_Hunt::create_map() { //Creates the map

        bool is_start = false; //saying that we've yet to find the start point
        bool is_treasure = false; //saying that we've yet to find the treasure point

        std:: string line;

        while(std::getline(std::cin, line)) { //read in comments
            if (line[0] != '#') break;
        } //stops when we reach map format

        if (line.empty() || (line[0] != 'M' && line[0] != 'L')) { //check if it's an invalid map format
            std::cerr << ("Unknown option") << std::endl;
            exit(1);
        }

        char format = line[0]; //take in 'M' or 'L' for format
        size_t map_size; //the size of the map

        if (!(std::cin >> map_size) || map_size < 2) {
            std::cerr << ("Map is too small!") << std::endl; 
            exit(1);
        }

        map.resize(map_size, std::vector<Point>(map_size)); //set vec size to the size we read in

        //std::cin.ignore(); //ignore newline char

        if (format == 'M') { //read the map format

            std::string line;

            size_t row = 0;

            while(std::getline(std::cin, line)) {

                if (line.empty()) continue;

                for (size_t col = 0; col < map_size; ++col) {
                    if(line[col] != '#' && line[col] != '.' && line[col] != 'o'  //check for invalid chars
                        && line[col] != '$' && line[col] != '@') {
                            std::cerr << "Invalid terrain type" << std::endl;
                            exit(1);
                    }

                    Point point(line[col]); //value is terrain, yet to be discovered or have direction

                    map[row][col] = point; //setting map at that idx to point

                    if (point.value == '@') { //checking for start so we can store its coordinate
                        is_start = true; //the map has a starting point
                        start.row = row;
                        start.col = col;

                    } else if (point.value == '$') { //checking for start so we can store its coordinate and mark it found
                        is_treasure = true; //the map has a treasure location
                        treasure.row = row;
                        treasure.col = col;
                    } 
                }
                row++;
            }
        } 

        else { //format = 'L' --> read list


            for(size_t row = 0; row < map.size(); ++row){ //set all locations equal to water
                for(size_t col = 0; col < map.size(); ++col) {
                    map[row][col].value = '.';
                }
            }

            std::string line;

            while(std::getline(std::cin, line)) {

                if (line.empty()) continue; //ignore blank lines
                
                std::istringstream line_stream(line);

                int r;
                int c;
                char terrain;

                line_stream >> r >> c >> terrain;

                if (r < 0 || static_cast<size_t>(r) >= map_size 
                    || c < 0 || static_cast<size_t>(c) >= map_size) { //user input has invalid coordinates
                        std::cerr << "Invalid coordinates in list mode input" << std::endl;
                        exit(1);
                }

                size_t row = static_cast<size_t>(r);
                size_t col = static_cast<size_t>(c);

                if (terrain == '@') {
                    is_start = true;
                    start.row = row;
                    start.col = col;

                    //TODO: set the value of the start and make it discovered in the map
                    map[start.row][start.col].discovered = true;
                    map[start.row][start.col].value = terrain;

                }

                else if (terrain == '$') {
                        is_treasure = true;
                        treasure.row = row;
                        treasure.col = col;

                        map[treasure.row][treasure.col].value = terrain;
                }

                else {
                    map[row][col].value = terrain;
                }
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

        map[start.row][start.col].discovered = true;
        add_to_container('c', map[start.row][start.col]); //adding start to captain container
}

bool Treasure_Hunt::is_valid_index(char c, size_t row, size_t col){ //makes sure we don't index out of bounds
    if (c == 'N') return (static_cast<int>(row) - 1 >= 0);

    if (c == 'E') return (col + 1) < map.size();

    if (c == 'S') return (row + 1) < map.size();

    if (c == 'W') return (static_cast<int>(col) - 1 >= 0);
    return false;
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

void Treasure_Hunt::captain_search(Coordinate c) {
    if(verbose) std::cout << "Treasure hunt started at: " << start.row << "," << start.col << "\n";
        
    while (!captain_container.empty()){
        Coordinate to_investigate = c; //this is the point at the current row and column
        captain_container.pop_back();
        water_locations++;

        for (auto direction : hunt_order) {
            c = to_investigate;
            
            if (is_valid_index(direction, c.row, c.col)) { //checking if coordinate can go in the specific direction
                move(c, direction); //if possible, move c, changing the coordinate row or col depending on direction

                if (map[c.row][c.col].value == '.' && !map[c.row][c.col].discovered) { //if the value at new coordinate is water
                    map[c.row][c.col].discovered = true; //set that point in map = true
                    map[c.row][c.col].direction = direction; //set the direction to be the direction the og point went to get to the new point
                    add_to_container('c', map[c.row][c.col]); //add this point to the container
                }

                else if (map[c.row][c.col].value == 'o' && !map[c.row][c.col].discovered) { // if the new value is land
                    map[c.row][c.col].discovered = true;  //mark discovered
                    map[c.row][c.col].direction = direction; //set the direction

                    first_mate_search(c); //send the first mate to search the land
                    if (map[treasure.row][treasure.col].discovered) { //checking to see if first mate found the treasure
                        land_locations++;
                        return;
                    }
                }
                    
                else if (map[c.row][c.col].value == '$'){ //if captain found the treasure
                    map[treasure.row][treasure.col].discovered = true; //mark treasure as discovered
                    map[treasure.row][treasure.col].direction = direction; //store the direction from which the previous value came;
                    map[treasure.row][treasure.col].value = '$'; //make sure to give the treasure its value //TODO; DELETE THIS LINE

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

    add_to_container('f', map[c.row][c.col]); //add to first mate's container

    while(!first_mate_container.empty()){

        //Point p = first_mate_container.back(); //retrieving point from container
        land_locations++;
        first_mate_container.pop_back();

        for (auto direction : hunt_order) {

            if (is_valid_index(direction, c.row, c.col)) { //check if c can go in that direction

                move(c, direction);

                if (map[c.row][c.col].value == 'o' && !map[c.row][c.col].discovered) { //check if the map point at the new coordinates is new land
                    map[c.row][c.col].discovered = true; //mark as discovered
                    map[c.row][c.col].direction = direction; //store direction of how the previous coordinate got here
                
                    add_to_container('f', map[c.row][c.col]); //add to search container
                }

                else if (map[c.row][c.col].value == '$') { //if the value at this new coordinate is $, then its the treasure! //treasure.row = c.row etc. 
                    map[treasure.row][treasure.col].discovered = true; //mark treasure as discovered
                    map[treasure.row][treasure.col].direction = direction; //store the direction from which prev coordinate came

                    if(verbose) std::cout << "party found treasure at " << c.row << "," << c.col << ".\n";
                    return;
                }

                else continue;
            }
        }
    }
    
    if(verbose) std::cout << "party returned with no treasure.\n";
    return;
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

void Treasure_Hunt::print_stats(){

    std::cout << "--- STATS ---\n";

    std::cout << "Starting location: " << start.row << "," << start.col << "\n";

    std::cout << "Water locations investigated: " << water_locations << "\n";

    std::cout << "Land locations investigated: " << land_locations << "\n";

    std::cout << "Went ashore: " << went_ashore << "\n";

    if (map[treasure.row][treasure.col].discovered) {
        std::cout << "Path length: " << path_length << "\n";
        std::cout << "Treasure location: " 
                  << treasure.row << "," << treasure.col << "\n";
    }

    std::cout << "--- STATS ---\n";
}

void Treasure_Hunt::hunt(){

    captain_search(start);

    if (map[treasure.row][treasure.col].discovered) {
        backtrace();
         
        if (stats) print_stats();

        if (show_path) print_path(); 

    }  else if (stats) print_stats();

    if (!map[treasure.row][treasure.col].discovered) std::cout << "No treasure found after investigating " << water_locations + land_locations << " locations.";
    else std::cout << "Treasure found at " << treasure.row << "," << treasure.col << " with path length " << path_length << ".\n"; 
}

void Treasure_Hunt::backtrace() {

    //Point p = map[treasure.row][treasure.col]; //set point = treasire
    Coordinate c (treasure); //set coordinate

    if(show_path) {
        if(path_type == 'M') map[treasure.row][treasure.col].value = 'X'; //reset treasure's value to X
        else search_path.push_back(treasure); //push to search path container
    }

    while(map[c.row][c.col].value != '@') { //while we're not at the starting coordinate
        switch(map[c.row][c.col].direction) { //the direction that the previous coordinate took to get to c
            case 'N': //prev went North to get here
               move(c, 'S'); //move c south

               if(show_path) {
                if (path_type == 'M') {
                    if (map[c.row][c.col].value != '@') {
                        if (map[c.row][c.col].direction == 'E' //look South and check if there was a change in direction
                        || map[c.row][c.col].direction == 'W') map[c.row][c.col].value = '+';
                        else map[c.row][c.col].value = '|';
                    }
               } else //path_type == 'L'
                    if (map[c.row][c.col].value == 'o') search_path.push_back(c); //push prev land coordinate
                    else sail_path.push_back(c); //push prev water coordinate
               }
                break;
            case 'E': //prev went East to get here
                move(c, 'W'); //move west to get to prev coordinate
                if(show_path) {
                    if (path_type == 'M') {
                        if (map[c.row][c.row].value != '@') { //check that we havent gotten to the start
                            if (map[c.row][c.col].direction == 'N' 
                                || map[c.row][c.col].direction == 'S') map[c.row][c.col].value = '+';
                            else map[c.row][c.col].value = '-';
                        }
                    } else //path_type == 'L'
                        if (map[c.row][c.col].value == 'o' && map[c.row][c.row].value != '@') search_path.push_back(c);
                        else sail_path.push_back(c);
                }
                break;
            case 'S': //prev went south to get here

                move(c, 'N'); //move north to get to prev coordinate

                if(show_path) {
                    if (path_type == 'M') {
                        if (map[c.row][c.col].value != '@') {
                            if (map[c.row][c.col].direction == 'E' 
                                || map[c.row][c.col].direction == 'W') map[c.row][c.col].value = '+';
                                else map[c.row][c.col].value = '|';
                        } else
                        if (map[c.row][c.col].value == 'o') search_path.push_back(c);
                        else sail_path.push_back(c);
                    }
                }
                break;
            case 'W': //prev went west to get here

                move(c, 'E'); //move east to get to prev coordinate

                if(show_path) {
                    if(path_type == 'M') {
                        if (map[c.row][c.col].value != '@') {
                            if (map[c.row][c.col].direction == 'N' 
                                || map[c.row][c.col + 1].direction == 'S') map[c.row][c.col].value = '+';
                                else map[c.row][c.col].value = '-';
                        }
                    } else 
                     if (map[c.row][c.col].value == 'o') search_path.push_back(c);
                     else sail_path.push_back(c);
                }
                break;
        }
        path_length++;
    }
}