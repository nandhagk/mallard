#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/min.h"
#include "lib/four_russian.h"
#include "lib/hld.h"
#include "lib/prelude.h"
#include "lib/sparse_table.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m, q;
    std::cin >> n >> m >> q;

    std::vector<u32> p(n);
    for (auto &&x : p) std::cin >> x, --x;

    std::vector<u32> nxt(n);
    for (u32 i = 1; i < n; ++i) nxt[p[i - 1]] = p[i];
    nxt[p[n - 1]] = p[0];

    std::vector<u32> a(m);
    for (auto &&x : a) std::cin >> x, --x;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * m);

    std::vector<u32> prv(n, m);
    for (u32 i = m; i--;) {
        e.emplace_back(prv[nxt[a[i]]], i);
        prv[a[i]] = i;
    }

    mld::static_csr g(m + 1, e);
    mld::hld h(g, m);

    mld::four_russian<mld::sparse_table<mld::algebra::min<u32>>> st(
        std::views::iota(u32{0}, m) |
        std::views::transform([&](u32 k) { return h.jump(k, n - 1).value_or(m); }));

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        std::cout << (st.fold(--l, r).val() < r);
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
