#pragma once

#if __has_include(<memory_resource>)
#include <memory_resource>
#include <vector>
#include <deque>
#include <string>

namespace platform {
namespace pmr = std::pmr;
} // namespace platform

#elif __has_include(<experimental/memory_resource>)
#include <experimental/memory_resource>
#include <experimental/vector>
#include <experimental/deque>
#include <experimental/string>

namespace platform {
namespace pmr = std::experimental::pmr;
} // namespace platform

#endif
