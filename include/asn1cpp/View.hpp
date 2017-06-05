#ifndef ASN1CPP_VIEW_HEADER_FILE
#define ASN1CPP_VIEW_HEADER_FILE

#include <stdexcept>

#include "asn_application.h"

#include "asn1cpp/Utils.hpp"
#include "asn1cpp/Seq.hpp"

namespace asn1cpp {
    namespace Impl {
        template <typename T, typename Check>
        struct Setter;
    }
    template <typename T>
    class View {
        public:
            View(asn_TYPE_descriptor_t * def, T * p);
            View();
            View(View & other);
            View(View && other);

            template <template <typename> class S, typename Y,
                      typename = typename std::enable_if<are_compatible_asn1_wrappers<View<T>, S<Y>>::value>::type>
            View(S<Y> & other);

            template <template <typename> class S, typename Y,
                      typename = typename std::enable_if<are_compatible_asn1_wrappers<View<T>, S<Y>>::value>::type>
            View & operator=(S<Y> other);
            View & operator=(const View & other);

            T & operator*();
            const T & operator*() const;
            T * operator->();
            const T * operator->() const;

            operator bool() const;

            asn_TYPE_descriptor_t * getTypeDescriptor() const;

            template <template <typename> class S, typename Y,
                      typename = typename std::enable_if<are_compatible_asn1_wrappers<View<T>, S<Y>>::value>::type>
            static void swap(View & lhs, S<Y> & rhs);

            template <template <typename> class S, typename Y,
                      typename = typename std::enable_if<are_compatible_asn1_wrappers<View<T>, S<Y>>::value>::type>
            static void swap(S<Y> & lhs, View & rhs);
        private:
            View(T * p);

            template <typename Check>
            friend struct Impl::Setter;

            T * seq_;
            asn_TYPE_descriptor_t * def_;
    };

    template <typename T>
    View<T>::View(asn_TYPE_descriptor_t * def, T * p) :
            seq_(p), def_(def)
    {
        if (!seq_)
            throw std::runtime_error("Cannot build empty View!");
        if (!def_)
            throw std::runtime_error("Cannot build View with no ASN descriptors!");
    }

    template <typename T>
    View<T>::View(View & other) : View(other.def_, other.seq_) {}

    template <typename T>
    View<T>::View(View && other) : View(other.def_, other.seq_) {}

    template <typename T>
    template <template <typename> class S, typename Y, typename>
    View<T>::View(S<Y> & other) : View(other.getTypeDescriptor(), &(*other)) {}

    template <typename T>
    View<T>::View() : seq_(nullptr), def_(nullptr) {}

    template <typename T>
    View<T>::View(T * seq) : seq_(seq), def_(nullptr) {}

    template <typename T>
    asn_TYPE_descriptor_t * View<T>::getTypeDescriptor() const {
        return def_;
    }

    template <typename T>
    View<T>::operator bool() const {
        return seq_;
    }

    template <typename T>
    T & View<T>::operator*() {
        if (!seq_)
            throw std::runtime_error("Cannot dereference null pointer");
        return *seq_;
    }

    template <typename T>
    const T & View<T>::operator*() const {
        if (!seq_)
            throw std::runtime_error("Cannot dereference null pointer");
        return *seq_;
    }

    template <typename T>
    T * View<T>::operator->() {
        return seq_;
    }

    template <typename T>
    const T * View<T>::operator->() const {
        return seq_;
    }

    template <typename T>
    template <template <typename> class S, typename Y, typename>
    View<T> & View<T>::operator=(S<Y> other) {
        swap(*this, other);
        return *this;
    }

    template <typename T>
    View<T> & View<T>::operator=(const View & other) {
        Seq<T> copy = other;
        swap(*this, copy);
        return *this;
    }

    template <typename T>
    template <template <typename> class S, typename Y, typename>
    void View<T>::swap(View & lhs, S<Y> & rhs) {
        if (!lhs && !rhs) return;
        if (!(lhs && rhs))
            throw std::runtime_error("Cannot swap null pointers!");
        T tmp = *rhs;
        *rhs = *lhs;
        *lhs = tmp;
    }

    template <typename T>
    template <template <typename> class S, typename Y, typename>
    void View<T>::swap(S<Y> & lhs, View & rhs) {
        swap(rhs, lhs);
    }
}

#endif
