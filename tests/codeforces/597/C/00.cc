#include <bits/stdc++.h>

#include "lib/algebra/sum.h"
#include "lib/fenwick_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::vector<u32> ord(n);
    for (u32 i = 0; i < n; ++i) ord[a[i] - 1] = i + 1;

    std::vector<u64> cur(n + 1), nxt(n + 1);
    cur[0] = 1;

    for (++k; k--; std::swap(cur, nxt)) {
        mld::fenwick_tree<mld::algebra::sum<u64>> fwt(n + 1);

        nxt[0] = 0;
        fwt.add(0, cur[0]);
        for (u32 j : ord) {
            nxt[j] = fwt.fold(j).val();
            fwt.add(j, cur[j]);
        }
    }

    std::cout << std::reduce(cur.begin(), cur.end()) << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
