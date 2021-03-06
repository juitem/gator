/* Copyright (C) 2018-2020 by Arm Limited. All rights reserved. */

#ifndef INCLUDE_LIB_SPAN_H
#define INCLUDE_LIB_SPAN_H

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace lib {
    /**
     * Array length pair
     */
    template<typename T, typename L = std::size_t>
    struct Span {
        using value_type = typename std::remove_cv<T>::type;
        using size_type = L;

        T * data;
        L length;

        L size() const { return length; }

        T & operator[](std::size_t pos) const
        {
            assert(pos < length);
            return data[pos];
        }

        bool operator==(const Span<T, L> & other) const { return std::equal(data, data + length, other.data); }

        Span() = default;

        /// convert Span<T> -> Span<const T>
        template<typename U,
                 typename M,
                 typename = typename std::enable_if<std::is_same<value_type, U>::value &&
                                                    std::is_convertible<M, L>::value>::type>
        Span(Span<U, M> other) : data {other.data}, length {other.length}
        {
        }

        Span(T * data, L length) : data {data}, length {length} {}

        template<typename C, //
                 typename = typename C::value_type,
                 typename = typename C::size_type, // make sure is a container
                 // make sure copy constructor is preferred to this
                 typename = typename std::enable_if<!std::is_same<typename std::remove_cv<C>::type, Span>::value>::type>
        Span(C & container) : data {container.data()}, length {container.size()}
        {
        }

        template<L Size>
        Span(T (&array)[Size]) : data {array}, length {Size}
        {
        }

        Span subspan(size_type offset) const
        {
            assert(offset <= length);
            return {data + offset, length - offset};
        }

        Span subspan(size_type offset, size_type count) const
        {
            assert(offset + count <= length);
            return {data + offset, count};
        }
    };

    template<typename T, typename L>
    T * begin(Span<T, L> span)
    {
        return span.data;
    }

    template<typename T, typename L>
    T * end(Span<T, L> span)
    {
        return span.data + span.length;
    }
}

#endif // INCLUDE_LIB_SPAN_H
