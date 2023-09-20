#pragma once
#include "has_attributes.hpp"

namespace xed {

struct false_type {
    constexpr static bool value = false;

    constexpr inline NODISCARD operator bool() const noexcept {
        return value;
    }
};

struct true_type {
    constexpr static bool value = true;

    constexpr inline NODISCARD operator bool() const noexcept{
        return value;
    }
};

template<typename T> struct remove_reference      { using type = T; };
template<typename T> struct remove_reference<T&>  { using type = T; };
template<typename T> struct remove_reference<T&&> { using type = T; };

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

template<typename T> struct remove_const          { using type = T};
template<typename T> struct remove_const<const T> { using type = T };

template<typename T>
using remove_const_t = remove_const<T>::type;

template<typename T> struct remove_volatile             { using type = T };
template<typename T> struct remove_volatile<volatile T> { using type = T };

template<typename T>
using remove_volatile_t = remove_volatile<T>::type;

template<typename T>
using remove_cv_t = remove_volatile_t<remove_const_t<T>>;

template<typename T, typename U> struct is_same : false_type {};
template<typename T> struct is_same<T, T>       : true_type {};

template<typename T,typename U>
constexpr static bool is_same_v = is_same<T, U>::value;
} // namespace xed


#include "undef.hpp"