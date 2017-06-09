#ifndef ASN1CPP_UTILS_HEADER_FILE
#define ASN1CPP_UTILS_HEADER_FILE

#include <type_traits>

#include "asn_application.h"

// This macro is used in order to be able to access an asn1c struct definition
// just by the type name.
#define ASN1CPP_ASN1C_DEF(T) asn_DEF_##T

namespace asn1cpp {
    template <typename W>
    struct is_asn1_wrapper {
        enum { value = false };
    };

    template <template <typename> class W, typename T>
    struct is_asn1_wrapper<W<T>>{
        private:
            template <typename Z> static constexpr auto test(int) -> decltype(

                    static_cast<asn_TYPE_descriptor_t * (Z::*)() const>                     (&Z::getTypeDescriptor),
                    static_cast<T & (Z::*)()>                                               (&Z::operator*),
                    static_cast<const T & (Z::*)() const>                                   (&Z::operator*),
                    static_cast<T * (Z::*)()>                                               (&Z::operator->),
                    static_cast<const T * (Z::*)() const>                                   (&Z::operator->),

                    bool()
            ) { return true; }

            template <typename Z> static constexpr auto test(...) -> bool
            { return false; }

        public:
            enum { value = test<W<T>>(0) };
    };

    template <typename A, typename B>
    struct are_compatible_asn1_wrappers {
        enum { value = false };
    };

    template <template <typename> class W, typename T,
              template <typename> class Z, typename Y>
    struct are_compatible_asn1_wrappers<W<T>, Z<Y>> {
        public:
            enum {
                value = is_asn1_wrapper<W<T>>::value &&
                        is_asn1_wrapper<Z<Y>>::value &&
                        std::is_same<
                            typename std::remove_cv<T>::type,
                            typename std::remove_cv<Y>::type
                        >::value
            };
    };

    template <typename A, typename B>
    struct is_convertible_asn1_wrapper {
        enum { value = false };
    };

    template <template <typename> class W, typename T,
              template <typename> class Z, typename Y>
    struct is_convertible_asn1_wrapper<W<T>, Z<Y>> {
        public:
            enum {
                value = is_asn1_wrapper<W<T>>::value &&
                        is_asn1_wrapper<Z<Y>>::value &&
                        (
                         std::is_same<T, Y>::value ||
                         std::is_same<
                            typename std::remove_cv<T>::type,
                            Y
                         >::value
                        )
            };
    };
}

#endif
