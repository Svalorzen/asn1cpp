#define BOOST_TEST_MODULE boolTests
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "TestBoolean.h"

#include "asn1cpp/Seq.hpp"
#include "asn1cpp/Getter.hpp"
#include "asn1cpp/Setter.hpp"
#include "asn1cpp/Encoding.hpp"

BOOST_AUTO_TEST_CASE( construction ) {
    auto test = asn1cpp::makeSeq(TestBoolean);
}

BOOST_AUTO_TEST_CASE( assignment ) {
    constexpr bool value = true;

    auto test = asn1cpp::makeSeq(TestBoolean);

    asn1cpp::set(test->boolean, value);

    auto v = asn1cpp::get(test->boolean, bool);

    BOOST_CHECK_EQUAL(value, v);
}

BOOST_AUTO_TEST_CASE( encoding ) {
    constexpr bool value = true;

    auto test = asn1cpp::makeSeq(TestBoolean);

    asn1cpp::set(test->boolean, value);

    auto str = asn1cpp::ber::encode(test);
    auto recoveredTest = asn1cpp::ber::decode(str, TestBoolean);

    BOOST_CHECK(recoveredTest);

    auto v = asn1cpp::get(recoveredTest->boolean, bool);

    BOOST_CHECK_EQUAL(value, v);
}
