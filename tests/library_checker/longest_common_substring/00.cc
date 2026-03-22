#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/lcp_array.h"
#include "lib/prelude.h"
#include "lib/suffix_array.h"

void solve() {
    std::string s, t;
    std::cin >> s >> t;

    u32 n = static_cast<u32>(s.size());
    u32 m = static_cast<u32>(t.size());
    u32 k = n + m + 1;

    std::string u(k, '\0');
    std::ranges::copy(s, u.begin());
    u[n] = '`';
    std::ranges::copy(t, u.begin() + n + 1);

    auto sa = mld::suffix_array(
        u | std::views::transform([](char c) { return c - '`'; }), 27);
    auto lcp = mld::lcp_array(u, sa);

    u32 ans = 0, l = 0, r = 0;
    for (u32 i = 0; i < k - 1; ++i) {
        if (lcp[i] <= ans) continue;

        if (sa[i] < n && sa[i + 1] >= n + 1) {
            ans = lcp[i];
            l = sa[i];
            r = sa[i + 1] - (n + 1);
        } else if (sa[i] >= n + 1 && sa[i + 1] < n) {
            ans = lcp[i];
            l = sa[i + 1];
            r = sa[i] - (n + 1);
        }
    }

    std::cout << l << ' ' << l + ans << ' ' << r << ' ' << r + ans << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
