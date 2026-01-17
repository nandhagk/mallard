#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/min.h"
#include "lib/prelude.h"
#include "lib/disjoint_sparse_table.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    mld::disjoint_sparse_table<mld::algebra::min<u32>> st(std::views::iota(u32{0}, n) |
                                                 std::views::transform([](auto) {
                                                     u32 a;
                                                     std::cin >> a;
                                                     return a;
                                                 }));

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        std::cout << st.fold(l, r).val() << '\n';
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
