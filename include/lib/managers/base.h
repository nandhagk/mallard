#ifndef MALLARD_MANAGERS_BASE_H
#define MALLARD_MANAGERS_BASE_H 1

#include <memory>

namespace mld::managers {

template <typename Allocator>
struct base {
    template <typename NodeT>
    struct handler {
        using allocator_type =
            std::allocator_traits<Allocator>::template rebind_alloc<NodeT>;
        using allocator_traits = std::allocator_traits<allocator_type>;

    protected:
        [[no_unique_address]] allocator_type allocator;

    public:
        constexpr handler(const allocator_type &alloc = allocator_type()) noexcept
            : allocator(alloc) {}

        // TODO: Figure out why pmr does not copy
        // constexpr handler(const handler &other) noexcept
        //     : allocator(allocator_traits::select_on_container_copy_construction(
        //           other.allocator)) {}

        constexpr handler(const handler &other) noexcept = default;
        constexpr handler(handler &&other) noexcept = default;

        constexpr auto &operator=(const handler &other) noexcept {
            if (this != &other) {
                if constexpr (allocator_traits::propagate_on_container_copy_assignment::
                                  value) {
                    allocator = other.allocator;
                }
            }
            return *this;
        }

        constexpr auto &operator=(handler &&other) noexcept {
            if (this != &other) {
                if constexpr (allocator_traits::propagate_on_container_move_assignment::
                                  value) {
                    allocator = other.allocator;
                }
            }
            return *this;
        }
    };
};
} // namespace mld::managers

#endif // MALLARD_MANAGERS_BASE_H
