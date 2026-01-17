#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/ctd.h"
#include "lib/static_csr.h"
#include "lib/rooted_tree_lca.h"

void solve() {
    u32 n, d;
    std::cin >> n >> d;

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

    mld::rooted_tree_lca h(g);
    auto [_, par] = mld::ctd(g);

    std::vector<u32> best(n, n);

    std::vector<u32> out;
    out.reserve(n);

    constexpr u32 UNSET = -1;

    std::vector<u32> ord(n);
    std::iota(ord.begin(), ord.end(), 0);
    std::ranges::sort(ord, [&](u32 u, u32 v) {
        return h.dep[u] == h.dep[v] ? u < v : h.dep[u] > h.dep[v];
    });

    for (u32 u : ord) {
        u32 z = best[u];
        for (u32 v = par[u]; v != UNSET; v = par[v])
            z = std::min(z, best[v] + h.dist(u, v));

        if (z < d) continue;

        best[u] = 0;
        for (u32 v = par[u]; v != UNSET; v = par[v])
            best[v] = std::min(best[v], best[u] + h.dist(u, v));

        out.push_back(u);
    }

    std::cout << out.size() << '\n';
    for (u32 u : out) std::cout << u + 1 << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
