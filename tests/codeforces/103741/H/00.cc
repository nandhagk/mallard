#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"
#include "lib/static_montgomery_modint.h"
#include "lib/topological_sort.h"

using Z = mld::montgomerymodint998244353;

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<u32> par(n, n);
    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        par[--u] = --v;
    }

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (u32 u = 0; u < n; ++u) e.emplace_back(par[u], u);
    mld::static_csr g(n + 1, e);

    if (!mld::topological_sort(g)) {
        std::cout << 0 << '\n';
        return;
    }

    mld::rooted_tree h(g, n);

    Z num = 1;
    for (u32 i = 1; i <= n; ++i) num *= i;

    Z den = 1;
    for (u32 u = 0; u < n; ++u) den *= h.siz[u];

    std::cout << (num / den).val() << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
