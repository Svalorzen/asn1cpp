#define BOOST_TEST_MODULE intTests
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "TestInteger.h"

#include "asn1cpp/Seq.hpp"
#include "asn1cpp/Setter.hpp"

BOOST_AUTO_TEST_CASE( construction ) {
    auto test = asn1cpp::makeSeq(TestInteger);
}

BOOST_AUTO_TEST_CASE( assignment ) {
    constexpr auto value = 5;

    auto test = asn1cpp::makeSeq(TestInteger);

    asn1cpp::set(test->integer, value);

}
