#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/pbds.h"
#include "lib/prelude.h"
#include "lib/union_find.h"

void solve() {
    u32 n, m, k;
    std::cin >> n >> m >> k;

    mld::pbds::hash_set<std::pair<u32, u32>> hs;
    hs.resize(m + k);

    for (u32 i = 0; i < m; ++i) {
        u32 a, b;
        std::cin >> a >> b;

        auto [u, v] = std::minmax(--a, --b);
        hs.insert({u, v});
    }

    std::vector<std::pair<u32, u32>> q;
    q.reserve(k);

    for (u32 i = 0; i < k; ++i) {
        u32 a, b;
        std::cin >> a >> b;

        auto [u, v] = std::minmax(--a, --b);
        hs.erase({u, v}), q.emplace_back(u, v);
    }

    mld::union_find uf(n);
    for (auto &&[u, v] : hs) uf.merge(u, v);

    std::vector<u32> ans;
    ans.reserve(k);

    for (auto &&[u, v] : q | std::views::reverse)
        ans.push_back(uf.ccs()), uf.merge(u, v);

    for (auto &&x : ans | std::views::reverse) std::cout << x << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
