#define BOOST_TEST_MODULE intTests
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "TestInteger.h"

#include "asn1cpp/Seq.hpp"
#include "asn1cpp/Getter.hpp"
#include "asn1cpp/Setter.hpp"
#include "asn1cpp/Encoding.hpp"

BOOST_AUTO_TEST_CASE( construction ) {
    auto test = asn1cpp::makeSeq(TestInteger);
}

BOOST_AUTO_TEST_CASE( assignment ) {
    constexpr int value = 5;

    auto test = asn1cpp::makeSeq(TestInteger);

    asn1cpp::set(test->integer, value);

    auto v = asn1cpp::get(test->integer, int);

    BOOST_CHECK_EQUAL(value, v);
}

BOOST_AUTO_TEST_CASE( encoding ) {
    constexpr int value = 64;

    auto test = asn1cpp::makeSeq(TestInteger);

    asn1cpp::set(test->integer, value);

    auto str = asn1cpp::ber::encode(test);
    auto recoveredTest = asn1cpp::ber::decode(str, TestInteger);

    BOOST_CHECK(recoveredTest);

    auto v = asn1cpp::get(recoveredTest->integer, int);

    BOOST_CHECK_EQUAL(value, v);
}

BOOST_AUTO_TEST_CASE( copy ) {
    constexpr int value = 5;

    auto test = asn1cpp::makeSeq(TestInteger);

    asn1cpp::set(test->integer, value);

    auto copy = asn1cpp::Seq<TestInteger>(test);

    auto v = asn1cpp::get(copy->integer, int);

    BOOST_CHECK_EQUAL(value, v);
    BOOST_CHECK_EQUAL(test, copy);
}
