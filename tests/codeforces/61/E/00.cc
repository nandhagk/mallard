#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/fenwick_set.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::vector<u32> rnk(n);
    std::iota(rnk.begin(), rnk.end(), 0);
    std::ranges::sort(rnk, {}, [&](u32 i) { return a[i]; });

    std::vector<u32> ord(n);
    for (u32 i = 0; i < n; ++i) ord[rnk[i]] = i;

    u64 ans = 0;
    mld::fenwick_set<> fws(n);

    u32 z = 0;
    for (u32 k : ord) {
        u32 l = fws.rank(k);
        ans += u64(k - l) * (z - l);
        fws.insert(k), ++z;
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
