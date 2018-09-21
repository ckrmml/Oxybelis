#ifndef _OXYBELIS_MATH_VEC_H
#define _OXYBELIS_MATH_VEC_H

#include <cstddef>
#include <type_traits>

namespace detail {
    template <typename T, size_t N>
    struct SimdVecHelper {
        constexpr const static bool valid = false;
    };

    #define DEFINE_SIMD_VEC(type, size) \
        template <> \
        struct SimdVecHelper<type, (size)> { \
            constexpr const static bool valid = true; \
            using Type [[gnu::vector_size((size) * sizeof(type))]] = type; \
        }

    #define DEFINE_SIMD_VEC_COMMON_SIZES(type) \
        DEFINE_SIMD_VEC(type, 2); \
        DEFINE_SIMD_VEC(type, 3); \
        DEFINE_SIMD_VEC(type, 4)

    DEFINE_SIMD_VEC_COMMON_SIZES(float);
    DEFINE_SIMD_VEC_COMMON_SIZES(double);
    DEFINE_SIMD_VEC_COMMON_SIZES(int);

    template <typename T, size_t N>
    using SimdVec = typename SimdVecHelper<T, N>::Type;
}

template <typename T, size_t N>
struct Vec {
    detail::SimdVec<T, N> elements;

    constexpr const static size_t Rows = N;
    constexpr const static size_t Cols = 1;

    template <typename U, typename V, typename... Ts>
    constexpr Vec(const U& x, const V& y, const Ts&... elems):
        elements{static_cast<T>(x), static_cast<T>(y), static_cast<T>(elems)...} {
    }

    constexpr Vec(const detail::SimdVec<T, N>& elements):
        elements(elements) {
    }

    T operator[](size_t row) const {
        return this->elements[row];
    }

    template <typename U>
    Vec<T, N>& operator+=(const Vec<U, N>& other);

    template <typename U>
    Vec<T, N>& operator-=(const Vec<U, N>& other);

    template <typename U>
    Vec<T, N>& operator*=(const U& other);

    template <typename U>
    Vec<T, N>& operator/=(const U& other);

    auto length_sq() const;
    auto length() const;

    Vec<T, N>& normalize();

    template <typename U>
    Vec<T, 3>& cross(const Vec<U, 3>& other);
};

using Vec2F = Vec<float, 2>;
using Vec3F = Vec<float, 3>;
using Vec4F = Vec<float, 4>;

using Vec2D = Vec<double, 2>;
using Vec3D = Vec<double, 3>;
using Vec4D = Vec<double, 4>;

using Vec2I = Vec<int, 2>;
using Vec3I = Vec<int, 3>;
using Vec4I = Vec<int, 4>;

template <typename T, typename U, size_t N>
auto operator+(const Vec<T, N>& lhs, const Vec<U, N>& rhs) {
    using Result = decltype(std::declval<T>() + std::declval<U>());
    return Vec<Result, N>(lhs.elements + rhs.elements);
}

template <typename T, typename U, size_t N>
auto operator-(const Vec<T, N>& lhs, const Vec<U, N>& rhs) {
    using Result = decltype(std::declval<T>() - std::declval<U>());
    return Vec<Result, N>(lhs.elements - rhs.elements);
}

template <typename T, typename U, size_t N>
auto operator*(const Vec<T, N>& lhs, const Vec<U, N>& rhs) {
    using Result = decltype(std::declval<T>() * std::declval<U>());
    return Vec<Result, N>(lhs.elements * rhs.elements);
}

template <typename T, typename U, size_t N>
auto operator*(const Vec<T, N>& lhs, const U& rhs) {
    using Result = decltype(std::declval<T>() * std::declval<U>());
    return Vec<Result, N>(lhs.elements * rhs);
}

template <typename T, typename U, size_t N>
auto operator*(const T& lhs, const Vec<U, N>& rhs) {
    using Result = decltype(std::declval<T>() * std::declval<U>());
    return Vec<Result, N>(lhs * rhs.elements);
}

template <typename T, typename U, size_t N>
auto operator/(const Vec<T, N>& lhs, const Vec<U, N>& rhs) {
    using Result = decltype(std::declval<T>() / std::declval<U>());
    return Vec<Result, N>(lhs.elements / rhs.elements);
}

template <typename T, typename U, size_t N>
auto operator/(const Vec<T, N>& lhs, const U& rhs) {
    using Result = decltype(std::declval<T>() / std::declval<U>());
    return Vec<Result, N>(lhs.elements / rhs);
}

template <typename T, typename U, size_t N>
auto operator/(const T& lhs, const Vec<U, N>& rhs) {
    using Result = decltype(std::declval<T>() / std::declval<U>());
    return Vec<Result, N>(lhs / rhs.elements);
}

template <typename T, typename U, size_t N>
auto dot(const Vec<T, N>& lhs, const Vec<U, N>& rhs) {
    using Result = decltype(std::declval<T>() * std::declval<U>() + std::declval<T>() * std::declval<U>());
    auto r = lhs.elements * rhs.elements;
    Result total = r[0];

    for (size_t i = 1; i < N; ++i)
        total += r[i];

    return total;
}

template <typename T, typename U, size_t N>
auto distance_sq(const Vec<T, N>& lhs, const Vec<T, N>& rhs) {
    return (lhs - rhs).length_sq();
}

template <typename T, typename U, size_t N>
auto distance(const Vec<T, N>& lhs, const Vec<T, N>& rhs) {
    return (lhs - rhs).length();
}

template <typename T, size_t N>
auto normalize(const Vec<T, N>& v) {
    return v / v.length();
}

template <typename T, typename U>
auto cross(const Vec<T, 3>& lhs, const Vec<U, 3>& rhs) {
    using Result = decltype(std::declval<T>() * std::declval<U>() + std::declval<U>() * std::declval<T>());
    
    return Vec<Result, 3>(
        lhs[1] * rhs[2] - lhs[2] * rhs[1],
        lhs[2] * rhs[0] - lhs[0] * rhs[2],
        lhs[0] * rhs[1] - lhs[1] * rhs[0]
    );
}

template <typename Os, typename T, size_t N>
Os& operator<<(Os& os, const Vec<T, N>& v) {
    os << '(';
    for (size_t i = 0; i < N; ++i) {
        if (i != 0)
            os << ", ";
        os << v[i];
    }
    os << ')';
    return os;
}

template <typename T, size_t N>
template <typename U>
Vec<T, N>& Vec<T, N>::operator+=(const Vec<U, N>& other) {
    return *this = *this + other;
}

template <typename T, size_t N>
template <typename U>
Vec<T, N>& Vec<T, N>::operator-=(const Vec<U, N>& other) {
    return *this = *this - other;
}

template <typename T, size_t N>
template <typename U>
Vec<T, N>& Vec<T, N>::operator*=(const U& other) {
    return *this = *this * other;
}

template <typename T, size_t N>
template <typename U>
Vec<T, N>& Vec<T, N>::operator/=(const U& other) {
    return *this = *this / other;
}

template <typename T, size_t N>
auto Vec<T, N>::length_sq() const {
    return dot(*this, *this);
}

template <typename T, size_t N>
auto Vec<T, N>::length() const {
    return std::sqrt(this->length_sq());
}

template <typename T, size_t N>
Vec<T, N>& Vec<T, N>::normalize() {
    return *this = ::normalize(*this);
}

template <typename T, size_t N>
template <typename U>
Vec<T, 3>& Vec<T, N>::cross(const Vec<U, 3>& other) {
    static_assert(N == 3, "Can only perform cross on vectors of size 3");
    return *this = cross(*this, other);
}

#endif
