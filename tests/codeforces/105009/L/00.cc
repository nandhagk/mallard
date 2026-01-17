#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/fenwick_tree.h"
#include "lib/prelude.h"
#include "lib/scan_table.h"
#include "lib/static_csr.h"

static constexpr u32 N = 200'000;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::vector<std::vector<u32>> y(N + 2);
    for (u32 i = 0; i < n; ++i) {
        for (u32 l = 2, x = a[i]; l <= x; l = x / (x / l) + 1) y[l].push_back(i);
        y[a[i] + 1].push_back(i);
    }

    std::vector<std::pair<u32, std::tuple<u32, u32, u32>>> f;
    f.reserve(q);

    for (u32 i = 0; i < q; ++i) {
        u32 l, r, x;
        std::cin >> l >> r >> x;

        f.emplace_back(x, std::tuple<u32, u32, u32>{--l, r, i});
    }

    mld::static_csr g(N + 1, f);
    f.clear(), f.shrink_to_fit();

    mld::scan_table<mld::algebra::sum<u64>> sc(a);

    auto b = a;
    mld::fenwick_tree<mld::algebra::sum<u64>> st(b);

    std::vector<u64> ans(q);
    for (u32 x = 1; x <= N; ++x) {
        for (u32 k : y[x]) st.add(k, u64(a[k] / x) - b[k]), b[k] = a[k] / x;
        for (auto &&[l, r, i] : g[x])
            ans[i] = sc.fold(l, r).val() - st.fold(l, r).val() * x;
    }

    for (auto &&x : ans) std::cout << x << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
