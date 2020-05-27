#define BOOST_TEST_MODULE sequenceofTests
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <array>

#include "TestSequenceOf.h"

#include "asn1cpp/Seq.hpp"
#include "asn1cpp/Getter.hpp"
#include "asn1cpp/Setter.hpp"
#include "asn1cpp/Encoding.hpp"
#include "asn1cpp/SequenceOf.hpp"

BOOST_AUTO_TEST_CASE( construction ) {
    auto test = asn1cpp::makeSeq(TestSequenceOf);
}

BOOST_AUTO_TEST_CASE( add_integer ) {
    constexpr std::array<int, 3> values = {{5, 54, 190}};

    auto test = asn1cpp::makeSeq(TestSequenceOf);

    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->integer, values[0]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->integer, values[1]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->integer, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->integer), 3);

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->integer, int, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->integer, int, 1), values[1]);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->integer, int, 2), values[2]);
}

BOOST_AUTO_TEST_CASE( set_integer ) {
    constexpr std::array<int, 3> values = {{5, 54, 190}};
    constexpr int newValue = 778;

    auto test = asn1cpp::makeSeq(TestSequenceOf);

    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->integer, values[0]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->integer, values[1]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->integer, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->integer), 3);

    BOOST_CHECK(asn1cpp::sequenceof::setField(test->integer, newValue, 1));
    BOOST_CHECK(!asn1cpp::sequenceof::setField(test->integer, newValue, 3));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->integer, int, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->integer, int, 1), newValue);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->integer, int, 2), values[2]);
}

BOOST_AUTO_TEST_CASE( remove_integer ) {
    constexpr std::array<int, 3> values = {{12, 23, 34}};

    auto test = asn1cpp::makeSeq(TestSequenceOf);

    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->integer, values[0]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->integer, values[1]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->integer, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->integer), 3);

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->integer, int, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->integer, int, 1), values[1]);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->integer, int, 2), values[2]);

    BOOST_CHECK(asn1cpp::sequenceof::popList(test->integer, INTEGER, 1));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->integer), 2);

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->integer, int, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->integer, int, 1), values[2]);
}

BOOST_AUTO_TEST_CASE( clear_integer ) {
    constexpr std::array<int, 3> values = {{12, 23, 34}};

    auto test = asn1cpp::makeSeq(TestSequenceOf);

    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->integer, values[0]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->integer, values[1]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->integer, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->integer), 3);

    asn1cpp::sequenceof::clrField(test->integer, INTEGER);

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->integer), 0);
}

BOOST_AUTO_TEST_CASE( add_string ) {
    const std::array<std::string, 3> values = {{"alpha", "beta", "gamma"}};

    auto test = asn1cpp::makeSeq(TestSequenceOf);

    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->string, values[0]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->string, values[1]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->string, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->string), 3);

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->string, std::string, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->string, std::string, 1), values[1]);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->string, std::string, 2), values[2]);
}

BOOST_AUTO_TEST_CASE( remove_string ) {
    const std::array<std::string, 3> values = {{"veni", "vidi", "vici"}};

    auto test = asn1cpp::makeSeq(TestSequenceOf);

    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->string, values[0]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->string, values[1]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->string, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->string), 3);

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->string, std::string, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->string, std::string, 1), values[1]);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->string, std::string, 2), values[2]);

    BOOST_CHECK(asn1cpp::sequenceof::popList(test->string, OCTET_STRING, 1));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->string), 2);

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->string, std::string, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->string, std::string, 1), values[2]);
}

BOOST_AUTO_TEST_CASE( clear_string ) {
    const std::array<std::string, 3> values = {{"god", "is", "dead"}};

    auto test = asn1cpp::makeSeq(TestSequenceOf);

    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->string, values[0]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->string, values[1]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->string, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->string), 3);

    asn1cpp::sequenceof::clrField(test->string, OCTET_STRING);

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->string), 0);
}

BOOST_AUTO_TEST_CASE( add_boolean ) {
    constexpr std::array<bool, 3> values = {{false, true, false}};

    auto test = asn1cpp::makeSeq(TestSequenceOf);

    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->boolean, values[0]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->boolean, values[1]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->boolean, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->boolean), 3);

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->boolean, bool, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->boolean, bool, 1), values[1]);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->boolean, bool, 2), values[2]);
}

BOOST_AUTO_TEST_CASE( remove_boolean ) {
    constexpr std::array<bool, 3> values = {{true, true, false}};

    auto test = asn1cpp::makeSeq(TestSequenceOf);

    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->boolean, values[0]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->boolean, values[1]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->boolean, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->boolean), 3);

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->boolean, bool, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->boolean, bool, 1), values[1]);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->boolean, bool, 2), values[2]);

    BOOST_CHECK(asn1cpp::sequenceof::popList(test->boolean, BOOLEAN, 1));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->boolean), 2);

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->boolean, bool, 0), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->boolean, bool, 1), values[2]);
}

BOOST_AUTO_TEST_CASE( clear_boolean ) {
    constexpr std::array<bool, 3> values = {{false, false, true}};

    auto test = asn1cpp::makeSeq(TestSequenceOf);

    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->boolean, values[0]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->boolean, values[1]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->boolean, values[2]));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->boolean), 3);

    asn1cpp::sequenceof::clrField(test->boolean, BOOLEAN);

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->boolean), 0);
}

BOOST_AUTO_TEST_CASE( add_nested ) {
    auto insert0 = asn1cpp::makeSeq(TestInteger);
    BOOST_CHECK(asn1cpp::setField(insert0->integer, 45));

    auto seq = asn1cpp::makeSeq(TestInteger);
    BOOST_CHECK(asn1cpp::setField(seq->integer, 23));
    asn1cpp::View<TestInteger> insert1 = seq;

    auto test = asn1cpp::makeSeq(TestSequenceOf);

    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->nested, insert0));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->nested, insert1));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->nested), 2);

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSeq(test->nested, TestInteger, 0), insert0);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getView(test->nested, TestInteger, 0), insert0);

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSeq(test->nested, TestInteger, 1), insert1);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getView(test->nested, TestInteger, 1), insert1);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSeq(test->nested, TestInteger, 1), seq);
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getView(test->nested, TestInteger, 1), seq);
}

BOOST_AUTO_TEST_CASE( remove_nested ) {
    constexpr std::array<int, 3> values = {{99, 202, 101}};
    auto insert = asn1cpp::makeSeq(TestInteger);

    auto test = asn1cpp::makeSeq(TestSequenceOf);

    BOOST_CHECK(asn1cpp::setField(insert->integer, values[0]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->nested, insert));

    BOOST_CHECK(asn1cpp::setField(insert->integer, values[1]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->nested, insert));

    BOOST_CHECK(asn1cpp::setField(insert->integer, values[2]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->nested, insert));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->nested), 3);

    BOOST_CHECK_EQUAL(asn1cpp::getField(asn1cpp::sequenceof::getView(test->nested, TestInteger, 0)->integer, int), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::getField(asn1cpp::sequenceof::getView(test->nested, TestInteger, 1)->integer, int), values[1]);
    BOOST_CHECK_EQUAL(asn1cpp::getField(asn1cpp::sequenceof::getView(test->nested, TestInteger, 2)->integer, int), values[2]);

    BOOST_CHECK(asn1cpp::sequenceof::popList(test->nested, TestInteger, 1));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->nested), 2);

    BOOST_CHECK_EQUAL(asn1cpp::getField(asn1cpp::sequenceof::getView(test->nested, TestInteger, 0)->integer, int), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::getField(asn1cpp::sequenceof::getView(test->nested, TestInteger, 1)->integer, int), values[2]);
}

BOOST_AUTO_TEST_CASE( clear_nested ) {
    constexpr std::array<int, 3> values = {{142, 798, 442}};
    auto insert = asn1cpp::makeSeq(TestInteger);

    auto test = asn1cpp::makeSeq(TestSequenceOf);

    BOOST_CHECK(asn1cpp::setField(insert->integer, values[0]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->nested, insert));

    BOOST_CHECK(asn1cpp::setField(insert->integer, values[1]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->nested, insert));

    BOOST_CHECK(asn1cpp::setField(insert->integer, values[2]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->nested, insert));

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->nested), 3);

    BOOST_CHECK_EQUAL(asn1cpp::getField(asn1cpp::sequenceof::getView(test->nested, TestInteger, 0)->integer, int), values[0]);
    BOOST_CHECK_EQUAL(asn1cpp::getField(asn1cpp::sequenceof::getView(test->nested, TestInteger, 1)->integer, int), values[1]);
    BOOST_CHECK_EQUAL(asn1cpp::getField(asn1cpp::sequenceof::getView(test->nested, TestInteger, 2)->integer, int), values[2]);

    asn1cpp::sequenceof::clrField(test->nested, TestInteger);

    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->nested), 0);
}
