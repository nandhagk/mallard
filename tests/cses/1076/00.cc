#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/compress.h"
#include "lib/prelude.h"
#include "lib/wavelet_matrix.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    auto [v, na] =
        mld::compress(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                          u32 a;
                          std::cin >> a;
                          return a;
                      }));

    mld::wavelet_matrix wm(na, static_cast<u32>(v.size()));
    for (u32 i = 0; i <= n - k; ++i)
        std::cout << v[wm.kth(i, i + k, (k - 1) / 2)] << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
