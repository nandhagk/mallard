#include <bits/stdc++.h>

#include "lib/actions/assign.h"
#include "lib/algebra/min.h"
#include "lib/compress.h"
#include "lib/four_russian.h"
#include "lib/lazy_segment_tree.h"
#include "lib/prelude.h"
#include "lib/sparse_table.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    mld::four_russian<mld::sparse_table<mld::algebra::min<u32>>> st(
        std::views::iota(u32{0}, n) | std::views::transform([](auto) {
            u32 a;
            std::cin >> a;
            return a;
        }));

    u32 q;
    std::cin >> q;

    std::vector<u32> p;
    p.reserve(2 * q);

    std::vector<u32> que;
    que.reserve(q);

    constexpr u32 UNSET = -1;
    for (u32 i = 0; i < q; ++i) {
        u32 t, l, r;
        std::cin >> t >> l >> r;

        p.push_back(--l), p.push_back(r);
        if (t == 1) {
            u32 x;
            std::cin >> x;

            que.push_back(x);
        } else {
            que.push_back(UNSET);
        }
    }

    auto min = [&](u32 l, u32 r) {
        if (r - l >= n) return st.fold(0, n).val();

        l = l % n, r = ((r - 1) % n) + 1;
        if (l < r) return st.fold(l, r).val();
        return (st.fold(l, n) + st.fold(0, r)).val();
    };

    auto [v, na] = mld::compress(p);

    std::vector<u32> a(v.size());
    for (u32 i = 0; i < static_cast<u32>(v.size()) - 1; ++i) a[i] = min(v[i], v[i + 1]);

    mld::lazy_segment_tree<mld::actions::assign<mld::algebra::min<u32>>> dt(a);
    for (u32 i = 0; i < q; ++i) {
        u32 l = na[i << 1 | 0], r = na[i << 1 | 1];
        if (que[i] == UNSET)
            std::cout << dt.fold(l, r).val() << '\n';
        else
            dt.apply(l, r, que[i]);
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
