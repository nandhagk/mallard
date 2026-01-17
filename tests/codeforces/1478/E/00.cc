#include <bits/stdc++.h>

#include "lib/actions/assign.h"
#include "lib/algebra/sum.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::string s, f;
    std::cin >> s >> f;

    std::vector<std::pair<u32, u32>> que(q);
    for (auto &&[l, r] : que) std::cin >> l >> r, --l;

    mld::lazy_segment_tree<mld::actions::assign<mld::algebra::sum<u32>>> st(
        f | std::views::transform([](char c) { return c - '0'; }));
    for (auto &&[l, r] : que | std::views::reverse) {
        u32 k = r - l;
        u32 z = st.fold(l, r).val();
        if (z > k / 2) {
            st.apply(l, r, 1);
        } else if (k - z > k / 2) {
            st.apply(l, r, 0);
        } else {
            std::cout << "NO\n";
            return;
        }
    }

    for (u32 i = 0; i < n; ++i) {
        if (st.get(i) != s[i] - '0') {
            std::cout << "NO\n";
            return;
        }
    }

    std::cout << "YES\n";
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
