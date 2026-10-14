#include "ip_filter.h"

#include <exception>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
    try {
        // std::ifstream file;
        std::istream* input = &std::cin;
        // if (argc > 1) {
        //     file.open(argv[1]);
        //     if (!file) {
        //         std::cerr << "Cannot open input file: " << argv[1] << '\n';
        //         return 1;
        //     }
        //     input = &file;
        // }

        auto ips = ipfilter::read_ips(*input);
        ipfilter::sort_descending(ips);

        ipfilter::ip_out(ips, std::cout);
        ipfilter::ip_out(ipfilter::filter_first(ips, 1), std::cout);
        ipfilter::ip_out(ipfilter::filter_first_two(ips, 46, 70), std::cout);
        ipfilter::ip_out(ipfilter::filter_any(ips, 46), std::cout);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
