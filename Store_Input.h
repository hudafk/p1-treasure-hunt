#ifndef STORE_INPUT_H
#define STORE_INPUT_H

#include <iostream>

class Read {
    public:
        void read();

    private:
        char format; //File format specifier ('M' for Map and 'L' for List)
        int map_size; //Size of the map (N x N)
        void read_map();
        void read_list();

};


#endif 