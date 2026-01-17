#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_distinct.h"
#include "lib/compress.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    auto [v, na] =
            mld::compress(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                              u32 a;
                              std::cin >> a;
                              return a;
                          }));

    mld::static_distinct sd(na, static_cast<u32>(v.size()));
    for (u32 i = k; i <= n; ++i) std::cout << sd.count(i - k, i) << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
