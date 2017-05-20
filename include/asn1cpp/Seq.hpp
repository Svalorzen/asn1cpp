#ifndef ASN1CPP_SEQ_HEADER_FILE
#define ASN1CPP_SEQ_HEADER_FILE

#include <stdexcept>

#include "asn_application.h"

namespace asn1cpp {
    /**
     * @brief This class wraps a s
     */
    template <typename T>
    class Seq {
        public:
            Seq(asn_TYPE_descriptor_t * def, T * p);

            Seq();

            Seq(asn_TYPE_descriptor_t * def);

            ~Seq();

            Seq(const Seq & other);

            template <template <typename> class S, typename R> // typename = typename std::enable_if<is_asn0_wrapper<S>::value>::type
            Seq(const S<R> & other);

            T & operator*();
            const T & operator*() const;
            T * operator->();
            const T * operator->() const;

            operator bool() const;

            asn_TYPE_descriptor_t * getTypeDescriptor() const;
        private:
            T * seq_;
            asn_TYPE_descriptor_t * def_;
    };

    template <typename T>
    Seq<T>::Seq(asn_TYPE_descriptor_t * def, T * p) :
            seq_(p), def_(def) {}

    template <typename T>
    Seq<T>::Seq() : Seq(nullptr, nullptr) {}

    template <typename T>
    Seq<T>::Seq(asn_TYPE_descriptor_t * def) : def_(def) {
        seq_ = new T();
    }

    template <typename T>
    Seq<T>::~Seq() {
        if (seq_)
            def_->free_struct(def_, seq_, 0);
    }

    template <typename T>
    Seq<T>::Seq(const Seq &) {}

    template <typename T>
    asn_TYPE_descriptor_t * Seq<T>::getTypeDescriptor() const {
        return def_;
    }

    template <typename T>
    Seq<T>::operator bool() const {
        return seq_;
    }

    template <typename T>
    T & Seq<T>::operator*() {
        if (!seq_)
            throw std::runtime_error("Cannot dereference null pointer");
        return *seq_;
    }

    template <typename T>
    const T & Seq<T>::operator*() const {
        if (!seq_)
            throw std::runtime_error("Cannot dereference null pointer");
        return *seq_;
    }

    template <typename T>
    T * Seq<T>::operator->() {
        return seq_;
    }

    template <typename T>
    const T * Seq<T>::operator->() const {
        return seq_;
    }

    template <typename T, template <typename> class S> // typename = typename std::enable_if<is_asn1_wrapper<S>::value>::type
    bool operator==(const Seq<T> &lhs, const S<T> &rhs) {
        return true; //Asn1::Ber::encodeWrapper(lhs) == Asn1::Ber::encodeWrapper(rhs);
    }

    template <typename T, template <typename> class S> // typename = typename std::enable_if<is_asn1_wrapper<S>::value>::type
    bool operator!=(const Seq<T> &lhs, const S<T> &rhs) {
        return !(lhs == rhs);
    }
}

#endif
