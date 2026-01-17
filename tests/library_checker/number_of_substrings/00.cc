#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/lcp_array.h"
#include "lib/prelude.h"
#include "lib/suffix_array.h"

void solve() {
    std::string s;
    std::cin >> s;

    auto sa = mld::suffix_array(
        s | std::views::transform([](char c) { return c - 'a'; }), 26);
    auto lcp = mld::lcp_array(s, sa);

    u64 ans =
        s.size() * (s.size() + 1) / 2 - std::reduce(lcp.begin(), lcp.end(), u64{0});
    std::cout << ans << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
