#ifndef ASN1CPP_ENCODING_HEADER_FILE
#define ASN1CPP_ENCODING_HEADER_FILE

#include <string>

#include "asn_application.h"

#include "asn1cpp/Utils.hpp"
#include "asn1cpp/Seq.hpp"

namespace asn1cpp {
    namespace Impl {
        inline int fill(const void * buffer, size_t size, void * appKey) {
            std::string * str = static_cast<std::string*>(appKey);
            if (!str)
                return -1;
            str->append((const char*)buffer, size);
            return 0;
        }
    }

    namespace ber {
        template <typename T, typename = typename std::enable_if<is_asn1_wrapper<T>::value>::type>
        std::string encode(const T & m) {
            if (!m) return {};
            std::string retval;
            const auto er = der_encode(m.getTypeDescriptor(), (void*)(&*m), Impl::fill, &retval);
            return er.encoded < 0 ? std::string() : retval;
        }

        template <typename T>
        Seq<T> decode(asn_TYPE_descriptor_t * def, const std::string & buffer) {
            if (buffer.size() == 0) return Seq<T>();

            T * m = nullptr;
            const auto dr = ber_decode(0, def, (void**)&m, buffer.data(), buffer.size());

            if (dr.code != RC_OK) {
                def->free_struct(def, m, 0);
                return Seq<T>();
            }

            return Seq<T>(def, m);
        }
    }

    namespace xer {
        template <typename T, typename = typename std::enable_if<is_asn1_wrapper<T>::value>::type>
        std::string encode(const T & m) {
            if (!m) return {};
            std::string retval;
            const auto er = xer_encode(m.getTypeDescriptor(), (void*)(&*m), XER_F_BASIC, Impl::fill, &retval);
            return er.encoded < 0 ? std::string() : retval;
        }
    }

    namespace uper {
        template <typename T, typename = typename std::enable_if<is_asn1_wrapper<T>::value>::type>
        std::string encode(const T & m) {
            if (!m) return {};
            std::string retval;
            const auto er = uper_encode(m.getTypeDescriptor(), (void*)(&*m), Impl::fill, &retval);
            return er.encoded < 0 ? std::string() : retval;
        }

        template <typename T>
        Seq<T> decode(asn_TYPE_descriptor_t * def, const std::string & buffer) {
            if (buffer.size() == 0) return Seq<T>();

            T * m = nullptr;
            const auto dr = uper_decode_complete(0, def, (void**)&m, buffer.data(), buffer.size());

            if (dr.code != RC_OK) {
                def->free_struct(def, m, 0);
                return Seq<T>();
            }

            return Seq<T>(def, m);
        }
    }
}

#define decode(m, T) \
    decode<T>(&ASN1CPP_ASN1C_DEF(T), m)

#endif
