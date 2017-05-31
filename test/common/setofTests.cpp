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

BOOST_AUTO_TEST_CASE( clear_integer ) {
    constexpr std::array<int, 3> values = {{12, 23, 34}};

    auto test = asn1cpp::makeSeq(TestSetOf);

    BOOST_CHECK(asn1cpp::setof::pushList(test->integer, values[0]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->integer, values[1]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->integer, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::setof::getSize(test->integer), 3);

    asn1cpp::setof::clrField(test->integer, INTEGER);

    BOOST_CHECK_EQUAL(asn1cpp::setof::getSize(test->integer), 0);
}

BOOST_AUTO_TEST_CASE( add_string ) {
    const std::array<std::string, 3> values = {{"alpha", "beta", "gamma"}};

    auto test = asn1cpp::makeSeq(TestSetOf);

    BOOST_CHECK(asn1cpp::setof::pushList(test->string, values[0]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->string, values[1]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->string, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::setof::getSize(test->string), 3);

    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->string, std::string, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->string, std::string, 1), values[1]);
    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->string, std::string, 2), values[2]);
}

BOOST_AUTO_TEST_CASE( remove_string ) {
    const std::array<std::string, 3> values = {{"veni", "vidi", "vici"}};

    auto test = asn1cpp::makeSeq(TestSetOf);

    BOOST_CHECK(asn1cpp::setof::pushList(test->string, values[0]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->string, values[1]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->string, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::setof::getSize(test->string), 3);

    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->string, std::string, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->string, std::string, 1), values[1]);
    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->string, std::string, 2), values[2]);

    BOOST_CHECK(asn1cpp::setof::popList(test->string, OCTET_STRING, 1));

    BOOST_CHECK_EQUAL(asn1cpp::setof::getSize(test->string), 2);

    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->string, std::string, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->string, std::string, 1), values[2]);
}

BOOST_AUTO_TEST_CASE( clear_string ) {
    const std::array<std::string, 3> values = {{"god", "is", "dead"}};

    auto test = asn1cpp::makeSeq(TestSetOf);

    BOOST_CHECK(asn1cpp::setof::pushList(test->string, values[0]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->string, values[1]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->string, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::setof::getSize(test->string), 3);

    asn1cpp::setof::clrField(test->string, OCTET_STRING);

    BOOST_CHECK_EQUAL(asn1cpp::setof::getSize(test->string), 0);
}

BOOST_AUTO_TEST_CASE( add_boolean ) {
    constexpr std::array<bool, 3> values = {{false, true, false}};

    auto test = asn1cpp::makeSeq(TestSetOf);

    BOOST_CHECK(asn1cpp::setof::pushList(test->boolean, values[0]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->boolean, values[1]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->boolean, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::setof::getSize(test->boolean), 3);

    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->boolean, bool, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->boolean, bool, 1), values[1]);
    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->boolean, bool, 2), values[2]);
}

BOOST_AUTO_TEST_CASE( remove_boolean ) {
    constexpr std::array<bool, 3> values = {{true, true, false}};

    auto test = asn1cpp::makeSeq(TestSetOf);

    BOOST_CHECK(asn1cpp::setof::pushList(test->boolean, values[0]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->boolean, values[1]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->boolean, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::setof::getSize(test->boolean), 3);

    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->boolean, bool, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->boolean, bool, 1), values[1]);
    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->boolean, bool, 2), values[2]);

    BOOST_CHECK(asn1cpp::setof::popList(test->boolean, BOOLEAN, 1));

    BOOST_CHECK_EQUAL(asn1cpp::setof::getSize(test->boolean), 2);

    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->boolean, bool, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::setof::getField(test->boolean, bool, 1), values[2]);
}

BOOST_AUTO_TEST_CASE( clear_boolean ) {
    constexpr std::array<bool, 3> values = {{false, false, true}};

    auto test = asn1cpp::makeSeq(TestSetOf);

    BOOST_CHECK(asn1cpp::setof::pushList(test->boolean, values[0]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->boolean, values[1]));
    BOOST_CHECK(asn1cpp::setof::pushList(test->boolean, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::setof::getSize(test->boolean), 3);

    asn1cpp::setof::clrField(test->boolean, BOOLEAN);

    BOOST_CHECK_EQUAL(asn1cpp::setof::getSize(test->boolean), 0);
}
