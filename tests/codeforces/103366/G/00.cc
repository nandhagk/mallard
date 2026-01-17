#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/compress.h"
#include "lib/dynamic_csr.h"
#include "lib/prelude.h"
#include "lib/sieve.h"
#include "lib/static_mode.h"

static constexpr u32 N = 1'000'000;
static mld::sieve s;
static mld::dynamic_csr<u32> g;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a;
    a.reserve(7 * n);

    std::vector<u32> z;
    z.reserve(n + 1);

    for (u32 i = 0; i < n; ++i) {
        u32 x;
        std::cin >> x;

        z.push_back(a.size());
        for (u32 p : g[x]) a.push_back(p);
    }

    z.push_back(a.size());

    if (a.empty()) {
        while (q--) {
            u32 l, r;
            std::cin >> l >> r;

            std::cout << 0 << '\n';
        }

        return;
    }

    auto [v, na] = mld::compress(a);
    mld::static_mode sm(na, static_cast<u32>(v.size()));

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        u32 x = z[--l], y = z[r];
        std::cout << (x == y ? 0 : sm.mode(x, y).second) << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    s.build(N);

    g.build(N + 1);
    g.reserve(7 * N);
    for (u32 p : s.pri)
        for (u32 k = p; k <= N; k += p) g[k].emplace_back(p);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
