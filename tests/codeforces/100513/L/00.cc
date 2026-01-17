#include <bits/stdc++.h>

#include "lib/dominator_tree.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"
#include "lib/union_find.h"

void solve(u32 n, u32 m) {
    std::vector<std::pair<u32, u32>> e(m);
    for (auto &&[u, v] : e) std::cin >> u >> v, --u, --v;

    mld::static_csr g(n, e);

    constexpr u32 UNSET = -1;

    auto par = mld::dominator_tree(g, 0);

    mld::union_find uf(n);
    for (u32 u = 0; u < n; ++u)
        if (par[u] == UNSET)
            par[u] = n;
        else
            uf.merge(u, par[u]);

    std::vector<std::pair<u32, u32>> f;
    f.reserve(n);

    for (u32 u = 0; u < n; ++u) f.emplace_back(par[u], u);

    mld::static_csr h(n + 1, f);
    mld::rooted_tree r(h, n);

    std::vector<u32> ans;
    ans.reserve(m);

    for (u32 i = 0; i < m; ++i) {
        auto &[u, v] = e[i];
        if (uf.is_same(u, v) && !r.is_ancestor(v, u)) ans.push_back(i);
    }

    std::cout << ans.size() << '\n';
    for (auto &&x : ans) std::cout << x + 1 << ' ';
    std::cout << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 n, m;
    while (std::cin >> n >> m) solve(n, m);
}
