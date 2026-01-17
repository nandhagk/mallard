#include <bits/stdc++.h>
#include <functional>
#include <limits>

#include "lib/prelude.h"
#include "lib/scc.h"
#include "lib/static_csr.h"
#include "lib/static_modint.h"

using Z = mld::modint1000000007;

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    u32 m;
    std::cin >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(--u, --v);
    }

    mld::static_csr g(n, e);
    auto [k, ids] = mld::scc(g);

    std::vector<u32> c(k, std::numeric_limits<u32>::max());
    for (u32 u = 0; u < n; ++u) c[ids[u]] = std::min(c[ids[u]], a[u]);

    std::vector<u32> d(k);
    for (u32 u = 0; u < n; ++u) d[ids[u]] += a[u] == c[ids[u]];

    std::cout << std::reduce(c.begin(), c.end(), u64{0}) << ' '
              << std::reduce(d.begin(), d.end(), Z(1), std::multiplies<>{}).val()
              << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
