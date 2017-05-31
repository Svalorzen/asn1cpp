#define BOOST_TEST_MODULE setofTests
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <array>

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
    constexpr std::array<int, 3> values = {{5, 54, 190}};

    auto test = asn1cpp::makeSeq(TestSetOf);

    BOOST_CHECK(asn1cpp::setof::pushList(test->integer, values[0]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->integer, values[1]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->integer, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::setof::getSize(test->integer), 3);

    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->integer, int, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->integer, int, 1), values[1]);
    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->integer, int, 2), values[2]);
}

BOOST_AUTO_TEST_CASE( remove_integer ) {
    constexpr std::array<int, 3> values = {{12, 23, 34}};

    auto test = asn1cpp::makeSeq(TestSetOf);

    BOOST_CHECK(asn1cpp::setof::pushList(test->integer, values[0]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->integer, values[1]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->integer, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::setof::getSize(test->integer), 3);

    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->integer, int, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->integer, int, 1), values[1]);
    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->integer, int, 2), values[2]);

    BOOST_CHECK(asn1cpp::setof::popList(test->integer, INTEGER, 1));

    BOOST_CHECK_EQUAL(asn1cpp::setof::getSize(test->integer), 2);

    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->integer, int, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->integer, int, 1), values[2]);
}
