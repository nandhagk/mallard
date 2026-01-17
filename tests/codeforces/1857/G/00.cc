#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/static_montgomery_modint.h"
#include "lib/union_find.h"

using Z = mld::montgomerymodint998244353;

void solve() {
    u32 n, s;
    std::cin >> n >> s;

    std::vector<std::tuple<u32, u32, u32>> e(n - 1);
    for (auto &&[w, u, v] : e) std::cin >> u >> v >> w, --u, --v;

    std::ranges::sort(e);

    mld::union_find uf(n);

    Z ans = 1;
    for (auto &&[w, u, v] : e) {
        assert(!uf.is_same(u, v));

        u64 a = uf.size(u), b = uf.size(v);
        uf.merge(u, v);

        ans *= Z(s - w + 1).pow(a * b - 1);
    }

    std::cout << ans.val() << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
