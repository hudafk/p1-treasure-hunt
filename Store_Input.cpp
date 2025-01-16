#include "store_input.h"
#include <iostream>

void Read::read() {
    std:: string line;

    while(std::getline(std::cin, line)) {
        if (line[0] != '#') break;
    }

    if (line.empty() || (line[0] != 'M' && line[0] != 'L')) throw std::runtime_error("the file isn't in correct format!");

    format = line[0]; //take in 'M' or 'L' for format

    if (!(std::cin >> map_size) || map_size < 2) throw std::runtime_error("map is too small!"); 

    std::cin.ignore(); //ignore newline char

    if (format == 'M') read_map();
    else read_list();
    
}

void Read::read_map() {
    //
}