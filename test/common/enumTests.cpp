#define BOOST_TEST_MODULE enumTests
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "TestEnum.h"

#include "asn1cpp/Seq.hpp"
#include "asn1cpp/Getter.hpp"
#include "asn1cpp/Setter.hpp"
#include "asn1cpp/Encoding.hpp"

BOOST_AUTO_TEST_CASE( construction ) {
    auto test = asn1cpp::makeSeq(TestEnum);
}

BOOST_AUTO_TEST_CASE( assignment ) {
    constexpr TestEnumType value = TestEnumType_value0;

    auto test = asn1cpp::makeSeq(TestEnum);

    asn1cpp::set(test->enm, value);

    auto v = asn1cpp::get(test->enm, TestEnumType);

    BOOST_CHECK_EQUAL(value, v);
}

BOOST_AUTO_TEST_CASE( encoding ) {
    constexpr TestEnumType value = TestEnumType_value2;

    auto test = asn1cpp::makeSeq(TestEnum);

    asn1cpp::set(test->enm, value);

    auto str = asn1cpp::ber::encode(test);
    auto recoveredTest = asn1cpp::ber::decode(str, TestEnum);

    BOOST_CHECK(recoveredTest);

    auto v = asn1cpp::get(recoveredTest->enm, TestEnumType);

    BOOST_CHECK_EQUAL(value, v);
}

BOOST_AUTO_TEST_CASE( copy ) {
    constexpr TestEnumType value = TestEnumType_value1;

    auto test = asn1cpp::makeSeq(TestEnum);

    asn1cpp::set(test->enm, value);

    auto copy = asn1cpp::Seq<TestEnum>(test);

    auto v = asn1cpp::get(copy->enm, TestEnumType);

    BOOST_CHECK_EQUAL(value, v);
    BOOST_CHECK_EQUAL(test, copy);
}
