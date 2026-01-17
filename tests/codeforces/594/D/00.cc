#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/offline/mo.h"
#include "lib/prelude.h"
#include "lib/sieve.h"
#include "lib/static_csr.h"
#include "lib/static_montgomery_modint.h"

using Z = mld::montgomerymodint1000000007;
static constexpr u32 N = 1'000'000;

void solve() {
    mld::sieve s(N);

    u32 n;
    std::cin >> n;

    std::vector<Z> a(n);
    for (auto &&x : a) {
        u32 y;
        std::cin >> y;
        x = y;
    }

    std::vector<std::pair<u32, u32>> e;
    e.reserve(N);

    std::vector<Z> b(n);
    for (u32 i = 0; i < n; ++i) {
        b[i] = a[i].inv();
        for (u32 x = a[i].val(); x != 1;) {
            u32 p = s.lpf[x];
            do {
                x /= p;
            } while (x % p == 0);
            e.emplace_back(i, p);
        }
    }

    std::vector<Z> tot(N + 1), ito(N + 1);
    for (u32 i : s.pri) tot[i] = Z(i).inv() * (i - 1), ito[i] = tot[i].inv();

    mld::static_csr g(n, e);

    u32 q;
    std::cin >> q;

    mld::offline::mo m(n);
    m.reserve(q);

    std::vector<Z> ans(q);
    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        m.query(--l, r);
    }

    std::vector<u32> cnt(N + 1);

    Z cur = 1;
    m.solve(
        [&](u32 i) {
            cur *= a[i];
            for (u32 p : g[i])
                if (++cnt[p] == 1) cur *= tot[p];
        },
        [&](u32 i) {
            cur *= b[i];
            for (u32 p : g[i])
                if (--cnt[p] == 0) cur *= ito[p];
        },
        [&](u32 i) { ans[i] = cur; });

    for (auto &&x : ans) std::cout << x.val() << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
