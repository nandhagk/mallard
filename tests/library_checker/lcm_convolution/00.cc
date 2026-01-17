#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/sieve.h"
#include "lib/static_montgomery_modint.h"

using Z = mld::montgomerymodint998244353;

void solve() {
    u32 n;
    std::cin >> n;

    mld::sieve s(n);
    std::vector<Z> a(n + 1), b(n + 1);

    for (auto &&x : a | std::views::drop(1)) {
        u32 y;
        std::cin >> y;
        x = y;
    }

    for (auto &&x : b | std::views::drop(1)) {
        u32 y;
        std::cin >> y;
        x = y;
    }

    s.lcm_transform<false>(a);
    s.lcm_transform<false>(b);
    std::ranges::transform(a, b, a.begin(), std::multiplies<>{});
    s.lcm_transform<true>(a);

    for (auto &&x : a | std::views::drop(1)) std::cout << x.val() << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
