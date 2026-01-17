#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/rabin_karp_rev.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"
#include "lib/static_montgomery_modint.h"

using Z1 = mld::montgomerymodint998244353;

using operand = mld::algebra::rabin_karp_rev<Z1>;

template <>
std::tuple<Z1> operand::base = mld::algebra::rabin_karp_rev<Z1>::rand();

void solve() {
    std::string s;
    std::cin >> s;

    u32 n = static_cast<u32>(s.size());
    mld::segment_tree<operand> st(
        s | std::views::transform([](char c) { return operand(c); }));

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 t, p;
        std::cin >> t >> p;

        --p;
        if (t == 1) {
            char x;
            std::cin >> x;

            s[p] = x;
            st.set(p, operand(x));
        } else if (t == 3) {
            if (s[p] != s[p + 1]) {
                std::cout << -1 << '\n';
                continue;
            }

            u32 lo = 0, hi = std::min(p + 1, n - p - 1) + 1;
            while (hi - lo > 1) {
                u32 m = std::midpoint(lo, hi);
                auto x = st.fold(p - m + 1, p + 1);
                auto y = st.fold(p + 1, p + m + 1);
                if (x == +y)
                    lo = m;
                else
                    hi = m;
            }

            std::cout << 2 * lo << '\n';
        } else {
            u32 lo = 0, hi = std::min(p + 1, n - p) + 1;
            while (hi - lo > 1) {
                u32 m = std::midpoint(lo, hi);
                auto x = st.fold(p - m + 1, p + 1);
                auto y = st.fold(p, p + m);
                if (x == +y)
                    lo = m;
                else
                    hi = m;
            }

            std::cout << 2 * lo - 1 << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
