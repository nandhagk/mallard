#ifndef MALLARD_MANAGERS_REUSING_H
#define MALLARD_MANAGERS_REUSING_H 1

#include <concepts>
#include <memory>
#include <memory_resource>
#include <type_traits>

#include "lib/managers/base.h"
#include "lib/prelude.h"

namespace mld {
namespace managers {
template <typename Allocator = std::allocator<std::byte>>
struct reusing {
    template <typename NodeT>
    struct handler : base<Allocator>::template handler<NodeT> {
        using node = NodeT;
        using pointer = std::add_pointer_t<node>;

        using base = base<Allocator>::template handler<node>;

        using base::base;
        using typename base::allocator_traits;
        using typename base::allocator_type;

    private:
        using base::allocator;
        inline static usize instance_count = 0;

    public:
        inline static pointer nil;

        handler(const allocator_type &alloc = allocator_type()) noexcept
            : base(alloc) {
            if (instance_count++ == 0) nil = new node{};
        }

        ~handler() noexcept {
            if (--instance_count == 0) delete nil;
        }

        template <typename... Args>
            requires std::constructible_from<node, Args...>
        [[nodiscard]] constexpr auto create(Args &&...args) noexcept {
            pointer ptr = allocator_traits::allocate(allocator, 1);
            allocator_traits::construct(allocator, ptr, std::forward<Args>(args)...);
            return ptr;
        }

        [[nodiscard]] constexpr auto clone(pointer ptr) const noexcept {
            return ptr;
        }

        [[nodiscard]] constexpr bool is_disposable(pointer ptr) const noexcept {
            return ptr != nil;
        }

        constexpr void dispose(pointer ptr) noexcept {
            allocator_traits::destroy(allocator, ptr);
            allocator_traits::deallocate(allocator, ptr, 1);
        }
    };
};
} // namespace managers

namespace pmr::managers {

template <typename Allocator = std::pmr::polymorphic_allocator<std::byte>>
struct reusing {
    template <typename NodeT>
    struct handler : mld::managers::base<Allocator>::template handler<NodeT> {
        using node = NodeT;
        using pointer = std::add_pointer_t<node>;

        using base = mld::managers::base<Allocator>::template handler<node>;

        using base::base;
        using typename base::allocator_traits;
        using typename base::allocator_type;

    private:
        using base::allocator;
        inline static usize instance_count = 0;

    public:
        inline static pointer nil;

        handler(const allocator_type &alloc) noexcept
            : base(alloc) {
            if (instance_count++ == 0) nil = new node{};
        }

        ~handler() noexcept {
            if (--instance_count == 0) delete nil;
        }

        template <typename... Args>
            requires std::constructible_from<node, Args...>
        [[nodiscard]] constexpr auto create(Args &&...args) noexcept {
            pointer ptr = allocator_traits::allocate(allocator, 1);
            allocator_traits::construct(allocator, ptr, std::forward<Args>(args)...);
            return ptr;
        }

        [[nodiscard]] constexpr auto clone(pointer ptr) const noexcept {
            return ptr;
        }

        [[nodiscard]] constexpr bool is_disposable(pointer) const noexcept {
            return false;
        }

        constexpr void dispose(pointer) noexcept {}
    };
};
} // namespace pmr::managers
} // namespace mld

#endif // MALLARD_MANAGERS_REUSING_H
