#pragma once
namespace xed {
template<typename T> struct remove_reference      { using type = T; };
template<typename T> struct remove_reference<T&>  { using type = T; };
template<typename T> struct remove_reference<T&&> { using type = T; };

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;


template<typename T,typename U> struct is_same { constexpr static bool value = false; };
template<typename T> struct is_same<T, T> { constexpr static bool value = true; };

template<typename T,typename U>
constexpr static bool is_same_v = is_same<T, U>::value;
} // namespace xed