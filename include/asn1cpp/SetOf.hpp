#ifndef ASN1CPP_SETOF_HEADER_FILE
#define ASN1CPP_SETOF_HEADER_FILE

#include "asn1cpp/Setter.hpp"
#include "asn1cpp/Getter.hpp"
#include "asn1cpp/Seq.hpp"
#include "asn1cpp/View.hpp"

namespace asn1cpp {
    namespace setof {
        namespace Impl {
            template <typename T>
            struct ArrayType {
                using type = typename std::remove_reference<decltype(**std::declval<T>().list.array)>::type;
            };
        }
        template <typename T>
        int size(const T& field) {
            return field.list.count;
        }

        template <typename R, typename T>
        R getterField(const T& field, int id, bool *ok = nullptr)
        {
            if (id < 0 || id > size(field)) {
                if (ok) *ok = false;
                return R();
            }
            bool iok;
            return asn1cpp::getterField<R, typename Impl::ArrayType<T>::type>(*field.list.array[id], ok ? ok : &iok);
        }

        template <typename T>
        Seq<typename Impl::ArrayType<T>::type> getterSeq(const T& field, asn_TYPE_descriptor_t * def, int id, bool *ok = nullptr) {
            using R = typename Impl::ArrayType<T>::type;
            if (id < 0 || id > size(field)) {
                if (ok) *ok = false;
                return Seq<R>();
            }
            bool iok;
            return getterSeq(field.list.array[id], def, ok ? ok : &iok);
        }

        template <typename T, typename V>
        bool setterField(T & field, const V & value, int id) {
            if (id < 0 || id > size(field))
                return false;
            return setterField(field.list.array[id], value);
        }

        template <typename T, typename V>
        bool adderElement(T & field, const V & value) {
            typename Impl::ArrayType<T>::type * ptr = nullptr;
            if (!asn1cpp::setterField(ptr, value))
                return false;
            return asn_set_add(&field, ptr);
        }

        template <typename T>
        bool removerElement(T & field, int id, asn_TYPE_descriptor_t * def) {
            if (id < 0 || id > size(field))
                return false;

            auto p = field.list.array[id];
            asn_set_del(&field, id, 0);
            def->free_struct(def, p, 0);
            return true;
        }

        template <typename T>
        void clearerField(T & field, asn_TYPE_descriptor_t * def) {
            for (int i = 0; i < size(field); ++i)
                def->free_struct(def, field.list.array[i], 0);
            field.list.count = 0;
        }
    }
}

#define pushList(field, value, ...) \
    adderElement(field, value, ## __VA_ARGS__)

#define popList(field, T, id, ...) \
    removerElement(field, id, &ASN1CPP_ASN1C_DEF(T), ## __VA_ARGS__)

#endif
