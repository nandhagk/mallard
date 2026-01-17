#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/gaussian_integer.h"

void solve() {
    i64 u, v, w, x;
    std::cin >> u >> v >> w >> x;

    mld::gaussian_integer<i64> a(u, v), b(w, x);

    auto &&[p, q] = mld::gcd(a, b);
    std::cout << p << ' ' << q << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
