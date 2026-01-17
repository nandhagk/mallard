#ifndef MALLARD_MANAGERS_CONTAINER_H
#define MALLARD_MANAGERS_CONTAINER_H 1

#include "lib/prelude.h"

namespace mld::managers::container {
template <typename Container>
struct handler {
    using container_type = Container;

    using node = container_type::value_type;
    using pointer = container_type::pointer;

    inline static pointer nil;

private:
    inline static usize instance_count = 0;

public:
    constexpr handler() noexcept {
        if (instance_count++ == 0) nil = new node{};
    }

    constexpr ~handler() noexcept {
        if (--instance_count == 0) delete nil;
    }
};
} // namespace mld::managers::container

#endif // MALLARD_MANAGERS_CONTAINER_H
