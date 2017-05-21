#define BOOST_TEST_MODULE optionalTests
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "TestOptional.h"

#include "asn1cpp/Seq.hpp"
#include "asn1cpp/Getter.hpp"
#include "asn1cpp/Setter.hpp"
#include "asn1cpp/Encoding.hpp"

BOOST_AUTO_TEST_CASE( construction ) {
    auto test = asn1cpp::makeSeq(TestOptional);
}

BOOST_AUTO_TEST_CASE( assignment ) {
    const std::string str = "abcd";
    constexpr unsigned integer = 98;
    constexpr bool boolean = false;

    auto test = asn1cpp::makeSeq(TestOptional);

    asn1cpp::set(test->string, str);
    asn1cpp::set(test->integer, integer);
    asn1cpp::set(test->boolean, boolean);

    bool strOk;
    bool intOk;
    bool boolOk;

    auto vstr = asn1cpp::get(test->string, std::string, &strOk);
    auto vint = asn1cpp::get(test->integer, unsigned, &intOk);
    auto vbool = asn1cpp::get(test->boolean, bool, &boolOk);

    BOOST_CHECK(strOk);
    BOOST_CHECK(intOk);
    BOOST_CHECK(boolOk);

    BOOST_CHECK_EQUAL(str, vstr);
    BOOST_CHECK_EQUAL(integer, vint);
    BOOST_CHECK_EQUAL(boolean, vbool);
}

BOOST_AUTO_TEST_CASE( nonassignment ) {
    auto test = asn1cpp::makeSeq(TestOptional);

    bool strOk;
    bool intOk;
    bool boolOk;

    asn1cpp::get(test->string, std::string, &strOk);
    asn1cpp::get(test->integer, unsigned, &intOk);
    asn1cpp::get(test->boolean, bool, &boolOk);

    BOOST_CHECK(!strOk);
    BOOST_CHECK(!intOk);
    BOOST_CHECK(!boolOk);
}

BOOST_AUTO_TEST_CASE( encoding ) {
    const std::string str = "qwerty";
    constexpr bool boolean = true;

    auto test = asn1cpp::makeSeq(TestOptional);

    asn1cpp::set(test->string, str);
    asn1cpp::set(test->boolean, boolean);

    auto encoded = asn1cpp::ber::encode(test);
    auto recoveredTest = asn1cpp::ber::decode(encoded, TestOptional);

    BOOST_CHECK(recoveredTest);

    bool strOk;
    bool intOk;
    bool boolOk;

    auto vstr = asn1cpp::get(recoveredTest->string, std::string, &strOk);
    asn1cpp::get(recoveredTest->integer, unsigned, &intOk);
    auto vbool = asn1cpp::get(recoveredTest->boolean, bool, &boolOk);

    BOOST_CHECK(strOk);
    BOOST_CHECK(!intOk);
    BOOST_CHECK(boolOk);

    BOOST_CHECK_EQUAL(str, vstr);
    BOOST_CHECK_EQUAL(boolean, vbool);
}

BOOST_AUTO_TEST_CASE( copy ) {
    const std::string str = "lkjhgf";
    constexpr unsigned integer = 9571;
    constexpr bool boolean = true;

    auto test = asn1cpp::makeSeq(TestOptional);

    asn1cpp::set(test->string, str);
    asn1cpp::set(test->integer, integer);
    asn1cpp::set(test->boolean, boolean);

    auto copy = asn1cpp::Seq<TestOptional>(test);

    auto vstr = asn1cpp::get(copy->string, std::string);
    auto vint = asn1cpp::get(copy->integer, unsigned);
    auto vbool = asn1cpp::get(copy->boolean, bool);

    BOOST_CHECK_EQUAL(str, vstr);
    BOOST_CHECK_EQUAL(integer, vint);
    BOOST_CHECK_EQUAL(boolean, vbool);
    BOOST_CHECK_EQUAL(test, copy);
}
