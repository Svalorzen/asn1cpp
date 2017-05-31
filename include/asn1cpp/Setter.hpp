#ifndef ASN1CPP_SETTER_HEADER_FILE
#define ASN1CPP_SETTER_HEADER_FILE

#include <type_traits>

#include "BOOLEAN.h"
#include "INTEGER.h"
#include "OCTET_STRING.h"

#include "asn1cpp/Utils.hpp"
#include "asn1cpp/View.hpp"

namespace asn1cpp {
    namespace Impl {
        template <typename T, typename Check = void>
        struct Setter {
            template <template <typename> class S, typename = typename std::enable_if<is_asn1_wrapper<S<T>>::value>::type>
            bool operator()(T * field, const S<T> & v) {
                if (!field) return false;

                View<T> vf(field);
                vf = v;

                return true;
            }
        };

        template <typename T>
        struct Setter<T, typename std::enable_if<std::is_fundamental<T>::value || std::is_enum<T>::value>::type> {
            bool operator()(T * field, const T & value) {
                if (!field) return false;
                *field = value;
                return true;
            }
        };

        template <>
        struct Setter<BOOLEAN_t> {
            bool operator()(BOOLEAN_t * field, bool value) {
                *field = static_cast<BOOLEAN_t>(value);
                return true;
            }
        };

        template <>
        struct Setter<INTEGER_t> {
            bool operator()(INTEGER_t * field, long value) {
                return asn_long2INTEGER(field, value) == 0;
            }
            bool operator()(INTEGER_t * field, int value) {
                return operator()(field, static_cast<long>(value));
            }
            bool operator()(INTEGER_t * field, unsigned long value) {
                return asn_ulong2INTEGER(field, value) == 0;
            }
            bool operator()(INTEGER_t * field, unsigned int value) {
                return operator()(field, static_cast<unsigned long>(value));
            }
        };

        template <>
        struct Setter<OCTET_STRING_t> {
            bool operator()(OCTET_STRING_t * field, const std::string & value) {
                return OCTET_STRING_fromBuf(field, value.data(), value.size()) == 0;
            }
            bool operator()(OCTET_STRING_t * field, const char * value) {
                return operator()(field, std::string(value));
            }
            bool operator()(OCTET_STRING_t * field, const OCTET_STRING_t * value) {
                return OCTET_STRING_fromBuf(field, reinterpret_cast<const char *>(value->buf), value->size) == 0;
            }
        };
    }

    template <typename F, typename V>
    bool setterField(F & field, const V & value) {
        return Impl::Setter<F>()(&field, value);
    }

    template <typename F, typename V>
    bool setterField(F *& field, const V & value) {
        if (!field) field = static_cast<F*>(calloc(1, sizeof(F)));
        return Impl::Setter<F>()(field, value);
    }

    template <typename F>
    bool clearerField(F *& field, asn_TYPE_descriptor_t * def) {
        if (field) {
            def->free_struct(def, field, 0);
            field = nullptr;
        }
        return true;
    }
}

#define setField(field, V, ...) \
    setterField(field, V, ## __VA_ARGS__)

#define clrField(field, T, ...) \
    clearerField(field, &ASN1CPP_ASN1C_DEF(T), ## __VA_ARGS__)

#endif
