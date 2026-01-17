#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/rabin_karp_rev.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"
#include "lib/static_montgomery_modint.h"

using Z1 = mld::montgomerymodint998244353;
using Z2 = mld::montgomerymodint1000000007;

using operand = mld::algebra::rabin_karp_rev<Z1, Z2>;

template <>
std::tuple<Z1, Z2> operand::base = mld::algebra::rabin_karp_rev<Z1, Z2>::rand();

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::string s;
    s.reserve(n);

    std::cin >> s;

    mld::segment_tree<operand> st(
        s | std::views::transform([](char c) { return operand(c); }));

    while (m--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 k;
            char x;
            std::cin >> k >> x;

            st.set(--k, operand(x));
        } else {
            u32 l, r;
            std::cin >> l >> r;

            auto p = st.fold(--l, r);
            std::cout << (p == +p ? "YES\n" : "NO\n");
        }
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
