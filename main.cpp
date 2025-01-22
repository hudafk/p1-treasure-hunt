// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "Treasure_Hunt.h"

#include <getopt.h>
using namespace std;


enum class Mode {
    NONE = 0,
    STATS,
    VERBOSE,
    SHOW_PATH
};

struct Options {
    Mode mode = Mode::NONE;
    char captainContainer = 'S';
    char firstMateContainer = 'Q';
    string huntOrder; 
    bool verbose = false;
    bool showPath = false;
    bool stats = false;
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
        {"show_path", no_argument, nullptr, 'p'},
        {"stats", no_argument, nullptr, 's'},
        {nullptr, 0, nullptr, '\0'}
    };

    while((choice = getopt_long(argc, argv,"hc:f:o:vps", static_cast<option *>(longOptions), &index)) != -1) {
        switch (choice) {
            case 'h':
                printHelp(*argv); //print help statements
                exit(1);
            case 'c':
                options.captainContainer = optarg[0]; //set captain's container type
                break;
            case 'f':
                options.firstMateContainer = optarg[0]; //set fm's container type
                break;
            case 'o':
                options.huntOrder = optarg; //hunt order specified
                break;
            case 'v':
                options.verbose = true; //print verbose
                cout << "verbose" << endl;
                break;
            case 'p':
                options.showPath = true; //show path
                break;
            case 's':
                options.stats = true; //show stats
                break;
            default:
                cerr << "Invalid option!" << endl;
                exit(1);
        }
    }
}


int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);


    Options options;
    getOptions(argc, argv, options);

    /*struct Options {
    Mode mode = Mode::NONE;
    string captainContainer = "STACK";
    string firstMateContainer = "QUEUE";
    string huntOrder; 
    bool verbose = false;
    bool showPath = false;
    bool stats = false;*/
    /*bool verbose = false;
        bool stats = false; 
        bool show_path = false;
        int land = 0;
        char captain_container_type = 's';
        char first_mate_container_type = 'q';
        std::string hunt_order = "NESW";*/
    cout << options.huntOrder << endl << "hmm" << endl;
    Treasure_Hunt hunt(options.huntOrder, 
                       options.verbose, options.stats, options.showPath,
                       static_cast<char>(options.captainContainer), 
                       static_cast<char>(options.firstMateContainer));

    hunt.create_map();

    hunt.print_grid();

    hunt.captain_search();
}

