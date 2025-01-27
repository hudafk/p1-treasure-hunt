// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#ifndef TREASURE_HUNT_H
#define TREASURE_HUNT_H

#include <iostream>
#include <vector>
#include <vector>
#include <deque>

struct Coordinate {
    size_t row;
    size_t col;
};
// make a pair


struct Point {
    char direction;
    char value;
    Point() : direction(' '), value(' ') {}
    Point(char d, char v) : direction(d), value(v) {}
};


class Treasure_Hunt {
    public:
        Treasure_Hunt() {}

        Treasure_Hunt(std::string hunt_order_in, bool v, bool s, bool p, char pt, char c, char f) : 
                hunt_order (hunt_order_in), verbose (v), stats (s), show_path (p), path_type(pt),
                captain_container_type (c), first_mate_container_type(f) {}


        void create_map(); //reads the input file and creates the grid
        void hunt();
        void captain_search();
        void first_mate_search(Coordinate c); 
        void print_results();
        void print_grid();
        void print_stats();
        void print_path();
        void backtrace();
        void backtrace_path();
    
    private:
        std::vector<std::vector<Point>> map; //pass by reference in functions
        Coordinate start;
        Coordinate treasure;
        bool is_valid_index(char c, size_t row, size_t col);
        void move(Coordinate &c, char direction);
        std::string hunt_order = "NESW";
        bool verbose = false;
        bool stats = false; 
        bool show_path = false;
        int went_ashore = 0;
        char path_type;
        char captain_container_type = 'S';
        char first_mate_container_type = 'Q';
        int water_locations = 0;
        int land_locations = 0;
        int path_length = 0;
};


#endif 