#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/lcp_array.h"
#include "lib/prelude.h"
#include "lib/suffix_array.h"

void solve() {
    std::string s;
    std::cin >> s;

    if (s.size() < 2) {
        std::cout << -1 << '\n';
        return;
    }

    auto sa = mld::suffix_array(
        s | std::views::transform([](char c) { return c - 'a'; }), 26);
    auto lcp = mld::lcp_array(s, sa);

    auto it = std::ranges::max_element(lcp);
    if (*it == 0) {
        std::cout << -1 << '\n';
        return;
    }

    std::cout << s.substr(sa[it - lcp.begin()], *it) << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
