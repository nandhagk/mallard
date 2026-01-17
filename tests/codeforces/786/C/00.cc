#include <bits/stdc++.h>

#include "lib/algebra/sum.h"
#include "lib/dynamic_segment_tree.h"
#include "lib/managers/clonable.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x, --x;

    static std::array<std::byte, (1 << 28) - (1 << 24)> buf;
    std::pmr::monotonic_buffer_resource mbr{buf.data(), buf.size(),
                                            std::pmr::null_memory_resource()};
    std::pmr::polymorphic_allocator pa{&mbr};

    constexpr u32 UNSET = -1;
    std::vector<u32> prv(n, UNSET), nxt(n, n);

    std::vector<u32> b(n);
    for (u32 i = 0; i < n; ++i) {
        if (prv[a[i]] == UNSET)
            ++b[i];
        else
            nxt[prv[a[i]]] = i;
        prv[a[i]] = i;
    }

    using tree = mld::dynamic_segment_tree<mld::algebra::sum<u32>,
                                           mld::pmr::managers::clonable<>>;

    std::vector<tree> st;
    st.reserve(n + 1);

    st.emplace_back(b, pa);
    for (u32 i = 0; i < n; ++i) {
        st.emplace_back(st.back());
        if (nxt[i] != n) st.back().set(nxt[i], 1);
    }

    for (u32 k = 1; k <= n; ++k) {
        u32 ans = 0;
        for (u32 l = 0; l < n; ++ans)
            l = st[l].max_right(l, [&](auto &&x) { return x <= k; });

        std::cout << ans << ' ';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
