#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/sieve.h"
#include "lib/static_csr.h"

static constexpr u32 N = 300'000;

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(std::bit_width(N) * n);

    mld::sieve z(N);
    for (u32 i = 0; i < n; ++i) {
        for (auto &&[p, _] : z.factorize(a[i])) {
            e.emplace_back(i, p + n);
            e.emplace_back(p + n, i);
        }
    }

    mld::static_csr g(n + N + 1, e);

    u32 s, t;
    std::cin >> s >> t;

    --s, --t;

    constexpr u32 UNSET = -1;
    std::vector<u32> d(n + N + 1, UNSET);
    std::vector<u32> p(n + N + 1, UNSET);
    std::queue<u32> q;

    q.push(s);
    d[s] = 0;

    while (!q.empty()) {
        u32 u = q.front();
        q.pop();

        for (u32 v : g[u]) {
            if (d[v] == UNSET) {
                d[v] = d[u] + 1;
                p[v] = u;
                q.push(v);
            }
        }
    }

    if (d[t] == UNSET) {
        std::cout << -1 << '\n';
        return;
    }

    std::vector<u32> pth;
    pth.reserve(n);

    for (u32 cur = t; cur != UNSET; cur = p[cur])
        if (cur < n) pth.push_back(cur);

    std::cout << pth.size() << '\n';
    for (u32 u : pth | std::views::reverse) std::cout << u + 1 << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
