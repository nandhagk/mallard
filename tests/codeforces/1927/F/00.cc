#include <bits/stdc++.h>

#include "lib/becc.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::pair<u32, u32>> e, f;
    e.reserve(m), f.reserve(m);

    for (u32 i = 0; i < m; ++i) {
        u32 u, v, w;
        std::cin >> u >> v >> w;

        --u, --v;
        e.emplace_back(u, v);
        e.emplace_back(v, u);

        f.emplace_back(w, i);
    }

    mld::static_csr g(n, e);
    auto [k, ids] = mld::becc(g);

    std::ranges::sort(f);
    for (auto &&[w, i] : f) {
        auto &&[u, v] = e[i << 1];
        if (ids[u] != ids[v]) continue;

        constexpr u32 UNSET = -1;
        std::vector<u32> prv(n, UNSET);

        std::queue<u32> q;
        q.push(u);
        prv[u] = v;

        while (!q.empty()) {
            u32 s = q.front();
            q.pop();

            if (s == v) break;

            for (u32 t : g[s]) {
                if (s == u && t == v) continue;
                if (prv[t] == UNSET) prv[t] = s, q.push(t);
            }
        }

        std::vector<u32> pth;
        pth.reserve(n);

        for (u32 s = v; prv[s] != v; s = prv[s]) pth.push_back(s);
        pth.push_back(u);

        std::cout << w << ' ' << pth.size() << '\n';
        for (u32 s : pth) std::cout << s + 1 << ' ';
        std::cout << '\n';

        return;
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
