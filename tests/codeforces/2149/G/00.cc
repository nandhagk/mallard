#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/misra_gries.h"
#include "lib/disjoint_sparse_table.h"
#include "lib/four_russian.h"
#include "lib/prelude.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<std::pair<u32, u32>> p;
    p.reserve(n);

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x, p.emplace_back(x, p.size());

    std::ranges::sort(p);

    using operand = mld::algebra::misra_gries<u32, 3>;
    mld::four_russian<mld::disjoint_sparse_table<operand>> st(
        a | std::views::transform([](u32 x) { return operand(x, 1); }));

    std::vector<u32> ans;
    ans.reserve(3);

    while (q--) {
        u32 l, r;
        std::cin >> l >> r;

        ans.clear();
        for (auto &&[t, _] : st.fold(--l, r).val()) {
            u32 z = std::ranges::lower_bound(p, std::make_pair(t, r)) -
                    std::ranges::lower_bound(p, std::make_pair(t, l));
            if (z > (r - l) / 3) ans.push_back(t);
        }

        if (ans.empty()) {
            std::cout << -1 << '\n';
        } else {
            std::ranges::sort(ans);
            for (auto &&x : ans) std::cout << x << ' ';
            std::cout << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    u32 t;
    std::cin >> t;

    while (t--) solve();
}
