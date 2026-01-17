#include <bits/stdc++.h>

#include "lib/algebra/sum.h"
#include "lib/compress.h"
#include "lib/fenwick_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    u64 k;
    std::cin >> n >> k;

    auto [v, na] =
        mld::compress(std::views::iota(u32{0}, n) | std::views::transform([](auto) {
                          u32 a;
                          std::cin >> a;
                          return a;
                      }));

    mld::fenwick_tree<mld::algebra::sum<u64>> lft(n + 1), rgt(n + 1);

    u64 cur = 0;
    for (u32 r = 1; r < n; ++r) {
        cur += rgt.fold(na[r] + 1, n + 1).val();
        rgt.add(na[r], 1);
    }

    u64 ans = 0;
    for (u32 l = 0, r = 1; l < n; ++l) {
        cur += rgt.fold(na[l]).val();
        cur += lft.fold(na[l] + 1, n + 1).val();
        lft.add(na[l], 1);

        for (; (l == r || cur > k) && r < n; ++r) {
            cur -= rgt.fold(na[r]).val();
            cur -= lft.fold(na[r] + 1, n + 1).val();
            rgt.add(na[r], -1);
        }

        ans += n - r;
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
