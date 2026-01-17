#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/algebra/max.h"
#include "lib/compress.h"
#include "lib/prelude.h"
#include "lib/segment_tree.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<std::tuple<u32, u32, u32>> q;
    std::vector<std::pair<u32, u32>> p;

    p.reserve(n), q.reserve(n);
    for (u32 i = 0; i < n; ++i) {
        std::string t;
        u32 x, y;
        std::cin >> t >> x >> y;

        p.emplace_back(x, y);

        u32 k = t == "add" ? 0 : t == "remove" ? 1 : 2;
        q.emplace_back(k, x, y);
    }

    auto [v, na] = mld::compress(p);
    u32 m = static_cast<u32>(v.size());

    mld::segment_tree<mld::algebra::max<u32>> st(
        std::views::iota(u32{0}, m) | std::views::transform([](auto) { return 0; }));

    auto id = [&](u32 x, u32 y) {
        return static_cast<u32>(
            std::lower_bound(v.begin(), v.end(), std::pair<u32, u32>{x, y}) -
            v.begin());
    };

    for (auto &&[t, x, y] : q) {
        if (t == 0) {
            st.set(id(x, y), y);
        } else if (t == 1) {
            st.set(id(x, y), 0);
        } else {
            u32 k = id(x + 1, y + 1);
            if (k == m) {
                std::cout << -1 << '\n';
                continue;
            }

            k = st.max_right(k, [&](auto &&z) { return z <= y; });
            if (k == m) {
                std::cout << -1 << '\n';
                continue;
            }

            auto &&[a, b] = v[k];
            std::cout << a << ' ' << b << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
