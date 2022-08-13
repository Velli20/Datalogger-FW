#pragma once

#if __has_include(<source_location>)
#include <source_location>

namespace platform {
namespace source_location = std::source_location;
} // namespace platform

#elif __has_include(<experimental/source_location>)
#include <experimental/source_location>

namespace platform {
namespace source_location = std::experimental::source_location;
} // namespace platform

#endif
