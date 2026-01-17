#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/suffix_array.h"
#include "lib/keyword_search.h"
#include "lib/wavelet_matrix.h"

void solve() {
    std::string s;
    std::cin >> s;

    auto sa = mld::suffix_array(s | std::views::transform([](char c) { return c - 'a'; }), 26);
    mld::wavelet_matrix wm(sa, static_cast<u32>(s.size()));

    u32 q;
    std::cin >> q;

    while (q--) {
        std::string t;
        u32 k;
        std::cin >> t >> k;

        --k;
        auto [l ,r] = mld::keyword_search(s, sa, t);
        if (k >= r - l) {
            std::cout << -1 << '\n';
        } else {
            std::cout << wm.kth(l, r, k) + 1 << '\n';
        }
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
