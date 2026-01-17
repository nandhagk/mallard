#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/combined.h"
#include "lib/algebra/sum.h"
#include "lib/fenwick_tree.h"
#include "lib/prelude.h"
#include "ska_sort.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<std::pair<u32, u32>> p;
    p.reserve(n);
    for (u32 i = 0; i < n; ++i) {
        u32 a;
        std::cin >> a;
        p.emplace_back(a, i);
    }
    ska::ska_sort(p.begin(), p.end());

    std::vector<std::pair<u32, u32>> y, z;
    y.reserve(q), z.reserve(q);
    for (u32 i = 0; i < q; ++i) {
        u32 l, r, x;
        std::cin >> l >> r >> x;
        y.emplace_back(l, r);
        z.emplace_back(x, i);
    }
    ska::ska_sort(z.begin(), z.end());

    std::vector<std::tuple<u32, u64>> ans(q);
    mld::fenwick_tree<
        mld::algebra::combined<mld::algebra::sum<u32>, mld::algebra::sum<u64>>>
        ft(n);

    u32 j = 0;
    for (auto &&[x, i] : z) {
        for (; j < n && p[j].first <= x; ++j) {
            auto &&[a, k] = p[j];
            ft.add(k, {1, a});
        }

        auto &&[l, r] = y[i];
        if (l != r) ans[i] = ft.fold(l, r).val();
    }

    for (auto &&[a, b] : ans) std::cout << a << ' ' << b << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
