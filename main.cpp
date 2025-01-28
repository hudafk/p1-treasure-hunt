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
                exit(0);
            }
            case 'c': {
                if(optarg) {
                    if(string(optarg) != "STACK" && string(optarg) != "QUEUE"){
                        cerr << "Invalid argument to --captain" << endl;
                        exit(1);
                    }
                    options.captainContainer = optarg[0];
                } else {
                    cerr << "Invalid argument to --captain" << endl;
                    exit(1);
                } 
                //set captain's container type
                break;
            }
            case 'f': {
                if(optarg) {
                    if(string(optarg) != "STACK" && string(optarg) != "QUEUE"){
                        cerr << "Invalid argument to --first-mate" << endl;
                        exit(1);
                    }
                    options.firstMateContainer = optarg[0];
                } else {
                    cerr << "Invalid argument to --first-mate" << endl;
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
                    for (char c : str) {
                        if(valid.find(c) == string::npos) {
                            cerr << "Invalid argument to --hunt-order" << endl;
                            exit(1);
                        }
                    }
                    if(unique(str.begin(), str.end()) != str.end())  {
                        cerr << "Invalid argument to --hunt-order" << endl;
                        exit(1);
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
                    if(string(optarg) == "M" || string(optarg) == "L") {
                        options.pathType = optarg[0];
                    } 
                    else {
                        cerr << "Invalid argument to --show-path" << endl;
                        exit(1);
                    }
                }
                else {
                    cerr << "Invalid argument to --show-path" << endl;
                    exit(1);
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


