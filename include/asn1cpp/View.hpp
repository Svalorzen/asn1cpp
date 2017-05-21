#ifndef ASN1CPP_VIEW_HEADER_FILE
#define ASN1CPP_VIEW_HEADER_FILE

#include <stdexcept>

#include "asn_application.h"

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
            View(const View & other);
            template <template <typename> class S, typename = typename std::enable_if<is_asn1_wrapper<S<T>>::value>::type>
            View(const S<T> & other);

            View & operator=(Seq<T> other);
            View & operator=(const View & other);

            T & operator*();
            const T & operator*() const;
            T * operator->();
            const T * operator->() const;

            operator bool() const;

            asn_TYPE_descriptor_t * getTypeDescriptor() const;

            template <template <typename> class S, typename = typename std::enable_if<is_asn1_wrapper<S<T>>::value>::type>
            static void swap(View & lhs, S<T> & rhs);

            template <template <typename> class S, typename = typename std::enable_if<is_asn1_wrapper<S<T>>::value>::type>
            static void swap(S<T> & lhs, View & rhs);
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
    View<T> & View<T>::operator=(Seq<T> other) {
        swap(*this, other);
        return *this;
    }

    template <typename T>
    template <template <typename> class S, typename>
    void View<T>::swap(View & lhs, S<T> & rhs) {
        if (!lhs && !rhs) return;
        if (!(lhs && rhs))
            throw std::runtime_error("Cannot swap null pointers!");
        T tmp = *rhs;
        *rhs = *lhs;
        *lhs = tmp;
    }
    template <typename T>
    template <template <typename> class S, typename>
    void View<T>::swap(S<T> & lhs, View & rhs) {
        swap(rhs, lhs);
    }

    template <typename T>
    View<T> makeView(asn_TYPE_descriptor_t * def, T & seq) {
        return View<T>(def, &seq);
    }
}

#define makeView(m, T) \
    makeView<T>(&ASN1CPP_ASN1C_DEF(T), m)

#endif
