// PROJECT IDENTIFIER: 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <getopt.h>
using namespace std;

struct Options {
    int help = 0;
    int captain = 1;
    int first_mate = 2;
    int hunt_order = 3;
    int verbose = 4;
    int show_path = 5;
};

void getOptions(int argc, char **argv, Options &options) {
    opterr = static_cast<int>(false);
    int choice;
    int index = 0;

    option longOptions[] = {
        {"help", no_argument, nullptr, 'h'},
        {"captain", optional_argument, nullptr, 'c'},
        {"first-mate", optional_argument, nullptr, 'f'},
        {"hunt-order", optional_argument, nullptr, 'o'},
        {"verbose", no_argument, nullptr, 'v'},
        {"show_path", no_argument, nullptr, 'p'},
        {"stats", no_argument, nullptr, 's'}
    };
}


int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);

    Options options;;
    getOptions(argc, argv, options);

    
}

