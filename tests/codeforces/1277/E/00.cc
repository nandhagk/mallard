#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/union_find.h"

void solve() {
    u32 n, m, a, b;
    std::cin >> n >> m >> a >> b;

    --a, --b;
    std::vector<std::pair<u32, u32>> e(m);
    for (auto &&[u, v] : e) std::cin >> u >> v, --u, --v;

    mld::union_find ufa(n), ufb(n);
    for (auto &&[u, v] : e) {
        if (u != a && v != a) ufa.merge(u, v);
        if (u != b && v != b) ufb.merge(u, v);
    }

    std::cout << u64(n - ufa.size(b) - 1) * (n - ufb.size(a) - 1) << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
