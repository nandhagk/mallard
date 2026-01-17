#include <bits/stdc++.h>

#include "lib/algebra/mxprefsuff.h"
#include "lib/dynamic_segment_tree.h"
#include "lib/managers/clonable.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x, --x;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);
    for (auto &&x : a) e.emplace_back(x, e.size());
    mld::static_csr g(n, e);

    static std::array<std::byte, (1 << 28) - (1 << 25)> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};
    std::pmr::polymorphic_allocator pa{&mbr};

    using operand = mld::algebra::mxprefsuff<i32>;
    using tree = mld::dynamic_segment_tree<operand, mld::pmr::managers::clonable<>>;

    std::vector<tree> st;
    st.reserve(n + 1);
    st.emplace_back(std::views::iota(u32{0}, n) |
                        std::views::transform([](auto) { return operand(-1); }),
                    pa);

    for (u32 x = n; x--;) {
        st.emplace_back(st.back());
        for (u32 k : g[x]) st.back().set(k, operand(1));
    }

    u32 ans = 0;
    for (u32 i = 0; i < n; ++i) {
        u32 lo = 0, hi = n + 1;
        while (hi - lo > 1) {
            u32 z = std::midpoint(lo, hi);

            i32 u = std::get<2>(st[z].fold(0, i + 1).val());
            i32 v = std::get<1>(st[z].fold(i, n).val());
            i32 y = u + v + (a[i] < n - z ? 1 : -1);

            if (y < 0)
                lo = z;
            else
                hi = z;
        }

        assert(n - hi >= a[i]);
        ans = std::max(ans, n - hi - a[i]);
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
