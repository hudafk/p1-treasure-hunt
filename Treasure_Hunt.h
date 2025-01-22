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

        Treasure_Hunt(std::string hunt_order_in) : hunt_order (hunt_order_in) {}

        Treasure_Hunt(std::string hunt_order_in, bool v, bool s, bool p, char c, char f) : 
                hunt_order (hunt_order_in), verbose (v), stats (s), show_path (p), 
                captain_container_type (c), first_mate_container_type(f) {}

        std::string hunt_order = "NESW";
        bool verbose = false;
        bool stats = false; 
        bool show_path = false;
        int land = 0;
        char captain_container_type = 'S';
        char first_mate_container_type = 'Q';

        void create_map(); //reads the input file and creates the grid

        void hunt();

        void captain_search();

        void first_mate_search(Point &point); 

        void print_results();

        void print_grid();

        void print_verbose();
        void print_stats();
        void print_path();
    
    private:
        std::vector<std::vector<Point>> map; //pass by reference in functions

        std::deque<Point> captain_container;

        std::deque<Point> first_mate_container;

        Point start;

        Point treasure;

        
        bool is_valid_index(char c, size_t row, size_t col);
        void add_to_container(char c, Point p);
        Point move(Point p, char direction);
};


#endif 