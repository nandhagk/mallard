#include <bits/stdc++.h>

#include "lib/algebra/gcd.h"
#include "lib/four_russian.h"
#include "lib/prelude.h"
#include "lib/sparse_table.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    if (std::ranges::count(a, a[0]) == n) {
        std::cout << 0 << '\n';
        return;
    }

    mld::four_russian<mld::sparse_table<mld::algebra::gcd<u32>>> st(a);

    u32 lo = 0, hi = n;
    while (hi - lo > 1) {
        u32 m = std::midpoint(lo, hi);
        for (u32 i = 0; i < n; ++i)
            if (i + m <= n)
                a[i] = st.fold(i, i + m).val();
            else
                a[i] = (st.fold(i, n) + st.fold(0, i + m - n)).val();

        if (std::ranges::count(a, a[0]) == n)
            hi = m;
        else
            lo = m;
    }

    std::cout << hi - 1 << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
