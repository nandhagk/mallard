#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/compress.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    auto [v, na] =
        mld::compress(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                          u32 a;
                          std::cin >> a;
                          return a;
                      }));

    constexpr u32 UNSET = -1;
    std::vector<u32> prv(v.size(), UNSET), nxt(n, n);
    for (u32 i = 0; i < n; ++i) {
        if (prv[na[i]] != UNSET) nxt[prv[na[i]]] = i;
        prv[na[i]] = i;
    }

    u64 ans = 0;
    for (u32 i = 0; i < n; ++i) ans += u64(i + 1) * (nxt[i] - i);

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
