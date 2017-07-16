#pragma once

#include <utility>

namespace Sdl {
namespace internal {
namespace utils {
template <class T>
auto id(T&& t)
{
    return [t = std::forward<T>(t)](auto&&...) {
        return t;
    };
}
}
}
}
