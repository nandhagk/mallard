#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/add_min.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u64> a(n), b(n);
    std::vector<u32> c(n - 1);

    std::vector<std::pair<u32, u32>> e;
    e.reserve(n);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        e.emplace_back(--u, --v);
        a[v] = w;
    }

    mld::static_csr g(n, e);
    mld::rooted_tree h(g);

    for (auto &&[u, v] : e) assert(h.par[v] == u);

    for (u32 i = 1; i < n; ++i) {
        u32 u, v, x;
        std::cin >> u >> v >> x;

        --u, --v;
        assert(v == 0);
        b[u] = x;
        c[i - 1] = u;
    }

    mld::lazy_segment_tree<mld::actions::add_min<u64>> st(
        std::views::iota(u32{0}, n) | std::views::transform([](auto) { return 0; }));

    for (u32 u = 1; u < n; ++u)
        st.apply(h.tin[u], h.tin[u] + h.siz[u], a[u]), st.apply(h.tin[u], b[u]);

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 i, w;
            std::cin >> i >> w;

            --i;
            if (i < n - 1) {
                auto &&[_, v] = e[i];
                st.apply(h.tin[v], h.tin[v] + h.siz[v], w - a[v]);
                a[v] = w;
            } else {
                u32 u = c[i - (n - 1)];
                st.apply(h.tin[u], w - b[u]);
                b[u] = w;
            }
        } else {
            u32 u, v;
            std::cin >> u >> v;

            --u, --v;
            u64 ans = (st.get(h.tin[v]).val() - b[v]) - (st.get(h.tin[u]).val() - b[u]);
            if (!h.is_ancestor(u, v))
                ans += st.fold(h.tin[u], h.tin[u] + h.siz[u]).val();

            std::cout << ans << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
