#include <bits/stdc++.h>

#include "lib/algebra/sum.h"
#include "lib/foldable_union_find.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::tuple<u32, u32, u32>> e(m);
    for (auto &&[w, u, v] : e) std::cin >> u >> v >> w, --u, --v;

    std::ranges::sort(e, std::greater{});

    mld::foldable_union_find<mld::algebra::sum<u32>> uf(
        std::views::iota(u32{0}, n) | std::views::transform([](auto) { return 0; }));

    u32 ans = 0;
    for (auto &&[w, u, v] : e) {
        if (uf.fold(u) + uf.fold(v) <= 1) {
            if (!uf.merge(u, v)) uf.add(u, 1);
            ans += w;
        }
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
