#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/sum.h"
#include "lib/fenwick_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u64> z(n);
    for (auto &&x : z) std::cin >> x;

    using operand = mld::algebra::sum<u64>;
    mld::fenwick_tree<operand> st(z);

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 k, x;
            std::cin >> k >> x;

            --k;
            st.add(k, x - z[k]);
            z[k] = x;
        } else {
            u32 a, b;
            std::cin >> a >> b;

            std::cout << st.fold(--a, b).val() << '\n';
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
