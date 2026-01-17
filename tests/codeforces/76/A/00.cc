#include <bits/stdc++.h>

#include "lib/am_tree.h"
#include "lib/erasable_heap.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m, a, b;
    std::cin >> n >> m >> a >> b;

    std::vector<std::tuple<u32, u32, u32, u32>> e(m);
    for (auto &&[g, s, u, v] : e) std::cin >> u >> v >> g >> s, --u, --v;

    std::ranges::sort(e);

    mld::am_tree<u32> amt(n);
    mld::erasable_heap<u32> hep;

    u64 ans = std::numeric_limits<u64>::max();
    for (auto &&[g, s, u, v] : e) {
        if (auto res = amt.insert(u, v, s)) {
            if (auto r = *res) hep.erase(*r);
            hep.insert(s);
        }
        if (amt.ccs() == 1) ans = std::min(ans, u64(a) * g + u64(b) * hep.top());
    }

    std::cout << i64(amt.ccs() == 1 ? ans : -1) << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
