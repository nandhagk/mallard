#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/parallel_union_find.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"

using Z = mld::modint998244353;

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<Z> a(n);
    for (u32 i = 0; i < n; ++i) {
        u32 x;
        std::cin >> x;
        a[i] = x;
    }

    mld::parallel_union_find puf(n);

    Z ans = 0;
    auto f = [&](u32 i, u32 j) -> void { ans += a[i] * a[j], a[i] += a[j]; };

    while (q--) {
        u32 k, u, v;
        std::cin >> k >> u >> v;

        if (k > 0) puf.merge(u, v, k, f);
        std::cout << ans.val() << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
