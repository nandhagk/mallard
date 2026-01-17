#include <bits/stdc++.h>

#include "lib/algebra/gcd.h"
#include "lib/segment_tree.h"
#include "lib/union_find.h"

void solve() {
    u32 n, p;
    std::cin >> n >> p;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    mld::segment_tree<mld::algebra::gcd<u32>> st(a);

    std::vector<u32> ord(n);
    std::iota(ord.begin(), ord.end(), 0);
    std::ranges::sort(ord, {}, [&](u32 i) { return std::tie(a[i], i); });

    std::vector<u32> par(n);
    std::iota(par.begin(), par.end(), 0);

    u64 ans = 0;
    mld::union_find uf(n);

    auto find = [&](u32 u) {
        for (; par[u] != u; u = par[u] = par[par[u]]);
        return u;
    };

    auto merge = [&](u32 l, u32 r, u32 x) {
        for (l = find(l); l + 1 < r;) {
            if (uf.merge(l, l + 1)) ans += x;
            l = par[l] = find(l + 1);
        }
    };

    for (u32 k : ord) {
        u32 x = a[k];
        if (x >= p) break;

        auto f = [&](auto &&y) { return y + x == x; };
        u32 l = st.min_left(k + 1, f);
        u32 r = st.max_right(k, f);

        merge(l, r, x);
    }

    std::cout << ans + u64(uf.ccs() - 1) * p << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
