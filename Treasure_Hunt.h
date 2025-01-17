#ifndef TREASURE_HUNT_H
#define TREASURE_HUNT_H

#include <iostream>
#include <vector>
#include <vector>
#include <deque>



class Treasure_Hunt {
    public:
        void create_grid(); //reads the input file and creates the grid
    
    private:
        std::vector<std::vector<int>> grid;
        std::deque<int> trace;

         
        
};


#endif 