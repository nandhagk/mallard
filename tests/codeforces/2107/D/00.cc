#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::set<u32>> g(n);
    for (u32 i = 1; i < n; ++i) {
        u32 u, v;
        std::cin >> u >> v;

        --u, --v;
        g[u].insert(v);
        g[v].insert(u);
    }

    std::vector<u32> sen;
    sen.reserve(n);

    std::vector<u32> dst(n), prv(n);

    auto dfs1 = [&](auto &&self, u32 u, u32 t) -> void {
        sen.push_back(u);
        for (u32 v : g[u]) {
            if (v == t) continue;

            dst[v] = dst[u] + 1;
            self(self, v, u);
        }
    };

    auto dfs2 = [&](auto &&self, u32 u, u32 t) -> void {
        for (u32 v : g[u]) {
            if (v == t) continue;

            dst[v] = dst[u] + 1;
            prv[v] = u;
            self(self, v, u);
        }
    };

    std::vector<std::tuple<u32, u32, u32>> ans;
    ans.reserve(n);

    std::queue<u32> q;
    q.push(0);

    while (!q.empty()) {
        u32 u = q.front();
        q.pop();

        if (g[u].empty()) {
            ans.emplace_back(0, u, u);
            continue;
        }

        dst[u] = 0;
        sen.clear();

        dfs1(dfs1, u, u);

        u32 s = u;
        for (u32 v : sen)
            if (std::tie(dst[v], v) > std::tie(dst[s], s)) s = v;

        dst[s] = 0;
        prv[s] = n;
        dfs2(dfs2, s, s);

        u32 t = s;
        for (u32 v : sen)
            if (std::tie(dst[v], v) > std::tie(dst[t], t)) t = v;

        ans.emplace_back(dst[t], std::max(s, t), std::min(s, t));

        for (u32 v = t; v != s; v = prv[v]) {
            u32 w = prv[v];
            for (u32 x : g[v]) {
                g[x].erase(v);
                if (x != w) q.push(x);
            }
        }
    }

    std::ranges::sort(ans, std::greater{});
    for (auto &&[d, u, v] : ans)
        std::cout << d + 1 << ' ' << u + 1 << ' ' << v + 1 << ' ';

    std::cout << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
