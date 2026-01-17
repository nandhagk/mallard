#include <bits/stdc++.h>
#include <limits>
#include <queue>

#include "lib/actions/add_min.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(m);

    std::vector<u32> in(n);
    std::iota(in.begin(), in.end(), 0);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(--u, --v);
        --in[v];
    }

    mld::static_csr g(n, e);

    mld::lazy_segment_tree<mld::actions::add_min<i32>> st(in);

    std::priority_queue<u32> q;
    for (u32 u = 0; u < n; ++u)
        if (in[u] == 0) q.push(u), st.set(u, std::numeric_limits<i32>::max());

    while (!q.empty()) {
        u32 u = q.top();
        q.pop();

        std::cout << u + 1 << ' ';

        if (u == n - 1) continue;

        st.apply(u + 1, n, -1);
        for (u32 v : g[u]) st.apply(v, 1);

        for (;;) {
            u32 v = st.max_right(u + 1, [](auto &&x) { return x > 0; });
            if (v == n) break;

            q.push(v);
            st.set(v, std::numeric_limits<i32>::max());
        }
    }

    std::cout << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
