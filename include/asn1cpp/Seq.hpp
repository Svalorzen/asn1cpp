#ifndef ASN1CPP_SEQ_HEADER_FILE
#define ASN1CPP_SEQ_HEADER_FILE

#include <stdexcept>
#include <type_traits>

#include "asn_application.h"

#include "asn1cpp/Utils.hpp"

namespace asn1cpp {
    template <typename T>
    class Seq;

    namespace ber {
        template <typename T, typename = typename std::enable_if<is_asn1_wrapper<T>::value>::type>
        std::string encode(const T & m);

        template <typename T>
        Seq<T> decode(asn_TYPE_descriptor_t * def, const std::string & buffer);
    }

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

            Seq & operator=(Seq other);

            template <template <typename> class S, typename = typename std::enable_if<is_asn1_wrapper<S<T>>::value>::type>
            Seq(const S<T> & other);

            T & operator*();
            const T & operator*() const;
            T * operator->();
            const T * operator->() const;

            operator bool() const;

            asn_TYPE_descriptor_t * getTypeDescriptor() const;

        private:
            template <template <typename> class S, typename = typename std::enable_if<is_asn1_wrapper<S<T>>::value>::type>
            void deepCopy(const S<T> & other);

            T * seq_;
            asn_TYPE_descriptor_t * def_;
    };

    template <typename T>
    Seq<T>::Seq(asn_TYPE_descriptor_t * def, T * p) :
            seq_(p), def_(def)
    {
        if (seq_ && !def_)
            throw std::runtime_error("Cannot build non-empty Seq with no ASN descriptors!");
    }

    template <typename T>
    Seq<T>::Seq() : Seq(nullptr, nullptr) {}

    template <typename T>
    Seq<T>::Seq(asn_TYPE_descriptor_t * def) :
            seq_(new T()), def_(def) {}

    template <typename T>
    Seq<T>::~Seq() {
        if (seq_)
            def_->free_struct(def_, seq_, 0);
    }

    template <typename T>
    Seq<T>::Seq(const Seq & other)  {
        deepCopy(other);
    }

    template <typename T>
    template <template <typename> class S, typename>
    Seq<T>::Seq(const S<T> & other) {
        deepCopy(other);
    }

    template <typename T>
    Seq<T> & Seq<T>::operator=(Seq other) {
        std::swap(*this, other);
        return *this;
    }

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

    template <typename T, template <typename> class S, typename = typename std::enable_if<is_asn1_wrapper<S<T>>::value>::type>
    bool operator==(const Seq<T> &lhs, const S<T> &rhs) {
        return ber::encode(lhs) == ber::encode(rhs);
    }

    template <typename T, template <typename> class S, typename = typename std::enable_if<is_asn1_wrapper<S<T>>::value>::type>
    bool operator!=(const Seq<T> &lhs, const S<T> &rhs) {
        return !(lhs == rhs);
    }

    template <typename T>
    template <template <typename> class S, typename>
    void Seq<T>::deepCopy(const S<T> & other) {
        if (other.seq_ && other.def_) {
            *this = ber::decode<T>(other.def_, ber::encode(other));
        } else {
            seq_ = nullptr;
            def_ = other.def_;
        }
    }

    template <typename T>
    Seq<T> makeSeq(asn_TYPE_descriptor_t * def) {
        return Seq<T>(def);
    }
}

#define makeSeq(T) \
    makeSeq<T>(&ASN1CPP_ASN1C_DEF(T))

#endif
