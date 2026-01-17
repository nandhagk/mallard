#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/scc.h"
#include "lib/static_csr.h"
#include "lib/strong_connectivity_augmentation.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::pair<u32, u32>> e, f;
    f.reserve(n), e.reserve(n);

    for (u32 u = 0; u < n; ++u) {
        u32 v;
        std::cin >> v;

        e.emplace_back(u, --v);
    }

    mld::static_csr g(n, e);
    auto [k, ids] = mld::scc(g);

    std::vector<u32> rid(k);
    for (u32 u = 0; u < n; ++u) rid[ids[u]] = u;

    for (auto &&[u, v] : e)
        if (ids[u] != ids[v]) f.emplace_back(ids[u], ids[v]);

    std::ranges::sort(f);
    f.erase(std::unique(f.begin(), f.end()), f.end());

    mld::static_csr h(k, f);
    auto z = mld::strong_connectivity_augmentation(h);

    std::cout << z.size() << '\n';
    for (auto &&[u, v] : z) std::cout << rid[u] + 1 << ' ' << rid[v] + 1 << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
