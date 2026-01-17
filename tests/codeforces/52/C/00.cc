#include <bits/stdc++.h>

#include "lib/actions/add_min.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    mld::lazy_segment_tree<mld::actions::add_min<i64>> st(
        std::views::iota(u32{0}, n) | std::views::transform([](auto) {
            i32 a;
            std::cin >> a;
            return a;
        }));

    u32 m;
    std::cin >> m;

    std::cin.ignore();
    for (std::string line; m--;) {
        std::getline(std::cin, line);

        std::istringstream ss(line);

        u32 l, r;
        i32 v;
        if (ss >> l >> r >> v) {
            if (l <= r) {
                st.apply(l, r + 1, v);
            } else {
                st.apply(l, n, v);
                st.apply(0, r + 1, v);
            }
        } else {
            if (l <= r)
                std::cout << st.fold(l, r + 1).val() << '\n';
            else
                std::cout << (st.fold(l, n) + st.fold(0, r + 1)).val() << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
