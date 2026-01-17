#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/compress.h"
#include "lib/prelude.h"
#include "lib/wavelet_matrix.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    auto &&[v, na] =
        mld::compress(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                          u32 a;
                          std::cin >> a;
                          return a;
                      }));

    mld::wavelet_matrix wm(na, static_cast<u32>(v.size()));

    while (q--) {
        u32 a, b, c, d;
        std::cin >> a >> b >> c >> d;

        auto l = std::lower_bound(v.begin(), v.end(), c) - v.begin();
        auto r = std::upper_bound(v.begin(), v.end(), d) - v.begin();

        --a;
        std::cout << wm.rank(a, b, r) - wm.rank(a, b, l) << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
