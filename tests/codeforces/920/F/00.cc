#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/fenwick_tree.h"
#include "lib/prelude.h"

static constexpr u32 N = 1'000'000;

void solve() {
    std::vector<u32> d(N + 1);
    for (u32 i = 1; i <= N; ++i)
        for (u32 j = i; j <= N; j += i) ++d[j];

    u32 n, m;
    std::cin >> n >> m;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::map<u32, u32> b;
    for (auto &&x : a) b.emplace_hint(b.end(), b.size(), x);

    mld::fenwick_tree<mld::algebra::sum<u64>> fwt(a);

    while (m--) {
        u32 t, l, r;
        std::cin >> t >> l >> r;

        --l;
        if (t == 1) {
            for (auto it = b.lower_bound(l); it != b.end();) {
                auto &&[p, q] = *it;
                if (p >= r) break;

                fwt.add(p, u64(d[q]) - q), q = d[q];
                if (q <= 2)
                    b.erase(it++);
                else
                    ++it;
            }
        } else {
            std::cout << fwt.fold(l, r).val() << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
