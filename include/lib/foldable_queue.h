#ifndef MALLARD_FOLDABLE_QUEUE_H
#define MALLARD_FOLDABLE_QUEUE_H 1

#include <cassert>
#include <vector>

#include "lib/algebra/internal/concepts.h"

namespace mld {
template <algebra::internal::monoid Monoid>
struct foldable_queue {
    using operand = Monoid;

    using size_type = u32;
    using container_type = std::vector<operand>;

private:
    size_type len;
    operand ar;
    container_type vr, al{operand()};

public:
    constexpr foldable_queue() noexcept = default;

    explicit constexpr foldable_queue(size_type n) noexcept {
        build(n);
    }

    constexpr void build(size_type n) noexcept {
        clear();
        vr.reserve(n);
        al.reserve(n);
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return len == 0;
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return len;
    }

    constexpr void clear() noexcept {
        al = {operand()};
        vr.clear();
        ar = operand();
    }

    constexpr void push(const operand &x) noexcept {
        ++len;
        ar = ar + x;
        vr.push_back(x);
    }

    constexpr void pop() noexcept {
        assert(len--);

        al.pop_back();
        if (!al.empty()) return;

        al = {operand()};
        while (vr.size() > 1) {
            al.push_back(vr.back() + al.back());
            vr.pop_back();
        }

        vr.pop_back();
        ar = operand();
    }

    [[nodiscard]] constexpr operand fold() const noexcept {
        return al.back() + ar;
    }
};
} // namespace mld

#endif // MALLARD_FOLDABLE_QUEUE_H
