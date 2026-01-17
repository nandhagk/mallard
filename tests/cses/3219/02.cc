#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/offline/static_mex.h"
#include "lib/prelude.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    mld::offline::static_mex sm(std::views::iota(u32{0}, n) |
                                std::views::transform([](auto) {
                                    u32 a;
                                    std::cin >> a;
                                    return a;
                                }));
    sm.reserve(n - k + 1);

    for (u32 i = k; i <= n; ++i) sm.query(i - k, i);

    for (auto &&x : sm.solve()) std::cout << x << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
