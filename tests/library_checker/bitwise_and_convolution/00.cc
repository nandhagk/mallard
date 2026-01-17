#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_montgomery_modint.h"
#include "lib/wht.h"

using Z = mld::montgomerymodint998244353;

void solve() {
    u32 n;
    std::cin >> n;

    u32 m = 1 << n;
    std::vector<Z> a(m), b(m);

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

    mld::wht<'&', false>(a);
    mld::wht<'&', false>(b);
    std::ranges::transform(a, b, a.begin(), std::multiplies<>{});
    mld::wht<'&', true>(a);

    for (auto &&x : a) std::cout << x.val() << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
