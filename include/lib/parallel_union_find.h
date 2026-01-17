#ifndef MALLARD_PARALLEL_UNION_FIND_H
#define MALLARD_PARALLEL_UNION_FIND_H 1

#include "lib/prelude.h"
#include "lib/union_find.h"

namespace mld {
struct parallel_union_find {
    using size_type = u32;
    using container_type = std::vector<union_find>;

private:
    size_type len, log;
    container_type buf;

public:
    constexpr parallel_union_find() noexcept = default;

    explicit constexpr parallel_union_find(size_type n) noexcept {
        build(n);
    }

    constexpr void build(size_type n) noexcept {
        len = n;
        log = std::max<size_type>(1, std::countr_zero(std::bit_ceil(len)));

        buf.resize(log);
        for (size_type i = 0; i < log; ++i) buf[i].build(len - (1 << i) + 1);
    }

    template <std::invocable<size_type, size_type> F>
    constexpr void merge(size_type l1, size_type l2, size_type x, F &&f) noexcept {
        assert(x > 0);
        if (x == 1) return merge_inner(0, l1, l2, std::forward<F>(f));

        size_type k = std::bit_width(x - 1) - 1;
        merge_inner(k, l1, l2, std::forward<F>(f));
        merge_inner(k, l1 + x - (1 << k), l2 + x - (1 << k), std::forward<F>(f));
    }

private:
    template <typename F>
    constexpr void merge_inner(size_type k, size_type l1, size_type l2,
                               F &&f) noexcept {
        if (k == 0) {
            auto &&uf = buf[0];

            size_type a = uf.find(l1), b = uf.find(l2);
            if (a == b) return;

            uf.merge(a, b);
            size_type c = uf.find(a);

            f(c, a ^ b ^ c);
            return;
        }

        if (!buf[k].merge(l1, l2)) return;

        merge_inner(k - 1, l1, l2, std::forward<F>(f));
        merge_inner(k - 1, l1 + (1 << (k - 1)), l2 + (1 << (k - 1)),
                    std::forward<F>(f));
    }
};
} // namespace mld

#endif // MALLARD_PARALLEL_UNION_FIND_H
