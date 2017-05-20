#ifndef ASN1CPP_GETTER_HEADER_FILE
#define ASN1CPP_GETTER_HEADER_FILE

#include <cerrno>
#include <string>
#include <type_traits>

#include "BOOLEAN.h"
#include "INTEGER.h"
#include "OCTET_STRING.h"

#include "asn1cpp/Seq.hpp"
#include "asn1cpp/View.hpp"

namespace asn1cpp {
    namespace Impl {
        template <typename R, typename T, typename Check = void>
        struct Getter {
            static_assert(!std::is_same<T, T>::value, "Getter not implemented for this type!");
        };

        template <>
        struct Getter<bool, BOOLEAN_t> {
            bool operator()(const BOOLEAN_t * field, bool & ok) {
                ok = true;
                return !!*field;
            }
        };

        template <>
        struct Getter<long, INTEGER_t> {
            long operator()(const INTEGER_t * field, bool & ok) {
                long retval;
                if (asn_INTEGER2long(field, &retval) == 0) {
                    ok = true;
                    return retval;
                } else {
                    ok = (errno == EINVAL);
                    return 0;
                }
            }
        };

        template <>
        struct Getter<unsigned long, INTEGER_t> {
            unsigned long operator()(const INTEGER_t * field, bool & ok) {
                unsigned long retval;
                if (asn_INTEGER2ulong(field, &retval) == 0) {
                    ok = true;
                    return retval;
                } else {
                    ok = (errno == EINVAL);
                    return 0;
                }
            }
        };

        template <typename R>
        struct Getter<R, INTEGER_t, typename std::enable_if<std::is_integral<R>::value && std::is_signed<R>::value>::type> {
            R operator()(const INTEGER_t * field, bool & ok) {
                return static_cast<R>(Getter<long, INTEGER_t>()(field, ok));
            }
        };

        template <typename R>
        struct Getter<R, INTEGER_t, typename std::enable_if<std::is_integral<R>::value && std::is_unsigned<R>::value>::type> {
            R operator()(const INTEGER_t * field, bool & ok) {
                return static_cast<R>(Getter<unsigned long, INTEGER_t>()(field, ok));
            }
        };

        template <typename R>
        struct Getter<R, long, typename std::enable_if<std::is_arithmetic<R>::value>::type> {
            R operator()(const long * field, bool & ok) {
                ok = true;
                return static_cast<R>(*field);
            }
        };

        template <>
        struct Getter<std::string, OCTET_STRING_t> {
            std::string operator()(const OCTET_STRING_t * field, bool & ok) {
                ok = true;
                return std::string((const char *)field->buf, field->size);
            }
        };

        template <typename R, typename T>
        struct Getter<R, T, typename std::enable_if<std::is_enum<R>::value && std::is_enum<T>::value>::type> {
            R operator()(const T * field, bool & ok) {
                ok = true;
                return static_cast<R>(*field);
            }
        };

        template <typename R, typename T>
        struct Getter<R, T, typename std::enable_if<std::is_enum<R>::value && !std::is_enum<T>::value>::type> {
            R operator()(const T * field, bool & ok) {
                return static_cast<R>(Getter<int, T>()(field, ok));
            }
        };

        template <typename T>
        struct Getter<Seq<T>, T> {
            Seq<T> operator()(const T * field, asn_TYPE_descriptor_t * def, bool & ok) {
                ok = true;
                return Seq<T>(View<T>(def, field));
            }
        };
    }

    template <typename R, typename T>
    R get(const T & field, bool * ok = nullptr) {
        bool iok;
        return Impl::Getter<R, T>()(&field, ok ? *ok : iok);
    }

    template <typename R, typename T>
    R get(const T *& field, bool * ok = nullptr) {
        if (field) {
            bool iok;
            return Impl::Getter<R, T>()(field, ok ? *ok : iok);
        } else {
            if (ok) *ok = true;
            return R();
        }
    }

    template <typename T>
    Seq<T> getSeq(const T & field, asn_TYPE_descriptor_t * def, bool * ok = nullptr) {
        bool iok;
        return Impl::Getter<Seq<T>, T>()(&field, def, ok ? ok : &iok);
    }

    template <typename T>
    View<T> getView(T & field, asn_TYPE_descriptor_t * def, bool * ok = nullptr) {
        bool iok;
        return Impl::Getter<View<T>, T>()(&field, def, ok ? ok : &iok);
    }
}

#define get(field, R, ...) \
    get<R>(field, ## __VA_ARGS__)

#define getSeq(field, R, ...) \
    getSeq(field, &ASN1CPP_ASN1C_DEF(R), ## __VA_ARGS__)

#define getView(field, R, ...) \
    getView(field, &ASN1CPP_ASN1C_DEF(R), ## __VA_ARGS__)

#endif