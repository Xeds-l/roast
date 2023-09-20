#pragma once
#ifndef XED_BASIC_STRING_VIEW_HPP
#define XED_BASIC_STRING_VIEW_HPP 1
#include <cstdint>
#include "has_attributes.hpp"
namespace xed {
template<typename CharType>
class basic_string_view_reverse_iterator {
public:
    using this_type = basic_string_view_reverse_iterator;
    using value_type = CharType;
    using pointer_type = value_type*;
    using const_pointer_type = const value_type*;

    using reference_type = value_type&;
    using const_reference_type = const value_type&;


    constexpr basic_string_view_reverse_iterator() noexcept = default;
    constexpr basic_string_view_reverse_iterator(pointer_type ptr) noexcept : ptr_(ptr) {}
    constexpr inline NODISCARD this_type operator+(this_type ptr) const noexcept { return ptr_ - ptr; }
    constexpr inline NODISCARD this_type operator-(this_type ptr) const noexcept { return ptr_ + ptr; }
    constexpr inline NODISCARD this_type operator*(this_type ptr) const noexcept { return ptr_ * ptr; }
    constexpr inline NODISCARD this_type operator/(this_type ptr) const noexcept { return ptr_ / ptr; }
    constexpr inline NODISCARD this_type operator%(this_type ptr) const noexcept { return ptr_ % ptr; }

    constexpr inline this_type& operator+=(this_type ptr) noexcept { ptr_ -= ptr; return *this; }
    constexpr inline this_type& operator-=(this_type ptr) noexcept { ptr_ += ptr; return *this; }
    constexpr inline this_type& operator*=(this_type ptr) noexcept { ptr_ *= ptr; return *this; }
    constexpr inline this_type& operator/=(this_type ptr) noexcept { ptr_ /= ptr; return *this; }
    constexpr inline this_type& operator%=(this_type ptr) noexcept { ptr_ %= ptr; return *this; }

    this_type operator++(int) noexcept { return ptr_--; }
    this_type operator++() noexcept { return --ptr_; }

    this_type operator--(int) noexcept { return ptr_++; }
    this_type operator--() noexcept { return ++ptr_; }

    constexpr inline NODISCARD bool operator==(this_type other) noexcept {
        return ptr_ == other.ptr_;
    }
    constexpr inline NODISCARD bool operator!=(this_type other) noexcept {
        return ptr_ != other.ptr_;
    }
    constexpr inline NODISCARD bool operator<(this_type other) noexcept {
        return ptr_ < other.ptr_;
    }
    constexpr inline NODISCARD bool operator>(this_type other) noexcept {
        return ptr_ > other.ptr_;
    }
    constexpr inline NODISCARD bool operator<=(this_type other) noexcept {
        return ptr_ <= other.ptr_;
    }
    constexpr inline NODISCARD bool operator>=(this_type other) noexcept {
        return ptr_ >= other.ptr_;
    }

    constexpr inline NODISCARD const_pointer_type operator->() const noexcept {
        return ptr_;
    }

    constexpr inline NODISCARD const_reference_type operator*() const noexcept {
        return *ptr_;
    }

    constexpr inline NODISCARD pointer_type operator->() noexcept {
        return ptr_;
    }

    constexpr inline NODISCARD reference_type operator*() noexcept {
        return *ptr_;
    }
private:
    value_type* ptr_;
};



template<typename CharType>
class basic_string_view {
public:
    using this_type = basic_string_view<CharType>;
    using size_type = std::size_t;
    using value_type = CharType;
    using pointer_type = value_type*;
    using reference_type = value_type&;
    using const_reference_type = const value_type&;
    using const_pointer_type = const value_type* const;
    using const_iterator = const value_type*;
    using const_reverse_iterator = basic_string_view_reverse_iterator<const value_type>;

public:

    constexpr basic_string_view(value_type ch) noexcept
        : data_(&ch)
        , length_(1) {
    }

    basic_string_view(std::nullptr_t) = delete;
    basic_string_view(std::nullptr_t,size_type) = delete;


    constexpr basic_string_view(const_pointer_type str) noexcept
        : data_(str)
        , length_(std::strlen(str)) {
    }

    constexpr basic_string_view(const_pointer_type str, size_type length) noexcept
        : data_(str)
        , length_(length) {
    }


    constexpr basic_string_view(const basic_string_view&) noexcept = default;
    constexpr basic_string_view(basic_string_view&&) noexcept = default;

    constexpr basic_string_view& operator=(const basic_string_view&) noexcept = default;
    constexpr basic_string_view& operator=(basic_string_view&&) noexcept = default;


    constexpr inline [[nodiscard]] auto length() const noexcept { return length_; }
    constexpr inline [[nodiscard]] auto data() const noexcept   { return data_; }
    
    constexpr inline const_reference_type at(size_type index) const {
        if (index >= length_)
            throw std::out_of_range("from basic_string_view<>::at method.");


        return data_[index];
    }

    constexpr const_reference_type back() const noexcept {
        return data_[0];
    }

    constexpr const_reference_type front() const noexcept {
        return data_[length_ - 1];
    }

    constexpr const_reference_type operator[](size_type index) const noexcept {
        return data_[index];
    }


    inline NODISCARD const_iterator begin() const noexcept { return data_ + 0; }
    inline NODISCARD const_iterator end() const noexcept { return data_ + length_ + 1; }
    inline NODISCARD const_reverse_iterator rbegin() const noexcept { return (data_ + length_) - 1; }
    inline NODISCARD const_reverse_iterator rend() const noexcept { return data_ - 1; }
    inline NODISCARD const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    inline NODISCARD const_reverse_iterator crend() const noexcept { return rend(); }
    inline NODISCARD const_iterator cbegin() const noexcept { return begin(); }
    inline NODISCARD const_iterator cend() const noexcept { return end(); }
private:
    const_pointer_type data_;
    size_type length_;
};

using string_view    = basic_string_view<char>;
using wstring_view   = basic_string_view<wchar_t>;
using u8string_view  = basic_string_view<char8_t>;
using u16string_view = basic_string_view<char16_t>;
using u32string_view = basic_string_view<char32_t>;

INLINE_NAMESPACE namespace literals {
INLINE_NAMESPACE namespace string_view_literals {

inline NODISCARD string_view operator""_sv(const char* str, std::size_t len) {
    return { str,len };
}

inline NODISCARD wstring_view operator""_sv(const wchar_t* str, std::size_t len) {
    return { str,len };
}

inline NODISCARD u8string_view operator""_sv(const char8_t* str, std::size_t len) {
    return { str,len };
}

inline NODISCARD u16string_view operator""_sv(const char16_t* str, std::size_t len) {
    return { str,len };
}

inline NODISCARD u32string_view operator""_sv(const char32_t* str, std::size_t len) {
    return { str,len };
}

} // namespace string_literals 
} // namespace literals
} // namespace xed

#include "undef.hpp"
#endif // !BASIC_STRING_VIEW_HPP