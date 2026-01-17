#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/min.h"
#include "lib/algebra/sum.h"
#include "lib/compress.h"
#include "lib/fenwick_tree.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"
#include "lib/static_modint.h"

using Z = mld::modint1000000007;

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

    mld::segment_tree<mld::algebra::min<u32>> st(nxt);
    mld::fenwick_tree<mld::algebra::sum<Z>> ft(n + 1);

    ft.add(0, 1);
    for (u32 i = 0; i < n; ++i) {
        u32 l = st.min_left(i + 1, [&](auto &&x) { return x > i; });
        ft.add(i + 1, ft.fold(l, i + 1).val());
    }

    std::cout << ft.fold(n, n + 1).val().val() << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
