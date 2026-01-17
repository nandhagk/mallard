#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/max.h"
#include "lib/dynamic_graph.h"
#include "lib/four_russian.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/sparse_table.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::vector<u32> mx;
    mx.reserve(n);

    std::vector<u32> nxt(n, n);
    for (u32 i = 0; i < n; ++i) {
        while (!mx.empty() && a[i] > a[mx.back()]) {
            nxt[mx.back()] = i;
            mx.pop_back();
        }

        mx.push_back(i);
    }

    mld::dynamic_graph<u32> g(n + 1);
    g.reserve(n);

    for (u32 u = 0; u < n; ++u) g.add_edge(nxt[u], u);
    mld::rooted_tree h(g, n);

    mld::four_russian<mld::sparse_table<mld::algebra::max<std::pair<u32, u32>>>> st(
        std::views::iota(u32{0}, n) |
        std::views::transform([&](u32 i) { return std::make_pair(a[i], i); }));

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        auto [_, j] = st.fold(--l, r).val();
        std::cout << h.dep[l] - h.dep[j] + 1 << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
