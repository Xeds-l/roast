#pragma once
#include "type_traits.hpp"
#include "has_attributes.hpp"

namespace xed {

template<typename T>
constexpr inline NODISCARD remove_reference_t<T>&& move(T&& object) noexcept {
    return static_cast<remove_reference_t<T>&&>(object);
}

}

#include "undef.hpp"