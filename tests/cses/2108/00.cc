#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/lcp_array.h"
#include "lib/prelude.h"
#include "lib/suffix_array.h"

void solve() {
    std::string s;
    std::cin >> s;

    u64 k;
    std::cin >> k;

    auto sa = mld::suffix_array(
        s | std::views::transform([](char c) { return c - 'a'; }), 26);

    auto lcp = mld::lcp_array(s, sa);
    lcp.insert(lcp.begin(), 0);

    for (u64 i = 0, cur = 0; i < s.size(); ++i) {
        if (cur + (s.size() - sa[i]) - lcp[i] >= k) {
            std::cout << s.substr(sa[i], lcp[i] + k - cur) << '\n';
            return;
        }

        cur += (s.size() - sa[i]) - lcp[i];
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
