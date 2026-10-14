#pragma once

#include <cstdint>
#include <istream>
#include <ostream>
#include <string>
#include <tuple>
#include <vector>

namespace ipfilter {

// The tuple type is derived with decltype, while each octet uses exactly 8 bits.
using IPAddress = decltype(std::make_tuple(
    std::uint8_t{}, std::uint8_t{}, std::uint8_t{}, std::uint8_t{}));
using IpAddress = IPAddress;

bool parse_ip(const std::string& text, IPAddress& result);
std::vector<IPAddress> read_ips(std::istream& input);
void sort_descending(std::vector<IPAddress>& ips);
void ip_out(const std::vector<IPAddress>& list, std::ostream& output);
void print_ips(const std::vector<IPAddress>& ips, std::ostream& output);
std::vector<IPAddress> filter_first(const std::vector<IPAddress>& ips, int first);
std::vector<IPAddress> filter_first_two(const std::vector<IPAddress>& ips,
                                        int first, int second);
std::vector<IPAddress> filter_any(const std::vector<IPAddress>& ips, int byte);
std::string to_string(const IPAddress& ip);

} // namespace ipfilter
