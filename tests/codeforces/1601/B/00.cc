#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/static_csr.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n + 1), b(n + 1);
    for (auto &&x : a | std::views::drop(1)) std::cin >> x;
    for (auto &&x : b | std::views::drop(1)) std::cin >> x;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(std::bit_width(n) * n);

    auto id = [&](u32 k) { return k >= n + 1 ? k - (n + 1) : n + 1 + k; };

    for (u32 i = 2; i < 2 * (n + 1); ++i) e.emplace_back(id(i / 2), id(i));

    for (u32 i = 1; i <= n; ++i) {
        u32 l = i - a[i], r = i + 1;
        for (l += n + 1, r += n + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) e.emplace_back(2 * (n + 1) + i, id(l++));
            if (r & 1) e.emplace_back(2 * (n + 1) + i, id(--r));
        }

        e.emplace_back(i, 2 * (n + 1) + i + b[i]);
    }

    mld::static_csr g(3 * (n + 1), e);

    std::deque<u32> q;

    constexpr u32 UNSET = -1;
    std::vector<u32> d(3 * (n + 1), UNSET);
    std::vector<u32> p(3 * (n + 1), UNSET);

    q.push_front(n);
    d[n] = 0;

    while (!q.empty()) {
        u32 u = q.front();
        q.pop_front();

        u32 w = u < n + 1;
        for (u32 v : g[u]) {
            if (d[v] == UNSET || d[u] + w < d[v]) {
                d[v] = d[u] + w;
                p[v] = u;
                if (w == 1)
                    q.push_back(v);
                else
                    q.push_front(v);
            }
        }
    }

    if (d[0] == UNSET) {
        std::cout << -1 << '\n';
        return;
    }

    std::cout << d[0] << '\n';

    std::vector<u32> pth;
    pth.reserve(n);

    for (u32 cur = 0; cur != n; cur = p[cur])
        if (cur < n + 1) pth.push_back(cur);
    for (u32 u : pth | std::views::reverse) std::cout << u << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
