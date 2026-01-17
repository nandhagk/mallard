#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/chmin_min.h"
#include "lib/algebra/max.h"
#include "lib/four_russian.h"
#include "lib/hld_aggregate.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/sparse_table.h"
#include "lib/static_csr.h"
#include "lib/union_find.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::tuple<u32, u32, u32>> e(m);
    for (auto &&[u, v, w] : e) std::cin >> u >> v >> w, --u, --v;

    std::vector<u32> ord(m);
    std::iota(ord.begin(), ord.end(), 0);
    std::ranges::sort(ord, {}, [&](u32 i) { return std::tie(std::get<2>(e[i]), i); });

    mld::union_find uf(n);

    std::vector<std::pair<u32, u32>> f;
    f.reserve(n);

    std::vector<u32> c;
    c.reserve(n);

    std::vector<bool> edg(m);
    for (u32 k : ord) {
        auto &&[u, v, w] = e[k];
        if (uf.merge(u, v))
            f.emplace_back(u, v), f.emplace_back(v, u), c.emplace_back(w), edg[k] = 1;
    }

    mld::static_csr g(n, f);
    mld::rooted_tree r(g);

    std::vector<u32> d(n);
    for (u32 i = 0; i < n - 1; ++i) {
        auto &&[u, v] = f[i << 1];
        if (r.par[u] == v) std::swap(u, v);

        d[v] = c[i];
    }

    mld::hld_aggregate<mld::four_russian<mld::sparse_table<mld::algebra::max<u32>>>>
        hag(g, d);

    mld::hld_aggregate<mld::lazy_segment_tree<mld::actions::chmin_min<u32>>> st(
        g, std::views::iota(u32{0}, n) | std::views::transform([](auto) {
               return std::numeric_limits<u32>::max();
           }));

    for (u32 i = 0; i < m; ++i) {
        if (edg[i]) continue;

        auto &&[u, v, w] = e[i];
        st.apply_path(u, v, w, 1);
    }

    for (u32 i = 0; i < m; ++i) {
        auto &&[u, v, w] = e[i];
        if (edg[i]) {
            auto x = st.fold_path(u, v, 1);
            if (x.val() == w)
                std::cout << "at least one\n";
            else
                std::cout << "any\n";
        } else {
            auto x = hag.fold_path(u, v, 1);
            if (x.val() == w)
                std::cout << "at least one\n";
            else
                std::cout << "none\n";
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
