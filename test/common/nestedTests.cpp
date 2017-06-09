#define BOOST_TEST_MODULE nestedTests
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "TestNested.h"

#include "asn1cpp/Seq.hpp"
#include "asn1cpp/View.hpp"
#include "asn1cpp/Getter.hpp"
#include "asn1cpp/Setter.hpp"
#include "asn1cpp/Encoding.hpp"

BOOST_AUTO_TEST_CASE( construction ) {
    auto test = asn1cpp::makeSeq(TestNested);
}

BOOST_AUTO_TEST_CASE( assignment ) {
    constexpr unsigned value = 98;

    auto test = asn1cpp::makeSeq(TestNested);

    asn1cpp::setField(test->nested.integer, value);

    auto v = asn1cpp::getField(test->nested.integer, unsigned);

    BOOST_CHECK_EQUAL(v, value);
}

BOOST_AUTO_TEST_CASE( view_assignment ) {
    constexpr unsigned value = 44;
    auto test = asn1cpp::makeSeq(TestNested);

    auto view = asn1cpp::getView(test->nested, TestInteger);

    asn1cpp::setField(view->integer, value);

    auto v = asn1cpp::getField(test->nested.integer, unsigned);

    BOOST_CHECK_EQUAL(v, value);
}

BOOST_AUTO_TEST_CASE( view_copy ) {
    constexpr unsigned value = 44;
    auto test = asn1cpp::makeSeq(TestNested);

    auto view = asn1cpp::getView(test->nested, TestInteger);

    asn1cpp::setField(view->integer, value);

    auto copy = asn1cpp::Seq<TestInteger>(view);

    auto v = asn1cpp::getField(copy->integer, unsigned);

    BOOST_CHECK_EQUAL(v, value);
    BOOST_CHECK_EQUAL(view, copy);
}

BOOST_AUTO_TEST_CASE( view_const ) {
    constexpr unsigned value = 44;
    auto test = asn1cpp::makeSeq(TestNested);
    asn1cpp::setField(test->nested.integer, value);

    asn1cpp::View<const TestInteger> view = asn1cpp::getView(test->nested, TestInteger);

    auto testValue = asn1cpp::getField(test->nested.integer, unsigned);
    auto viewValue = asn1cpp::getField(view->integer, unsigned);

    BOOST_CHECK_EQUAL(testValue, viewValue);

    const auto const_view = asn1cpp::getView(test->nested, TestInteger);
    asn1cpp::View<const TestInteger> internal_const_view = const_view;

    auto v1 = asn1cpp::getField(const_view->integer, unsigned);
    auto v2 = asn1cpp::getField(internal_const_view->integer, unsigned);

    BOOST_CHECK_EQUAL(testValue, v1);
    BOOST_CHECK_EQUAL(v1, v2);
}

BOOST_AUTO_TEST_CASE( to_view_assignment ) {
    constexpr unsigned value = 897;
    auto test = asn1cpp::makeSeq(TestNested);

    auto view = asn1cpp::getView(test->nested, TestInteger);

    asn1cpp::setField(view->integer, value);

    auto v = asn1cpp::getField(test->nested.integer, unsigned);

    BOOST_CHECK_EQUAL(v, value);

    constexpr unsigned otherValue = 777;
    auto other = asn1cpp::makeSeq(TestInteger);
    asn1cpp::setField(other->integer, otherValue);

    view = other;
    auto vv = asn1cpp::getField(test->nested.integer, unsigned);
    auto otherv = asn1cpp::getField(other->integer, unsigned);

    BOOST_CHECK_EQUAL(vv, otherValue);
    BOOST_CHECK_EQUAL(otherv, otherValue);
}

BOOST_AUTO_TEST_CASE( encoding ) {
    constexpr unsigned value = 98;
    auto test = asn1cpp::makeSeq(TestNested);

    asn1cpp::setField(test->nested.integer, value);

    auto str = asn1cpp::ber::encode(test);
    auto recoveredTest = asn1cpp::ber::decode(str, TestNested);

    BOOST_CHECK_EQUAL(test, recoveredTest);
}
