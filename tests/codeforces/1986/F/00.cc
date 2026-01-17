#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/becc.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);
    }

    mld::static_csr g(n, e);
    auto [k, ids] = mld::becc(g);

    std::vector<u32> cnt(k);
    for (u32 u = 0; u < n; ++u) ++cnt[ids[u]];

    std::vector<std::pair<u32, u32>> f;
    f.reserve(2 * n);

    for (auto &&[u, v] : e)
        if (ids[u] != ids[v]) f.emplace_back(ids[u], ids[v]);

    mld::static_csr h(k, f);
    mld::rooted_tree r(h);

    for (u32 u : r.tor | std::views::drop(1) | std::views::reverse)
        cnt[r.par[u]] += cnt[u];

    std::vector<u64> ans(k);
    for (u32 u = 0; u < k; ++u) ans[u] = u64(cnt[u]) * (n - cnt[u]);

    std::cout << u64(n) * (n - 1) / 2 - *std::ranges::max_element(ans) << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
