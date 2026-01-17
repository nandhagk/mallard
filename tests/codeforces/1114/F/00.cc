#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/mul_prod.h"
#include "lib/actions/or_or.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"
#include "lib/sieve.h"
#include "lib/static_montgomery_modint.h"

using Z = mld::montgomerymodint1000000007;
static constexpr u32 N = 300;

void solve() {
    mld::sieve s(N);

    std::vector<u64> m(N + 1);
    for (u32 i = 0; i < s.pri.size(); ++i)
        for (u32 j = s.pri[i]; j <= N; j += s.pri[i]) m[j] |= u64(1) << i;

    std::vector<Z> ito(N + 1);
    for (u32 p : s.pri) ito[p] = Z(p - 1) / p;

    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    mld::lazy_segment_tree<mld::actions::mul_prod<Z>> st(a);
    mld::lazy_segment_tree<mld::actions::or_or<u64>> dt(
        a | std::views::transform([&](u32 x) { return m[x]; }));

    while (q--) {
        std::string t;
        std::cin >> t;

        if (t == "MULTIPLY") {
            u32 l, r, x;
            std::cin >> l >> r >> x;

            --l;
            st.apply(l, r, x);
            dt.apply(l, r, m[x]);
        } else {
            u32 l, r;
            std::cin >> l >> r;

            --l;
            Z ans = st.fold(l, r).val();

            u64 k = dt.fold(l, r).val();
            for (u32 i = 0; i < s.pri.size(); ++i)
                if ((k >> i) & 1) ans *= ito[s.pri[i]];

            std::cout << ans.val() << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
