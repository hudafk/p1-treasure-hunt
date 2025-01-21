// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#ifndef TREASURE_HUNT_H
#define TREASURE_HUNT_H

#include <iostream>
#include <vector>
#include <vector>
#include <deque>



class Treasure_Hunt {
    public:
        Treasure_Hunt() {}
        
        Treasure_Hunt(std::string hunt_order_in) : hunt_order (hunt_order_in) {}

        void create_grid(); //reads the input file and creates the grid

        void hunt();

        void captain_search();

        void first_mate_search(std::pair<size_t, size_t> coordinate); 

        void print_results();
    
    private:
        std::vector<std::vector<int>> grid; //pass by reference in functions
        std::deque<std::pair<int,int>> captain_container;
        std::deque<std::pair<size_t,size_t>> first_mate_container;
        std::vector<std::pair<char,std::pair<size_t, size_t>>> path;
        std::pair<size_t,size_t> start;
        std::pair<size_t,size_t> treasure;
        bool verbose;
        bool stats; 
        std::string hunt_order = "NESW";
        bool is_valid_index(char c, std::pair<size_t, size_t> coordinate);
};


#endif 