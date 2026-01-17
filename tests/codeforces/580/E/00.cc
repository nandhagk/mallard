#include <bits/stdc++.h>

#include "lib/actions/assign.h"
#include "lib/algebra/rabin_karp.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"
#include "lib/static_montgomery_modint.h"

using Z1 = mld::montgomerymodint998244353;
using Z2 = mld::montgomerymodint1000000007;

using operand = mld::algebra::rabin_karp<Z1, Z2>;

template <>
std::tuple<Z1, Z2> operand::base = mld::algebra::rabin_karp<Z1, Z2>::rand();

void solve() {
    u32 n, m, k;
    std::cin >> n >> m >> k;

    std::string s;
    std::cin >> s;

    mld::lazy_segment_tree<mld::actions::assign<operand>> st(
        s | std::views::transform([](char c) { return operand(c - '0'); }));

    u32 q = m + k;
    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 l, r, c;
            std::cin >> l >> r >> c;

            st.apply(--l, r, operand(c));
        } else {
            u32 l, r, d;
            std::cin >> l >> r >> d;

            --l;
            if (l == r - d) {
                std::cout << "YES\n";
                continue;
            }

            std::cout << (st.fold(l, r - d) == st.fold(l + d, r) ? "YES\n" : "NO\n");
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
