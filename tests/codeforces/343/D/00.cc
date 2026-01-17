#include <bits/stdc++.h>

#include "lib/actions/assign.h"
#include "lib/algebra/lxor.h"
#include "lib/dual_segment_tree.h"
#include "lib/hld.h"
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
    mld::hld h(g);

    mld::dual_segment_tree<mld::actions::assign<mld::algebra::lxor<bool>>> dt(
        std::views::iota(u32{0}, n) | std::views::transform([](auto) { return 0; }));

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 c, u;
        std::cin >> c >> u;

        --u;
        if (c == 1) {
            dt.apply(h.tin[u], h.tin[u] + h.siz[u], 1);
        } else if (c == 2) {
            for (auto &&[s, t] : h.decompose(u, 0)) dt.apply(h.tin[t], h.tin[s] + 1, 0);
        } else {
            std::cout << dt.get(h.tin[u]).val() << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
