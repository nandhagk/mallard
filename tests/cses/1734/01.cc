#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/compress.h"
#include "lib/offline/static_distinct.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    auto [v, na] =
        mld::compress(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                          u32 a;
                          std::cin >> a;
                          return a;
                      }));

    mld::offline::static_distinct sd(na, static_cast<u32>(v.size()));
    for (u32 i = 0; i < q; ++i) {
        u32 l, r;
        std::cin >> l >> r;

        sd.query(--l, r);
    }

    // CSES compiler bug
    auto z = sd.solve();
    for (auto &&x : z | std::views::drop(z.size() - q)) std::cout << x << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
