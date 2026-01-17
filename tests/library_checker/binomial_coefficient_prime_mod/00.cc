#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/binomial.h"
#include "lib/dynamic_montgomery_modint.h"
#include "lib/prelude.h"

using Z = mld::dynamic_montgomery_modint_32<-1>;

void solve() {
    u32 q, m;
    std::cin >> q >> m;

    if (m == 2) {
        while (q--) {
            u32 n, k;
            std::cin >> n >> k;

            if (k > n)
                std::cout << 0 << '\n';
            else
                std::cout << (1 - (k & (n - k))) << '\n';
        }
    } else {
        Z::set_mod(m);

        mld::binomial<Z> b(std::min<u32>(m, 10'000'000) - 1);
        while (q--) {
            u32 n, k;
            std::cin >> n >> k;

            if (k > n)
                std::cout << 0 << '\n';
            else
                std::cout << b.comb(n, k).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
