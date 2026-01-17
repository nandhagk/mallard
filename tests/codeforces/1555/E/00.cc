#include <bits/stdc++.h>

#include "lib/actions/add_min.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

static constexpr u32 N = 1'000'000;

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, std::pair<u32, u32>>> e;
    e.reserve(n);

    while (n--) {
        u32 l, r, w;
        std::cin >> l >> r >> w;

        e.emplace_back(w, std::pair<u32, u32>{--l, r});
    }

    mld::lazy_segment_tree<mld::actions::add_min<u32>> st(
        std::views::iota(u32{0}, 2 * m - 1) | std::views::transform([](auto) { return 0; }));

    mld::static_csr g(N + 1, e);

    u32 ans = N;
    for (u32 a = 1, b = 1; a <= N; ++a) {
        for (; b <= N && st.fold() == 0; ++b)
            for (auto &&[l, r] : g[b]) st.apply(2 * l, 2 * r - 1, 1);
        if (st.fold() == 0) break;

        ans = std::min(ans, b - a - 1);
        for (auto &&[l, r] : g[a]) st.apply(2 * l, 2 * r - 1, -1);
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
