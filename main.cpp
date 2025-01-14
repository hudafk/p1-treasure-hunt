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
        {"captain", required_argument, nullptr, 'c'},
        {"first-mate", required_argument, nullptr, 'f'},
        {"hunt-order", required_argument, nullptr, 'o'},
        {"verbose", no_argument, nullptr, 'v'},
        {"show_path", required_argument, nullptr, 'p'}
    }
}


int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);

    Options options;;
    getOptions(argc, argv, options);
}