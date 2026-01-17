#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/keyword_search.h"
#include "lib/prelude.h"
#include "lib/suffix_array.h"

void solve() {
    std::string s;
    std::cin >> s;

    auto sa = mld::suffix_array(
        s | std::views::transform([](char c) { return c - 'a'; }), 26);

    u32 k;
    std::cin >> k;

    while (k--) {
        std::string p;
        std::cin >> p;

        auto [l, r] = mld::keyword_search(s, sa, p);
        std::cout << r - l << '\n';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
