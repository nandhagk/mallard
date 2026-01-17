#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/max.h"
#include "lib/algebra/sum.h"
#include "lib/dynamic_graph.h"
#include "lib/four_russian.h"
#include "lib/hld_aggregate.h"
#include "lib/prelude.h"
#include "lib/scan_table.h"
#include "lib/sparse_table.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n + 1);
    for (auto &&x : a | std::views::take(n)) std::cin >> x;

    std::vector<u32> mx;
    mx.reserve(n);

    std::vector<u32> nxt(n + 1, n);
    for (u32 i = 0; i < n; ++i) {
        while (!mx.empty() && a[i] > a[mx.back()]) {
            nxt[mx.back()] = i;
            mx.pop_back();
        }

        mx.push_back(i);
    }

    mld::dynamic_graph<u32> g(n + 1);
    g.reserve(n);

    for (u32 i = 0; i < n; ++i) g.add_edge(nxt[i], i);
    mld::hld_aggregate<mld::scan_table<mld::algebra::sum<u64>>> hag(
        g, n, std::views::iota(u32{0}, n + 1) | std::views::transform([&](u32 u) {
                  return static_cast<u64>(a[u]) * (nxt[u] - u);
              }));

    mld::scan_table<mld::algebra::sum<u64>> b(a);
    mld::four_russian<mld::sparse_table<mld::algebra::max<std::pair<u32, u32>>>> st(
        std::views::iota(u32{0}, n) |
        std::views::transform([&](u32 i) { return std::make_pair(a[i], n - i - 1); }));

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        auto [m, z] = st.fold(--l, r).val();
        z = n - z - 1;
        std::cout << hag.fold_path(l, z, 1).val() + static_cast<u64>(m) * (r - z) -
                         b.fold(l, r).val()
                  << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
