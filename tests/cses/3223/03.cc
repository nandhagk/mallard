#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/fenwick_set.h"
#include "ska_sort.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    std::vector<std::pair<u32, u32>> a;
    a.reserve(n);

    for (u32 i = 0; i < n; ++i) {
        u32 x;
        std::cin >> x;
        a.emplace_back(x, i);
    }

    // std::ranges::sort(a);
    ska::ska_sort(a.begin(), a.end());

    std::vector<u32> na(n);
    for (u32 i = 0; i < n; ++i) na[a[i].second] = i;

    mld::fenwick_set<> fws(n);

    u64 ans = 0;
    for (u32 i = 0; i < k - 1; ++i) {
        ans += i - fws.rank(na[i]);
        fws.insert(na[i]);
    }

    for (u32 i = k - 1; i < n; ++i) {
        ans += k - 1 - fws.rank(na[i]);
        fws.insert(na[i]);

        std::cout << ans << ' ';

        ans -= fws.rank(na[i - k + 1]);
        fws.erase(na[i - k + 1]);
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
