#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/wavelet_matrix.h"

static constexpr u32 N = 1'000'000;

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> p(n);
    for (auto &&[l, r] : p) std::cin >> l >> r;

    std::ranges::sort(p);
    mld::wavelet_matrix wm(p | std::views::elements<1>, N + 2);

    while (m--) {
        u32 k;
        std::cin >> k;

        std::vector<u32> a(k);
        for (auto &&x : a) std::cin >> x;
        a.push_back(N + 1);

        u32 ans = n, y = 0;
        for (auto &&x : a) {
            u32 l = static_cast<u32>(
                std::lower_bound(p.begin(), p.end(), std::make_pair(y, y)) - p.begin());
            u32 r = static_cast<u32>(
                std::lower_bound(p.begin(), p.end(), std::make_pair(x, x)) - p.begin());

            if (l < r) ans -= wm.rank(l, r, x);
            y = x + 1;
        }

        std::cout << ans << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
