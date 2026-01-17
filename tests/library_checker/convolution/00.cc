#include <bits/stdc++.h>
#include <functional>

#include "blazingio.min.h"
#include "lib/ntt.h"
#include "lib/prelude.h"
#include "lib/static_montgomery_modint.h"

using Z = mld::montgomerymodint998244353;

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<Z> a(n), b(m);

    for (auto &&x : a) {
        u32 y;
        std::cin >> y;
        x = y;
    }

    for (auto &&x : b) {
        u32 y;
        std::cin >> y;
        x = y;
    }

    u32 len = std::bit_ceil(n + m - 1);

    a.resize(len), mld::ntt<false>(a);
    b.resize(len), mld::ntt<false>(b);
    std::ranges::transform(a, b, a.begin(), std::multiplies<>{});
    mld::ntt<true>(a), a.resize(n + m - 1);

    for (auto &&x : a) std::cout << x.val() << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
