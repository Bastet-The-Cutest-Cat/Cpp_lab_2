#define BOOST_TEST_MODULE IpFilterTests
#include <boost/test/included/unit_test.hpp>

#include "ip_filter.h"

#include <sstream>
#include <vector>

using ipfilter::IpAddress;

static IpAddress ip(int a, int b, int c, int d)
{
    return std::make_tuple(static_cast<std::uint8_t>(a),
                           static_cast<std::uint8_t>(b),
                           static_cast<std::uint8_t>(c),
                           static_cast<std::uint8_t>(d));
}

BOOST_AUTO_TEST_CASE(parse_valid_and_invalid_addresses)
{
    IpAddress parsed;
    BOOST_CHECK(ipfilter::parse_ip("192.168.1.10", parsed));
    BOOST_CHECK_EQUAL(ipfilter::to_string(parsed), "192.168.1.10");
    BOOST_CHECK(!ipfilter::parse_ip("256.1.1.1", parsed));
    BOOST_CHECK(!ipfilter::parse_ip("-1.1.1.1", parsed));
    BOOST_CHECK(!ipfilter::parse_ip("1111.1.1.1", parsed));
    BOOST_CHECK(!ipfilter::parse_ip("1.2.3", parsed));
    BOOST_CHECK(!ipfilter::parse_ip("1.2.3.4x", parsed));
}

BOOST_AUTO_TEST_CASE(sort_is_numeric_and_descending)
{
    std::vector<IpAddress> ips;
    ips.push_back(ip(1, 2, 1, 1));
    ips.push_back(ip(1, 10, 1, 1));
    ips.push_back(ip(1, 1, 1, 1));

    ipfilter::sort_descending(ips);

    BOOST_CHECK_EQUAL(ipfilter::to_string(ips[0]), "1.10.1.1");
    BOOST_CHECK_EQUAL(ipfilter::to_string(ips[1]), "1.2.1.1");
    BOOST_CHECK_EQUAL(ipfilter::to_string(ips[2]), "1.1.1.1");
}

BOOST_AUTO_TEST_CASE(filters_keep_only_matching_addresses)
{
    std::vector<IpAddress> ips;
    ips.push_back(ip(46, 70, 225, 39));
    ips.push_back(ip(1, 231, 69, 33));
    ips.push_back(ip(186, 204, 34, 46));
    ips.push_back(ip(46, 1, 2, 3));

    BOOST_CHECK_EQUAL(ipfilter::filter_first(ips, 1).size(), 1u);
    BOOST_CHECK_EQUAL(ipfilter::filter_first_two(ips, 46, 70).size(), 1u);
    BOOST_CHECK_EQUAL(ipfilter::filter_any(ips, 46).size(), 3u);
}

BOOST_AUTO_TEST_CASE(read_ignores_fields_after_first_tab)
{
    std::istringstream input("1.2.3.4\t100\t0\n46.70.1.2\t3\t4\n");
    const auto ips = ipfilter::read_ips(input);
    BOOST_REQUIRE_EQUAL(ips.size(), 2u);
    BOOST_CHECK_EQUAL(ipfilter::to_string(ips[0]), "1.2.3.4");
    BOOST_CHECK_EQUAL(ipfilter::to_string(ips[1]), "46.70.1.2");
}
