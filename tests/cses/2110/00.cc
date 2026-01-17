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

    std::vector<u32> ans(s.size() + 2);
    for (u32 i = 0, prv = 0; i < s.size(); ++i) {
        ++ans[prv + 1];
        --ans[s.size() - sa[i] + 1];
        if (i < s.size() - 1) prv = lcp[i];
    }

    std::inclusive_scan(ans.begin(), ans.end(), ans.begin());
    for (auto &&x : ans | std::views::drop(1) | std::views::take(s.size()))
        std::cout << x << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
