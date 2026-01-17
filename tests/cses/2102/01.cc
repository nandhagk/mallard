#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/suffix_array_query.h"
#include "lib/prelude.h"

void solve() {
    std::string s;
    std::cin >> s;

    mld::suffix_array_query saq(s | std::views::transform([](char c) { return c - 'a'; }), 26);

    u32 k;
    std::cin >> k;

    while (k--) {
        std::string p;
        std::cin >> p;

        auto [l, r, _, __] = saq.search(s, p);
        std::cout << (l != r ? "YES\n" : "NO\n");
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
