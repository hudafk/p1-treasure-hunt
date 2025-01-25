// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "Treasure_Hunt.h"

#include <getopt.h>
using namespace std;


struct Options {
    char captainContainer = 'S';
    char firstMateContainer = 'Q';
    string huntOrder = "NESW"; 
    bool verbose = false;
    bool showPath = false;
    bool stats = false;
    char pathType;
};

void printHelp(char *command) {
    cout << "Usage: " << command << " under construction~" << endl;
}

void getOptions(int argc, char **argv, Options &options) {
    opterr = static_cast<int>(false);
    int choice;
    int index = 0;

    option longOptions[] = {
        {"help", no_argument, nullptr, 'h'},
        {"captain", required_argument, nullptr, 'c'},
        {"first-mate", required_argument, nullptr, 'f'},
        {"hunt-order", required_argument, nullptr, 'o'},
        {"verbose", no_argument, nullptr, 'v'},
        {"show-path", required_argument, nullptr, 'p'},
        {"stats", no_argument, nullptr, 's'},
        {nullptr, 0, nullptr, '\0'}
    };

    while((choice = getopt_long(argc, argv,"hc:f:o:vp:s", static_cast<option *>(longOptions), &index)) != -1) {
        switch (choice) {
            case 'h': {
                printHelp(*argv); //print help statements
                exit(1);
            }
            case 'c': {
                if (optarg && (string(optarg) == "STACK" || string(optarg) == "QUEUE")) {
                        options.captainContainer = optarg[0];
                    } 
                else {
                    cerr << "Invalid argument to --captain" << endl;
                    exit(1);
                } //set captain's container type
                break;
            }
            case 'f': {
                if (optarg && (string(optarg) == "STACK" || string(optarg) == "QUEUE")) {
                    options.firstMateContainer = optarg[0];
                } else {
                    cerr << "Invalid argument to --captain" << endl;
                    exit(1);
                }
                break;
            }
            case 'o': {
                if (optarg) {
                    string str = optarg;
                    if(str.length() != 4) {
                        cerr << "Invalid argument to --hunt-order" << endl;
                        exit(1);
                    } 
                    string valid = "NESW";
                    for (char c : valid) {
                        if(count(str.begin(), str.end(), c != 1)) {
                            cerr << "Invalid argument to --hunt-order" << endl;
                            exit(1);
                        }
                    }
                    options.huntOrder = str; //hunt order specified
                }
                break;
            }
            case 'v': {
                options.verbose = true; //print verbose
                break;
            }
            case 'p': {
                if(options.showPath) {
                    cerr << "Specify --show-path only once" << endl;
                    exit(1);
                } 
                options.showPath = true; //show path
                if (optarg) {
                    if(optarg[0] == 'M' || optarg[0] == 'L') {
                        options.pathType = optarg[0];
                    } 
                    else {
                        cerr << "Invalid argument to --show-path" << endl;
                        exit(1);
                    }
                }
                break;
            }
            case 's': {
                options.stats = true; //show stats
                break;
            }
            default: {
                cerr << "Unknown option" << endl;
                exit(1);
            }
        }
    }
}

tuple<vector<vector<Point>>, Point, Point>  create_map() {
        string line;

        Point start;
        Point treasure;

        while(getline(cin, line)) {
            if (line[0] != '#') break;
        }

        if (line.empty() || (line[0] != 'M' && line[0] != 'L')) throw runtime_error("the file isn't in correct format!");

        char format = line[0]; //take in 'M' or 'L' for format

        size_t map_size; //the size of the map


        if (!(std::cin >> map_size) || map_size < 2) throw runtime_error("map is too small!"); 

        vector<vector<Point>> map(map_size, vector<Point>(map_size));

        std::cin.ignore(); //ignore newline char

        if (format == 'M') {
            //read the map format
            string line;
            size_t row = 0;

            while(getline(cin, line)) {
                if (line.empty()) continue;

                for (size_t col = 0; col < map_size; ++col) {
                    Point point('X', line[col], row, col, false);

                    if (line[col] == '@') {
                        start.row = row;
                        start.col = col;
                        start.value = line[col];
                    } else if (line[col] == '$') {
                        start.row = row;
                        start.col = col;
                        start.value = line[col];
                    } else{
                        map[row][col] = point;
                    }
                }

                row++;
            }
        } 
        else {
            //read the list format
            for(size_t row = 0; row < map.size(); ++row){
                for(size_t col = 0; col < map.size(); ++col) {
                    map[row][col].value = '.';
                    map[row][col].row = row;
                    map[row][col].col = col;
                }
            }

            string line;

            while(getline(cin, line)) {
                if (line.empty()) continue;

                size_t row = static_cast<size_t>(line[0] - '0');
                size_t col = static_cast<size_t>(line[2] - '0');
                char terrain = line[4];

                if (terrain == '@') {
                    start.row = row;
                    start.col = col;
                    start.value = terrain;
                } else if (terrain == '$') {
                        treasure.row = row;
                        treasure.col = col;
                        treasure.value = terrain;
                } else{
                    map[row][col].value = terrain;
                    map[row][col].row = row;
                    map[row][col].col = col; 
                } 
            }
        } 
    
    return {map, start, treasure};
}


int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);

    Options options;
    getOptions(argc, argv, options);


    Treasure_Hunt hunt(options.huntOrder, 
                       options.verbose, options.stats, 
                       options.showPath, options.pathType,
                       static_cast<char>(options.captainContainer), 
                       static_cast<char>(options.firstMateContainer));
    
    hunt.create_map();
    hunt.hunt();

}


