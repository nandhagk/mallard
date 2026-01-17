#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/lor.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

void solve() {
    std::string s;
    std::cin >> s;

    mld::segment_tree<mld::algebra::lor<u32>> st(
        s | std::views::transform([](char c) { return 1 << (c - 'a'); }));

    u32 q;
    std::cin >> q;

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 p;
            char c;
            std::cin >> p >> c;

            st.set(--p, 1 << (c - 'a'));
        } else {
            u32 l, r;
            std::cin >> l >> r;

            std::cout << std::popcount(st.fold(--l, r).val()) << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
