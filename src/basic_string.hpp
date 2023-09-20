#pragma once
#ifndef XED_BASIC_STRING_HPP
#define XED_BASIC_STRING_HPP 1

#include <cstdint>
#include <cstring>
#include "xutility.hpp"
#include "string_view.hpp"
#include "has_attributes.hpp"
#include <iosfwd>

static constexpr std::size_t npos = -1;
namespace xed {
namespace details {
template<typename CharType>
class basic_string_reverse_iterator {
public:
    using this_type = basic_string_reverse_iterator;
    using value_type = CharType;
    using pointer_type = value_type*;
    using const_pointer_type = const value_type*;

    using reference_type = value_type&;
    using const_reference_type = const value_type&;


    basic_string_reverse_iterator() noexcept = default;
    basic_string_reverse_iterator(pointer_type ptr) noexcept : ptr_(ptr) {}
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
} // namespace details
template<typename CharType>
class basic_string {
public:
    using this_type = basic_string;
    using size_type = std::size_t;
    using value_type = CharType;
    using char_type = value_type;
    using pointer_type = value_type*;
    using reference_type = value_type&;
    using string_view = basic_string_view<value_type>;
    using const_pointer_type = const value_type*;
    using const_reference_type = const value_type&;
    using iterator = pointer_type;
    using const_iterator = const pointer_type;
    using reverse_iterator = details::basic_string_reverse_iterator<char_type>;
    using const_reverse_iterator = details::basic_string_reverse_iterator<const char_type>;
public:
    basic_string()
        : capacity_(1)
        , length_(0)
        , data_(new value_type('\0')) {
    }

    basic_string(const_pointer_type string, size_t length)
        : capacity_(length + 1)
        , length_(length)
        , data_(new value_type[capacity_]) {
        std::memcpy(data_, string, sizeof(value_type) * (length_ + 1));
    }

    basic_string(pointer_type string, size_t length,std::nullptr_t)
        : capacity_(length + 1)
        , length_(length)
        , data_(string) {
    }

    basic_string(const_pointer_type string)
        : capacity_(std::strlen(string) + 1)
        , length_(capacity_ - 1)
        , data_(new value_type[capacity_]) {
        std::memcpy(data_, string, sizeof(value_type) * (length_ + 1));
    }

    basic_string(const basic_string& other)
        : capacity_(other.capacity_)
        , length_(other.length_) {
        delete[] data_;
        data_ = new value_type[other.capacity_];
        std::memcpy(data_, other.data_, sizeof(value_type) * (length_ + 1));
    }

    basic_string(basic_string&& other) noexcept
        : capacity_(other.capacity_)
        , length_(other.length_)
        , data_(exchange(other.data_, nullptr)) {
    }

    basic_string& operator=(const basic_string& other) {
        basic_string temp(other);
        this->swap(temp);
        return *this;
    }

    basic_string& operator=(basic_string&& other) noexcept {
        basic_string temp(move(other));
        this->swap(temp);
        return *this;
    }

    ~basic_string() noexcept {
        delete[] data_;
    }

    void swap(basic_string& other) noexcept {
        const auto data_swapper = other.data_;
        const auto length_swapper = other.length_;
        const auto capacity_swapper = other.capacity_;

        other.data_ = data_;
        other.length_ = length_;
        other.capacity_ = capacity_;


        data_ = data_swapper;
        length_ = length_swapper;
        capacity_ = capacity_swapper;

    }

    reference_type operator[](size_type index) noexcept {
        return data_[index];
    }

    const reference_type operator[](size_type index) const noexcept {
        return data_[index];
    }

    inline NODISCARD operator string_view() const noexcept { 
        return { data_ ,length_ };
    }

    inline NODISCARD bool operator==(const_pointer_type other) const noexcept {
        return
            data_[0] == other[0]
            && std::strncmp(data_, other.data(), sizeof(value_type) * length_) == 0;
    }

    inline NODISCARD bool operator==(string_view other) const noexcept {
        return
            length_ == other.length()
            && data_[0] == other[0]
            && std::memcmp(data_, other.data(), sizeof(value_type) * length_) == 0;
    }

    inline NODISCARD bool operator<(string_view other) const noexcept {
        return std::memcmp(data_, other.data(), sizeof(value_type) * length_) == 1;
    }

    inline NODISCARD bool operator>(string_view other) const noexcept {
        return std::memcmp(data_, other.data(), sizeof(value_type) * length_) == -1;
    }

    inline NODISCARD bool operator!=(const_pointer_type other) const noexcept {
        return !(*this == other);
    }

    inline NODISCARD bool operator!=(string_view other) const noexcept {
        return !(*this == other);
    }

    inline NODISCARD bool operator<(const_pointer_type other) const noexcept {
        return std::strncmp(data_, other.data(), sizeof(value_type) * length_) == 1;
    }

    inline NODISCARD bool operator>(const_pointer_type other) const noexcept {
        return std::strncmp(data_, other.data(), sizeof(value_type) * length_) == -1;
    }

    inline NODISCARD bool operator<=(const_pointer_type other) const noexcept {
        return !(*this > other);
    }
    inline NODISCARD bool operator>=(const_pointer_type other) const noexcept {
        return !(*this < other);
    }

    inline NODISCARD bool operator<=(string_view other) const noexcept {
        return !(*this > other);
    }
    inline NODISCARD bool operator>=(string_view other) const noexcept {
        return !(*this < other);
    }

    bool operator==(std::nullptr_t) const = delete;
    bool operator!=(std::nullptr_t) const = delete;
    bool operator>(std::nullptr_t) const = delete;
    bool operator<(std::nullptr_t) const = delete;
    bool operator<=(std::nullptr_t) const = delete;
    bool operator>=(std::nullptr_t) const = delete;

    inline NODISCARD basic_string operator+(string_view other) const {
        const auto sum = length_ + other.length();
        pointer_type ret_string = new value_type[sum];

        std::memcpy(ret_string, data_, length_ * sizeof(value_type));

        std::memcpy(ret_string + length_, other.data(), other.length() * sizeof(value_type));

        ret_string[sum] = '\0';
        return { ret_string , sum };
    }

    inline basic_string& operator+=(string_view other) {
        const auto sum = length_ + other.length();

        if (sum >= capacity_) {
            reallocate(calc_growth(sum));
        }

        std::memcpy(data_ + length_, other.data(), other.length() * sizeof(value_type));

        data_[sum] = '\0';
        return *this;
    }

    inline NODISCARD size_type length()   const noexcept { return length_; }
    inline NODISCARD size_type capacity() const noexcept { return capacity_; }
    inline NODISCARD size_type max_size() const noexcept { return INT_LEAST32_MAX; }
    inline NODISCARD pointer_type data()  const noexcept { return data_; }
    inline NODISCARD size_type find(string_view str, size_type from_index = 0) const noexcept {
        const auto diff = length_ - str.length();
        if (length_ < str.length())
            return npos;

        for (size_type i = from_index; i < diff; i++) {
            if (std::memcmp(data_ + i, str.data(), sizeof(value_type) * str.length()) == 0) {
                return i;
            }
        }
        return npos;
    }

    inline NODISCARD size_type find(value_type ch, size_type from_index = 0) const noexcept {
        size_type length = length_ - 1;
        for (size_type i = from_index; i < length; i++) {
            if (data_[i] == ch) {
                return i;
            }
        }
        return npos;
    }

    inline NODISCARD bool is_empty() const noexcept { return length_ == 0; }

    inline basic_string& replace(string_view str, string_view with) {
        const auto pos = find(str);
        if (pos == npos)
            return *this;

        if (str.length() == with.length()) {
            std::memcpy(data_ + pos, with.data(), sizeof(value_type) * with.length());
            return *this;
        }

        if ((with.length() + length_) >= capacity_) {
            reallocate(calc_growth(std::max(with.length(),length_)));
        }

        const auto jump_pos = pos + with.length();


        if (str.length() > with.length()) {
            const auto diff = str.length() - with.length();
            erase(jump_pos, diff);

            std::memcpy(data_ + pos, with.data(), sizeof(value_type) * with.length());

        }
        else {
            const auto diff = with.length() - str.length();
            const auto offset = (length_ - diff) -1;

            for (auto i = length_; i != offset; i--)
                data_[i + diff] = data_[i];

            std::memcpy(data_ + pos, with.data(), sizeof(value_type) * with.length());

            length_ += (with.length() - str.length());
            data_[length_] = '\0';
        }
        return *this;
    }

    void insert(size_type offset, string_view str) {
        if (offset > length_)
            offset = length_;

        const auto sum = str.length() + length_;

        if (sum >= capacity_)
            reallocate(calc_growth(sum));


        //std::memmove(
        //    data_ + sum,
        //    data_ + (sum - str.length()),
        //    sizeof(value_type) * offset
        //);
        for (auto i = sum; i != offset; i--)
            data_[i] = data_[i - str.length()];

        for (auto i = offset; i != offset + str.length(); i++)
            data_[i] = str[i - offset];

        //std::memmove(
        //    data_ + offset,
        //    data_,
        //    sizeof(value_type) * (offset + str.length())
        //);
        length_ = sum;
        data_[length_] = '\0';
    }

    void erase(size_type offset, size_type amount = 1) {
        if (length_ == 0 || amount == 0)
            return;

        if (offset > length_)
            offset = length_;

        if (amount > ((length_ - offset) + 1))
            amount = (length_ - offset);

        for (auto i = offset; i < length_; i++)
            data_[i] = data_[i + amount];

        length_ -= amount;
        data_[length_] = '\0';
        if (length_ > (capacity_ / 2))
            reallocate(length_ + 1);
    }

    void clear() noexcept {
        length_ = 0;
    }

    basic_string substr(size_type index, size_type amount = -1) const {
        if (index >= length_)
            return { "" ,0 };

        if (amount > (length_ - index))
            amount = length_ - index;

        pointer_type ret_string = new value_type[amount + 1];

        std::memcpy(ret_string, data_ + index, sizeof(value_type) * amount + 1);

        return { ret_string,amount };
    }

    inline void reserve(size_type amount) { reallocate(amount); }

    inline NODISCARD iterator begin() noexcept { return data_ + 0; }
    inline NODISCARD iterator end() noexcept { return data_ + length_ + 1; }
    inline NODISCARD const_iterator begin() const noexcept { return data_ + 0; }
    inline NODISCARD const_iterator end() const noexcept { return data_ + length_ + 1; }
    inline NODISCARD reverse_iterator rbegin() noexcept { return (data_ + length_)-1; }
    inline NODISCARD reverse_iterator rend() noexcept { return data_ - 1; }

    inline NODISCARD const_reverse_iterator rbegin() const noexcept { return (data_ + length_) - 1; }
    inline NODISCARD const_reverse_iterator rend() const noexcept { return data_ - 1; }

    inline NODISCARD const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    inline NODISCARD const_reverse_iterator crend() const noexcept { return rend(); }

    inline NODISCARD const_iterator cbegin() const noexcept { return begin(); }
    inline NODISCARD const_iterator cend() const noexcept { return end(); }
    
    inline NODISCARD reference_type back() noexcept { return data_[0]; }
    inline NODISCARD reference_type front() noexcept { return data_[length_ - 1]; }
    inline NODISCARD const_reference_type back() const noexcept { return data_[0]; }
    inline NODISCARD const_reference_type front() const noexcept { return data_[length_ - 1]; }

    inline void push_front(string_view str) {
        insert(0, str);
    }
    inline void push_back(string_view str) {
        insert(-1, str);
    }

    inline void pop_front() {
        erase(-1, 1);
    }

    inline void pop_back() {
        erase(0, 1);
    }

    inline int compare(string_view str) const noexcept {
        return std::strncmp(data_,str.data(), length_);
    }
private:
    size_type capacity_;
    size_type length_;
    pointer_type data_;
private:
    inline void reallocate(size_type new_capacity) {
        // i use the arg insead of member so when new throws the capacity stays the same!
        pointer_type buffer = new value_type[new_capacity];
        capacity_ = new_capacity;

        std::memcpy(buffer, data_, sizeof(value_type) * (length_ + 1));

        delete[] data_;

        data_ = buffer;

    }
    constexpr inline NODISCARD size_type calc_growth(size_type extra_size = 0) const noexcept {
        return ((capacity_ * 3) / 2) + extra_size;
    }

};

template<typename CharType>
std::ostream& operator<<(std::ostream& o, const xed::basic_string<CharType>& s) {
    for (const auto c : s)
        o << c;
    return o;
}

using string    = basic_string<char>;
using wstring   = basic_string<wchar_t>;
using u8string  = basic_string<char8_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;

string to_lowercase(const string& str) {
    constexpr static char CHAR_TO_LOWERCASE_LOOKUP_TABLE[256] = { -128,-127,-126,-125,-124,-123,-122,-121,-120,-119,-118,-117,-116,-115,-114,-113,-112,-111,-110,-109,-108,-107,-106,-105,-104,-103,-102,-101,-100,-99,-98,-97,-96,-95,-94,-93,-92,-91,-90,-89,-88,-87,-86,-85,-84,-83,-82,-81,-80,-79,-78,-77,-76,-75,-74,-73,-72,-71,-70,-69,-68,-67,-66,-65,-64,-63,-62,-61,-60,-59,-58,-57,-56,-55,-54,-53,-52,-51,-50,-49,-48,-47,-46,-45,-44,-43,-42,-41,-40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-25,-24,-23,-22,-21,-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127 };

    const auto len = str.length();
    char* data = new char[len+1];
    for (auto i = 0ULL; i < len; i++) {
        data[i] = CHAR_TO_LOWERCASE_LOOKUP_TABLE[str[i] + 128];
   }

    return { data,len,nullptr };
}

string to_uppercase(const string& str) {
    constexpr static char CHAR_TO_UPPERCASE_LOOKUP_TABLE[256] = { -128,-127,-126,-125,-124,-123,-122,-121,-120,-119,-118,-117,-116,-115,-114,-113,-112,-111,-110,-109,-108,-107,-106,-105,-104,-103,-102,-101,-100,-99,-98,-97,-96,-95,-94,-93,-92,-91,-90,-89,-88,-87,-86,-85,-84,-83,-82,-81,-80,-79,-78,-77,-76,-75,-74,-73,-72,-71,-70,-69,-68,-67,-66,-65,-64,-63,-62,-61,-60,-59,-58,-57,-56,-55,-54,-53,-52,-51,-50,-49,-48,-47,-46,-45,-44,-43,-42,-41,-40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-25,-24,-23,-22,-21,-20,-19,-18,-17,-16,-15,-14,-13,-12,-11,-10,-9,-8,-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,123,124,125,126,127 };

    const auto len = str.length();
    char* data = new char[len + 1];
    for (auto i = 0ULL; i < len; i++) {
        data[i] = CHAR_TO_UPPERCASE_LOOKUP_TABLE[str[i] + 128];
    }

    return { data,len,nullptr };
}

INLINE_NAMESPACE namespace literals {
INLINE_NAMESPACE namespace string_literals {

inline NODISCARD string operator""_s(const char* str, std::size_t len) {
    return { str,len };
}

inline NODISCARD wstring operator""_s(const wchar_t* str, std::size_t len) {
    return { str,len };
}

inline NODISCARD u8string operator""_s(const char8_t* str, std::size_t len) {
    return { str,len };
}

inline NODISCARD u16string operator""_s(const char16_t* str, std::size_t len) {
    return { str,len };
}
inline NODISCARD u32string operator""_s(const char32_t* str, std::size_t len) {
    return { str,len };
}

} // namespace string_literals 
} // namespace literals
} // namespace xed

#include "undef.hpp"

#endif // !XED_BASIC_STRING_HPP
