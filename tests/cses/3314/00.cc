#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/max.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    mld::segment_tree<mld::algebra::max<u32>> sp(a);
    mld::segment_tree<mld::algebra::max<u32>> st(
        std::views::iota(u32{0}, n) | std::views::transform([](auto) { return 0; }));

    std::vector<u32> ord(n);
    std::iota(ord.begin(), ord.end(), 0);
    std::ranges::sort(ord, {}, [&](u32 k) { return std::tie(a[k], k); });

    for (u32 k : ord) {
        auto f = [&](auto &&x) { return x < a[k]; };

        u32 l = k == 0 ? 0 : sp.min_left(k, f);
        u32 r = k == n - 1 ? n : sp.max_right(k + 1, f);

        st.set(k, (l < r ? st.fold(l, r).val() : 0) + 1);
    }

    std::cout << st.fold().val() << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
