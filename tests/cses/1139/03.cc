#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/compress.h"
#include "lib/static_csr.h"
#include "lib/fenwick_tree.h"
#include "lib/prelude.h"
#include "lib/rooted_tree.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> z(n);
    for (auto &&x : z) std::cin >> x;

    std::vector<std::pair<u32, u32>> e;
    e.reserve(2 * n);

    for (u32 i = 1; i < n; ++i) {
        u32 a, b;
        std::cin >> a >> b;

        --a, --b;
        e.emplace_back(a, b);
        e.emplace_back(b, a);
    }

    mld::static_csr g(n, e);
    mld::rooted_tree h(g);

    auto &&[v, na] =
        mld::compress(h.tor | std::views::transform([&](u32 k) { return z[k]; }));

    std::vector<u32> nxt(n, n);
    mld::fenwick_tree<mld::algebra::sum<u32>> fwt(n + 1);

    std::vector<i32> prv(v.size(), -1);

    u32 i = 0;
    for (auto &&a : na) {
        if (prv[a] == -1)
            fwt.add(i, 1);
        else
            nxt[prv[a]] = i;
        prv[a] = i++;
    }

    std::vector<u32> out(n);
    for (u32 u : h.tor) {
        out[u] = fwt.fold(h.tin[u], h.tin[u] + h.siz[u]).val();
        fwt.add(nxt[h.tin[u]], 1);
    }

    for (auto &&x : out) std::cout << x << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
