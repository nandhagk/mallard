#include <bits/stdc++.h>

#include "lib/actions/add_sum.h"
#include "lib/dual_fenwick_tree.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);

    u32 q;
    std::cin >> q;

    std::vector<std::pair<u32, std::pair<u32, u32>>> f;
    f.reserve(q);

    for (u32 i = 0; i < q; ++i) {
        u32 u, d, x;
        std::cin >> u >> d >> x;

        f.emplace_back(--u, std::pair<u32, u32>{d, x});
    }

    mld::static_csr z(n, f);

    mld::dual_fenwick_tree<mld::actions::add_sum<u64>> st(
        std::views::iota(u32{0}, n) | std::views::transform([](auto) { return 0; }));

    std::vector<u64> ans(n);
    auto dfs = [&](auto &&self, u32 h, u32 u, u32 t) -> void {
        for (auto &&[d, x] : z[u]) st.apply(h, std::min(n, h + d + 1), x);

        for (u32 v : g[u])
            if (v != t) self(self, h + 1, v, u);

        ans[u] = st.get(h).val();
        for (auto &&[d, x] : z[u]) st.apply(h, std::min(n, h + d + 1), -u64(x));
    };

    dfs(dfs, 0, 0, 0);
    for (auto &&x : ans) std::cout << x << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
