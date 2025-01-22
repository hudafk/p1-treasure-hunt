// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#ifndef TREASURE_HUNT_H
#define TREASURE_HUNT_H

#include <iostream>
#include <vector>
#include <vector>
#include <deque>


struct Point {
    char direction;
    char value;
    size_t row;
    size_t col;
    bool discovered;
    size_t prev_row;
    size_t prev_col;
};


class Treasure_Hunt {
    public:
        Treasure_Hunt() {}

        Treasure_Hunt(std::string hunt_order_in) : hunt_order (hunt_order_in) {}

        void create_map(); //reads the input file and creates the grid

        void hunt();

        void captain_search();

        void first_mate_search(std::pair<size_t, size_t> coordinate); 

        void print_results();

        void print_grid();
    
    private:
        std::vector<std::vector<char>> map; //pass by reference in functions
        std::deque<Point> captain_container;
        std::deque<std::pair<size_t,size_t>> first_mate_container;
        Point start = {'@',0,0};
        std::pair<size_t,size_t> treasure;
        bool verbose;
        bool stats; 
        bool show_path;
        void verbose();
        void stats();
        void show_path();
        std::string hunt_order = "NESW";
        bool is_valid_index(char c, std::pair<size_t, size_t> coordinate);
};


#endif 