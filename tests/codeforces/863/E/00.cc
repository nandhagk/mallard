#include <bits/stdc++.h>

#include "lib/actions/add_min.h"
#include "lib/compress.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a;
    a.reserve(2 * n);

    std::vector<std::pair<u32, u32>> p(n);
    for (auto &&[l, r] : p) std::cin >> l >> r, a.push_back(l), a.push_back(++r);

    auto [v, na] = mld::compress(a);

    mld::lazy_segment_tree<mld::actions::add_min<u32>> st(
        std::views::iota(u32{0}, static_cast<u32>(v.size())) |
        std::views::transform([](auto) { return 0; }));
    for (auto &&[l, r] : p) {
        l = static_cast<u32>(std::ranges::lower_bound(v, l) - v.begin());
        r = static_cast<u32>(std::ranges::lower_bound(v, r) - v.begin());
        st.apply(l, r, 1);
    }

    for (u32 i = 0; i < n; ++i) {
        auto &&[l, r] = p[i];
        if (st.fold(l, r) > 1) {
            std::cout << i + 1 << '\n';
            return;
        }
    }

    std::cout << -1 << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
