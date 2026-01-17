#include <bits/stdc++.h>

#include "lib/hash.h"
#include "lib/pbds.h"
#include "lib/prelude.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    mld::hash<u64> h;
    std::vector<u64> sum(n);

    std::vector<std::pair<u32, u32>> e(m);
    for (auto &&[u, v] : e) std::cin >> u >> v, --u, --v;
    for (auto &&[u, v] : e) sum[u] ^= h(v), sum[v] ^= h(u);

    mld::pbds::hash_map<u64, u32> cnt;
    cnt.resize(n);

    for (u32 u = 0; u < n; ++u) ++cnt[sum[u]];

    u64 ans = 0;
    for (auto &&[_, c] : cnt) ans += u64(c) * (c - 1) / 2;
    for (auto &&[u, v] : e)
        if ((sum[u] ^ h(u)) == (sum[v] ^ h(v))) ++ans;

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
