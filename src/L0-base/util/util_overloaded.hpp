#pragma once

namespace sys::util {

template <class... TypeSequence>
struct overloaded : TypeSequence...
{
    using TypeSequence::operator()...;
};

template <class... TypeSequence>
overloaded(TypeSequence...) -> overloaded<TypeSequence...>;

} // namespace sys::util
