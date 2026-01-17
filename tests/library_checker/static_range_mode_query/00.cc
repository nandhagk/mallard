#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/compress.h"
#include "lib/prelude.h"
#include "lib/static_mode.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    auto [v, na] =
        mld::compress(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                          u32 a;
                          std::cin >> a;
                          return a;
                      }));
    mld::static_mode sm(na, static_cast<u32>(v.size()));

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        auto [m, c] = sm.mode(l, r);
        std::cout << v[m] << ' ' << c << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
