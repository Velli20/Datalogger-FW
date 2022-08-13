namespace sys {

namespace detail {

struct unique_ptr_pmr_deleter
{
    inline void operator()(void* ptr)
    {
        allocator->deallocate(ptr, size, align);
    }

    platform::pmr::memory_resource* allocator{};
    std::size_t                     size{};
    std::size_t                     align{};
};

} // namespace detail

template <typename T>
using unique_ptr = std::unique_ptr<T, detail::unique_ptr_pmr_deleter>;

template <typename T>
[[nodiscard]] auto make_unique(auto&&... arguments)
{
    auto* allocator = platform::memory::default_resource();
    assert(allocator != nullptr);

    return unique_ptr<T>
    {
        new (allocator->allocate(sizeof(T), alignof(T)))
            T(std::forward<decltype(arguments)>(arguments)...),
                detail::unique_ptr_pmr_deleter{allocator, sizeof(T), alignof(T)}
    };
}

template <typename Type>
struct optional_ptr : sys::unique_ptr<Type>
{
    using base = sys::unique_ptr<Type>;

    using base::base;
    using base::operator=;

    constexpr optional_ptr(base&& other) noexcept
        : base(std::move(other))
    {}
};

} // namespace sys
