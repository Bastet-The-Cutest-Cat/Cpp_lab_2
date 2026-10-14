#include "ip_filter.h"

#include <algorithm>
#include <array>
#include <ranges>
#include <sstream>

namespace ipfilter {

bool parse_ip(const std::string& text, IPAddress& result)
{
    std::istringstream stream(text);
    std::array<int, 4> bytes = {{0, 0, 0, 0}};
    char dot = '\0';

    for (std::size_t i = 0; i < bytes.size(); ++i) {
        if (!(stream >> bytes[i])) {
            return false;
        }
        if (bytes[i] < 0 || bytes[i] > 255) {
            return false;
        }
        if (i != bytes.size() - 1) {
            if (!(stream >> dot) || dot != '.') {
                return false;
            }
        }
    }

    stream >> std::ws;
    if (!stream.eof()) {
        return false;
    }

    result = std::make_tuple(
        static_cast<std::uint8_t>(bytes[0]),
        static_cast<std::uint8_t>(bytes[1]),
        static_cast<std::uint8_t>(bytes[2]),
        static_cast<std::uint8_t>(bytes[3]));
    return true;
}

std::vector<IPAddress> read_ips(std::istream& input)
{
    std::vector<IPAddress> ips;
    std::string line;

    while (std::getline(input, line)) {
        const std::string::size_type tab = line.find('\t');
        const std::string address = line.substr(0, tab);
        IPAddress ip;
        if (parse_ip(address, ip)) {
            ips.push_back(ip);
        }
    }
    return ips;
}

void sort_descending(std::vector<IPAddress>& ips)
{
    std::sort(ips.begin(), ips.end(),
              [](const auto& lhs, const auto& rhs) {
                  return lhs > rhs;
              });
}

std::string to_string(const IPAddress& ip)
{
    std::ostringstream out;
    out << static_cast<int>(std::get<0>(ip)) << '.'
        << static_cast<int>(std::get<1>(ip)) << '.'
        << static_cast<int>(std::get<2>(ip)) << '.'
        << static_cast<int>(std::get<3>(ip));
    return out.str();
}

void ip_out(const std::vector<IPAddress>& list, std::ostream& output)
{
    for (const auto& ip : list) {
        output << to_string(ip) << '\n';
    }
}

void print_ips(const std::vector<IPAddress>& ips, std::ostream& output)
{
    ip_out(ips, output);
}

std::vector<IPAddress> filter_first(const std::vector<IPAddress>& ips, int first)
{
    const auto matches = [first](const auto& ip) {
        return static_cast<int>(std::get<0>(ip)) == first;
    };
    std::vector<IPAddress> result;
    for (const auto& ip : ips | std::views::filter(matches)) {
        result.push_back(ip);
    }
    return result;
}

std::vector<IPAddress> filter_first_two(const std::vector<IPAddress>& ips,
                                        int first, int second)
{
    const auto matches = [first, second](const auto& ip) {
        return static_cast<int>(std::get<0>(ip)) == first &&
               static_cast<int>(std::get<1>(ip)) == second;
    };
    std::vector<IPAddress> result;
    for (const auto& ip : ips | std::views::filter(matches)) {
        result.push_back(ip);
    }
    return result;
}

std::vector<IPAddress> filter_any(const std::vector<IPAddress>& ips, int byte)
{
    const auto has_byte = [byte](const auto& ip) {
        return static_cast<int>(std::get<0>(ip)) == byte ||
               static_cast<int>(std::get<1>(ip)) == byte ||
               static_cast<int>(std::get<2>(ip)) == byte ||
               static_cast<int>(std::get<3>(ip)) == byte;
    };
    std::vector<IPAddress> result;
    for (const auto& ip : ips | std::views::filter(has_byte)) {
        result.push_back(ip);
    }
    return result;
}

}
