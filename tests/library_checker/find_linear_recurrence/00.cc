#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/berlekamp_massey.h"
#include "lib/prelude.h"
#include "lib/static_montgomery_modint.h"

using Z = mld::montgomerymodint998244353;

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<Z> a(n);
    for (auto &&x : a) {
        u32 y;
        std::cin >> y;
        x = y;
    }

    auto c = mld::berlekamp_massey(a);

    std::cout << c.size() << '\n';
    for (auto &&x : c) std::cout << x.val() << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
