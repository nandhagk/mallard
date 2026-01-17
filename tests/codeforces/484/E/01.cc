#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/base.h"
#include "lib/dynamic_segment_tree.h"
#include "lib/managers/clonable.h"
#include "lib/prelude.h"

struct fence : mld::algebra::base<std::tuple<u32, u32, u32, u32>>,
               mld::algebra::associative {
    using operand = mld::algebra::base<std::tuple<u32, u32, u32, u32>>;

    using operand::operand;
    using operand::val;

    fence(bool)
        : fence(1, 1, 1, 1) {}

    [[nodiscard]] friend fence operator+(const fence &lhs, const fence &rhs) noexcept {
        auto &&[a, b, c, d] = lhs.val();
        auto &&[p, q, r, s] = rhs.val();
        return {a + p, b + (b == a) * q, r + (r == p) * c, std::max({d, s, c + q})};
    }
};

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::vector<u32> ord(n);
    std::iota(ord.begin(), ord.end(), 0);
    std::ranges::sort(ord, std::greater{}, [&](u32 i) { return std::tie(a[i], i); });

    static std::array<std::byte, (1 << 28) - (1 << 25)> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};
    std::pmr::polymorphic_allocator pa{&mbr};

    using tree = mld::dynamic_segment_tree<fence, mld::pmr::managers::clonable<>>;

    std::vector<tree> st;
    st.reserve(n + 1);
    st.emplace_back(std::views::iota(u32{0}, n) |
                        std::views::transform([](auto) { return fence(1, 0, 0, 0); }),
                    pa);

    for (u32 k : ord) {
        st.emplace_back(st.back());
        st.back().set(k, fence(1));
    }

    u32 m;
    std::cin >> m;

    while (m--) {
        u32 l, r, w;
        std::cin >> l >> r >> w;

        u32 lo = 0, hi = n + 1;
        for (--l; hi - lo > 1;) {
            u32 z = std::midpoint(lo, hi);
            if (std::get<3>(st[z].fold(l, r).val()) >= w)
                hi = z;
            else
                lo = z;
        }

        std::cout << a[ord[lo]] << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
