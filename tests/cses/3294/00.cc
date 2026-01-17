#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/algebra/sum.h"
#include "lib/potentialized_union_find.h"

void solve() {
	u32 n, m;
    std::cin >> n >> m;

    mld::potentialized_union_find<mld::algebra::sum<i64>> d(n + 1);
    while (m--) {
        u32 l, r;
        i32 x;
        std::cin >> l >> r >> x;

        --l;
        if (auto y = d.potential(l, r)) {
            if (*y != x) {
                std::cout << "NO" << '\n';
                return;
            }
        } else {
            d.merge(l, r, x);
        }
    }

    std::vector<i64> as(n + 1);
    for (u32 i = 0; i <= n; ++i) as[i] = d.potential(i).val();

    std::cout << "YES" << '\n';
    for (u32 i = 1; i <= n; ++i) std::cout << as[i - 1] - as[i] << ' ';
}

i32 main() {
	solve();
}
