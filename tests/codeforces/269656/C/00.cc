#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/suffix_array_query.h"

void solve() {
    std::string s;
    std::cin >> s;

    mld::suffix_array_query saq(
        s | std::views::transform([](char c) { return c - 33; }), 95);

    u32 q;
    std::cin >> q;

    std::vector<std::pair<u32, u32>> sub(q);
    for (auto &&[l, r] : sub) std::cin >> l >> r, --l;

    std::ranges::sort(sub, [&](auto &&a, auto &&b) {
        auto &&[x, y] = a;
        auto &&[u, v] = b;
        auto z = saq.compare_substr(x, y, u, v);
        return z == 0 ? x < u : z < 0;
    });

    for (auto &&[l, r] : sub) std::cout << l + 1 << ' ' << r << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
