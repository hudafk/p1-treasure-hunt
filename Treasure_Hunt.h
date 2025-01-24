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

struct Point {
    char direction;
    char value;
    size_t row;
    size_t col;
    bool discovered;

    Point() : direction(' '), value(' '), row(0), col(0), discovered(false) {}

    Point(char d, char v, size_t r, size_t c, bool b = false) : direction(d), value(v), row(r), col(c), discovered(b) {}

};

/*struct Options {
    Mode mode = Mode::NONE;
    string captainContainer = "STACK";
    string firstMateContainer = "QUEUE";
    string huntOrder; 
    bool verbose = false;
    bool showPath = false;
    bool stats = false;*/

class Treasure_Hunt {
    public:
        Treasure_Hunt() {}

        Treasure_Hunt(std::string hunt_order_in, bool v, bool s, bool p, char pt, char c, char f) : 
                hunt_order (hunt_order_in), verbose (v), stats (s), show_path (p), path_type(pt),
                captain_container_type (c), first_mate_container_type(f) {}

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

        void create_map(); //reads the input file and creates the grid

        void hunt();
        void captain_search(Coordinate c);
        void first_mate_search(Coordinate c); 
        void print_results();
        void print_grid();
        void print_stats();
        void print_path();
        void backtrace();
    
    private:
        std::vector<std::vector<Point>> map; //pass by reference in functions
        std::deque<Point> captain_container;
        std::deque<Point> first_mate_container;
        std::deque<Point> sail_path;
        std::deque<Point> search_path;
        Point start;
        Point treasure;
        bool is_valid_index(char c, size_t row, size_t col);
        void add_to_container(char c, Point p);
        void move(Coordinate &c, char direction);
};


#endif 