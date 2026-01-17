#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/actions/or_and.h"
#include "lib/lazy_segment_tree.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::tuple<u32, u32, u32>> q(m);

    mld::lazy_segment_tree<mld::actions::or_and<u32>> st(
        std::views::iota(u32{0}, n) | std::views::transform([](auto) { return 0; }));

    for (auto &&[l, r, x] : q) std::cin >> l >> r >> x, --l;
    for (auto &&[l, r, x] : q) st.apply(l, r, x);

    for (auto &&[l, r, x] : q) {
        if (st.fold(l, r).val() != x) {
            std::cout << "NO\n";
            return;
        }
    }

    std::cout << "YES\n";
    for (u32 i = 0; i < n; ++i) std::cout << st.get(i).val() << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
