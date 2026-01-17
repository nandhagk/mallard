#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/pollard_rho.h"
#include "lib/prelude.h"

void solve() {
    u64 n;
    std::cin >> n;

    auto fs = mld::factorize(n);

    u32 t = 0;
    for (auto &&[_, c] : fs) t += c;

    std::cout << t << ' ';
    for (auto &&[p, c] : fs)
        for (u32 i = 0; i < c; ++i) std::cout << p << ' ';

    std::cout << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
