#define BOOST_TEST_MODULE setofTests
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "TestSetOf.h"

#include "asn1cpp/Seq.hpp"
#include "asn1cpp/Getter.hpp"
#include "asn1cpp/Setter.hpp"
#include "asn1cpp/Encoding.hpp"
#include "asn1cpp/SetOf.hpp"

BOOST_AUTO_TEST_CASE( construction ) {
    auto test = asn1cpp::makeSeq(TestSetOf);
}

BOOST_AUTO_TEST_CASE( add_integer ) {
    constexpr int value0 = 5;
    constexpr int value1 = 54;
    constexpr int value2 = 190;

    auto test = asn1cpp::makeSeq(TestSetOf);

    asn1cpp::setof::pushList(test->integer, value0);
    asn1cpp::setof::pushList(test->integer, value1);
    asn1cpp::setof::pushList(test->integer, value2);

    BOOST_CHECK_EQUAL(asn1cpp::setof::size(test->integer), 3);

    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->integer, int, 0), value0);
    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->integer, int, 1), value1);
    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->integer, int, 2), value2);
}
