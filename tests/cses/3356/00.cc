#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/min.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    std::set<std::pair<u32, u32>> z;
    for (auto &&x : a) z.emplace(x, z.size());

    mld::segment_tree<mld::algebra::min<u32>> st(
        std::views::iota(u32{0}, n) | std::views::transform([&](u32 k) {
            if (auto it = z.upper_bound({a[k], k}); it != z.end() && it->first == a[k])
                return it->second;
            return n;
        }));

    auto add = [&](u32 k) {
        auto [cur, _] = z.emplace(a[k], k);

        if (cur != z.begin()) {
            auto prv = std::prev(cur);
            if (prv->first == a[k]) {
                st.set(k, st.get(prv->second));
                st.set(prv->second, k);

                return;
            }
        }

        auto nxt = std::next(cur);
        st.set(k, nxt != z.end() && nxt->first == a[k] ? nxt->second : n);
    };

    auto rem = [&](u32 k) {
        auto cur = z.lower_bound({a[k], k});
        if (cur != z.begin()) {
            auto prv = std::prev(cur);
            if (prv->first == a[k]) st.set(prv->second, st.get(k));
        }

        z.erase(cur);
    };

    while (q--) {
        u32 t;
        std::cin >> t;

        if (t == 1) {
            u32 k, u;
            std::cin >> k >> u;

            --k;
            rem(k), a[k] = u, add(k);
        } else {
            u32 l, r;
            std::cin >> l >> r;

            std::cout << (st.fold(--l, r).val() < r ? "NO\n" : "YES\n");
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
