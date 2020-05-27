#define BOOST_TEST_MODULE optionalTests
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "TestOptional.h"
#include "TestInteger.h"

#include "asn1cpp/Seq.hpp"
#include "asn1cpp/Getter.hpp"
#include "asn1cpp/Setter.hpp"
#include "asn1cpp/Encoding.hpp"
#include "asn1cpp/SequenceOf.hpp"

BOOST_AUTO_TEST_CASE( construction ) {
    auto test = asn1cpp::makeSeq(TestOptional);
}

BOOST_AUTO_TEST_CASE( assignment ) {
    const std::string str = "abcd";
    constexpr unsigned integer = 98;
    constexpr bool boolean = false;
    constexpr std::array<int, 3> values = {{5, 54, 190}};

    auto test = asn1cpp::makeSeq(TestOptional);

    BOOST_CHECK(asn1cpp::setField(test->string, str));
    BOOST_CHECK(asn1cpp::setField(test->integer, integer));
    BOOST_CHECK(asn1cpp::setField(test->boolean, boolean));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->sequence, values[0]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->sequence, values[1]));
    BOOST_CHECK(asn1cpp::sequenceof::pushList(test->sequence, values[2]));

    bool strOk;
    bool intOk;
    bool boolOk;

    auto vstr = asn1cpp::getField(test->string, std::string, &strOk);
    auto vint = asn1cpp::getField(test->integer, unsigned, &intOk);
    auto vbool = asn1cpp::getField(test->boolean, bool, &boolOk);

    BOOST_CHECK(strOk);
    BOOST_CHECK(intOk);
    BOOST_CHECK(boolOk);

    for (size_t i = 0; i < values.size(); ++i) {
        BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->sequence, int, i, &intOk), values[i]);
        BOOST_CHECK(intOk);
    }
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->sequence), values.size());

    BOOST_CHECK_EQUAL(str, vstr);
    BOOST_CHECK_EQUAL(integer, vint);
    BOOST_CHECK_EQUAL(boolean, vbool);
}

BOOST_AUTO_TEST_CASE( nonassignment ) {
    auto test = asn1cpp::makeSeq(TestOptional);

    bool strOk;
    bool intOk;
    bool boolOk;
    bool seqOk;

    asn1cpp::getField(test->string, std::string, &strOk);
    asn1cpp::getField(test->integer, unsigned, &intOk);
    asn1cpp::getField(test->boolean, bool, &boolOk);
    asn1cpp::sequenceof::getField(test->sequence, int, 0, &seqOk);

    BOOST_CHECK(!strOk);
    BOOST_CHECK(!intOk);
    BOOST_CHECK(!boolOk);
    BOOST_CHECK(!seqOk);
}

BOOST_AUTO_TEST_CASE( encoding ) {
    const std::string str = "qwerty";
    constexpr bool boolean = true;
    constexpr std::array<int, 2> values = {{19, 1}};

    auto test = asn1cpp::makeSeq(TestOptional);

    asn1cpp::setField(test->string, str);
    asn1cpp::setField(test->boolean, boolean);
    asn1cpp::sequenceof::pushList(test->sequence, values[0]);
    asn1cpp::sequenceof::pushList(test->sequence, values[1]);

    auto encoded = asn1cpp::ber::encode(test);
    auto recoveredTest = asn1cpp::ber::decode(encoded, TestOptional);

    BOOST_CHECK(recoveredTest);

    bool strOk;
    bool intOk;
    bool boolOk;

    auto vstr = asn1cpp::getField(recoveredTest->string, std::string, &strOk);
    asn1cpp::getField(recoveredTest->integer, unsigned, &intOk);
    auto vbool = asn1cpp::getField(recoveredTest->boolean, bool, &boolOk);

    BOOST_CHECK(strOk);
    BOOST_CHECK(!intOk);
    BOOST_CHECK(boolOk);

    BOOST_CHECK_EQUAL(str, vstr);
    BOOST_CHECK_EQUAL(boolean, vbool);

    for (size_t i = 0; i < values.size(); ++i) {
        BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(recoveredTest->sequence, int, i, &intOk), values[i]);
        BOOST_CHECK(intOk);
    }
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->sequence), values.size());
}

BOOST_AUTO_TEST_CASE( copy ) {
    const std::string str = "lkjhgf";
    constexpr unsigned integer = 9571;
    constexpr bool boolean = true;
    constexpr std::array<int, 4> values = {{99, -15, 220, 15914}};

    auto test = asn1cpp::makeSeq(TestOptional);

    asn1cpp::setField(test->string, str);
    asn1cpp::setField(test->integer, integer);
    asn1cpp::setField(test->boolean, boolean);
    for (size_t i = 0; i < values.size(); ++i)
        asn1cpp::sequenceof::pushList(test->sequence, values[i]);

    auto copy = test;

    auto vstr = asn1cpp::getField(copy->string, std::string);
    auto vint = asn1cpp::getField(copy->integer, unsigned);
    auto vbool = asn1cpp::getField(copy->boolean, bool);

    BOOST_CHECK_EQUAL(str, vstr);
    BOOST_CHECK_EQUAL(integer, vint);
    BOOST_CHECK_EQUAL(boolean, vbool);
    BOOST_CHECK_EQUAL(test, copy);
    for (size_t i = 0; i < values.size(); ++i) {
        bool testOk, copyOk;
        BOOST_CHECK_EQUAL(
            asn1cpp::sequenceof::getField(test->sequence, int, i, &testOk),
            asn1cpp::sequenceof::getField(copy->sequence, int, i, &copyOk)
        );
        BOOST_CHECK(testOk);
        BOOST_CHECK(copyOk);
    }
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(copy->sequence), values.size());

    BOOST_CHECK(test == copy);
    BOOST_CHECK(!(test != copy));

    asn1cpp::setField(copy->string, "ooooo");
    BOOST_CHECK(test != copy);
    BOOST_CHECK(!(test == copy));

    asn1cpp::sequenceof::setField(test->sequence, 20, 0);
    BOOST_CHECK(test != copy);
}

BOOST_AUTO_TEST_CASE( clear ) {
    const std::string str = "abcd";
    constexpr unsigned integer = 98;
    constexpr bool boolean = false;
    constexpr std::array<int, 4> values = {{99, -15, 220, 15914}};

    auto test = asn1cpp::makeSeq(TestOptional);

    asn1cpp::setField(test->string, str);
    asn1cpp::setField(test->integer, integer);
    asn1cpp::setField(test->boolean, boolean);
    for (size_t i = 0; i < values.size(); ++i)
        asn1cpp::sequenceof::pushList(test->sequence, values[i]);

    bool strOk;
    bool intOk;
    bool boolOk;

    auto vstr = asn1cpp::getField(test->string, std::string, &strOk);
    auto vint = asn1cpp::getField(test->integer, unsigned, &intOk);
    auto vbool = asn1cpp::getField(test->boolean, bool, &boolOk);

    BOOST_CHECK(strOk);
    BOOST_CHECK(intOk);
    BOOST_CHECK(boolOk);

    BOOST_CHECK_EQUAL(str, vstr);
    BOOST_CHECK_EQUAL(integer, vint);
    BOOST_CHECK_EQUAL(boolean, vbool);

    for (size_t i = 0; i < values.size(); ++i) {
        BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getField(test->sequence, int, i, &intOk), values[i]);
        BOOST_CHECK(intOk);
    }
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->sequence), values.size());

    asn1cpp::clrField(test->string, OCTET_STRING);
    asn1cpp::clrField(test->integer, INTEGER);
    asn1cpp::clrField(test->boolean, BOOLEAN);
    asn1cpp::clrField(test->sequence, INTEGER);

    asn1cpp::getField(test->string, std::string, &strOk);
    asn1cpp::getField(test->integer, unsigned, &intOk);
    asn1cpp::getField(test->boolean, bool, &boolOk);

    BOOST_CHECK(!strOk);
    BOOST_CHECK(!intOk);
    BOOST_CHECK(!boolOk);
    for (size_t i = 0; i < values.size(); ++i) {
        asn1cpp::sequenceof::getField(test->sequence, int, i, &intOk);
        BOOST_CHECK(!intOk);
    }
    BOOST_CHECK_EQUAL(asn1cpp::sequenceof::getSize(test->sequence), -1);
}

BOOST_AUTO_TEST_CASE( add_nested ) {
    auto insert0 = asn1cpp::makeSeq(TestInteger);
    BOOST_CHECK(asn1cpp::setField(insert0->integer, 45));

    auto seq = asn1cpp::makeSeq(TestInteger);
    BOOST_CHECK(asn1cpp::setField(seq->integer, 23));
    asn1cpp::View<TestInteger> insert1 = seq;

    auto test = asn1cpp::makeSeq(TestOptional);

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
