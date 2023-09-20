#pragma once
#include "type_traits.hpp"
#include "move.hpp"
namespace xed {
template<typename T1,typename T2>
T1 exchange(T1& exchange_with,const T2& value) noexcept {
    T1 old_value = move(exchange_with);
    exchange_with = value;
    return old_value;
}

} // namespace xed