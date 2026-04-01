#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"
#include "lib/suffix_array_query.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    u32 m;
    std::cin >> m;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(--u, --v);
    }

    mld::static_csr g(n, e);

    std::vector<u32> b(n);
    for (auto &&x : b) std::cin >> x;

    std::cin >> m;
    e.clear();
    e.reserve(m);

    while (m--) {
        u32 u, v;
        std::cin >> u >> v;

        e.emplace_back(--u, --v);
    }

    mld::static_csr h(n, e);

    if (auto x = std::ranges::count(a, 0), y = std::ranges::count(b, 1); x != y) {
        std::cout << "NO\n";
        return;
    } else if (x == 0 || x == n) {
        std::cout << "YES\n";
        return;
    }

    constexpr u32 UNSET = -1;
    auto f = [n, k, UNSET](const mld::static_csr<u32> &z) {
        std::vector<u32> c(n, UNSET);

        std::queue<u32> q;
        c[0] = 0, q.push(0);
        while (!q.empty()) {
            u32 u = q.front();
            q.pop();

            for (u32 v : z[u])
                if (c[v] == UNSET)
                    c[v] = (c[u] + 1) % k, q.push(v);
                else
                    assert(c[v] == (c[u] + 1) % k);
        }

        for (u32 u = 0; u < n; ++u) assert(c[u] < k);
        return c;
    };

    auto c = f(g), d = f(h);
    std::vector<u32> x(2 * k + 1), y(2 * k + 1);
    x[k] = y[k] = n;
    for (u32 u = 0; u < n; ++u) {
        if (a[u])
            ++x[(c[u] + 1) % k];
        else
            ++y[(c[u] + k - 1) % k];

        if (b[u])
            ++y[d[u] + k + 1];
        else
            ++x[d[u] + k + 1];
    }

    mld::suffix_array_query s(x, n + 1), t(y, n + 1);
    for (u32 i = 0; i < k; ++i) {
        bool u = s.lcp(0, k + 1 + i) == k - i && s.lcp(k + 1, k - i) == i;
        bool v = t.lcp(0, k + 1 + i) == k - i && t.lcp(k + 1, k - i) == i;
        if (u && v) {
            std::cout << "YES\n";
            return;
        }
    }

    std::cout << "NO\n";
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
