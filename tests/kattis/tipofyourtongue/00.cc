#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/compress.h"
#include "lib/prelude.h"
#include "lib/wavelet_matrix.h"
#include "ska_sort.h"

void solve() {
    u32 n, q;
    std::cin >> n >> q;

    std::vector<std::string> ws(n);
    for (auto &&x : ws) std::cin >> x;

    auto zs = ws;
    for (auto &&s : zs) std::ranges::reverse(s);

    // std::ranges::sort(ws);
    // std::ranges::sort(zs);
    ska::ska_sort(ws.begin(), ws.end());
    ska::ska_sort(zs.begin(), zs.end());

    auto [xs, na] = mld::compress(ws | std::views::transform([](auto s) {
                                      std::ranges::reverse(s);
                                      return s;
                                  }));

    mld::wavelet_matrix wm(na, static_cast<u32>(xs.size()));

    while (q--) {
        std::string t, p, s;
        std::cin >> t >> p >> s;

        std::ranges::reverse(s);

        auto u = p, v = s;
        ++u.back(), ++v.back();

        u32 l = static_cast<u32>(std::ranges::lower_bound(ws, p) - ws.begin());
        u32 r = static_cast<u32>(std::ranges::lower_bound(ws, u) - ws.begin());

        u32 a = static_cast<u32>(std::ranges::lower_bound(zs, s) - zs.begin());
        u32 b = static_cast<u32>(std::ranges::lower_bound(zs, v) - zs.begin());

        u32 y = static_cast<u32>(std::ranges::lower_bound(xs, s) - xs.begin());
        u32 z = static_cast<u32>(std::ranges::lower_bound(xs, v) - xs.begin());

        u32 c = l == r ? 0 : wm.rank(l, r, z) - wm.rank(l, r, y);
        if (t == "AND")
            std::cout << c << '\n';
        else if (t == "OR")
            std::cout << r - l + b - a - c << '\n';
        else if (t == "XOR")
            std::cout << r - l + b - a - 2 * c << '\n';
        else
            assert(0);
    }
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
