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
};


class Treasure_Hunt {
    public:
        Treasure_Hunt() {}

        Treasure_Hunt(std::string hunt_order_in) : hunt_order (hunt_order_in) {}

        void create_map(); //reads the input file and creates the grid

        void hunt();

        void captain_search();

        void first_mate_search(Point point); 

        void print_results();

        void print_grid();

        void print_verbose();
        void print_stats();
        void print_path();
    
    private:
        std::vector<std::vector<Point>> map; //pass by reference in functions
        std::deque<Point> captain_container;
        std::deque<Point> first_mate_container;
        Point start = {'X','@',0,0, true};
        std::pair<size_t,size_t> treasure;
        bool verbose = false;
        bool stats = false; 
        bool show_path = false;
        int land = 0;
        std::string hunt_order = "NESW";
        bool is_valid_index(char c, size_t row, size_t col);
        void add_point();
};


#endif 