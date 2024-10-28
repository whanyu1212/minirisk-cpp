#include <iostream>
#include <iomanip>
#include <cstdint>
#include "Streamer.h"

using namespace std;

//void out_char_as_hex(int c)
//{
//    cout << hex << setw(2) << setfill('0') << c;
//}

int main()
{
    union
    {
        double d;
        uint64_t u;
    } tmp;
    double x = -0.15625;
    // Assume it is a wrapper of std::ofstream
    // The file will auto be created if it does not exist
    minirisk::my_ofstream output_file("test.txt");

    output_file << x;
    output_file.endl();
    output_file.close();

    // Assume it is a wrapper of std::ifstream
    minirisk::my_ifstream input_file("test.txt");
    double y;
    input_file.read_line();
    input_file >> y;
    cout << y << endl; // y should be reinterpreted as a double

    return 0;
}
