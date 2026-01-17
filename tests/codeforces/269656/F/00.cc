#include <bits/stdc++.h>

#include "lib/algebra/min.h"
#include "lib/four_russian.h"
#include "lib/lcp_array.h"
#include "lib/prelude.h"
#include "lib/sparse_table.h"
#include "lib/suffix_array.h"

void solve() {
    std::string s;
    std::cin >> s;

    u32 n = static_cast<u32>(s.size());
    if (n == 1) {
        std::cout << 1 << '\n';
        return;
    }

    auto t = s;
    std::ranges::reverse(t);

    auto sa = mld::suffix_array(
        s | std::views::transform([](char c) { return c - 'a'; }), 26);
    auto ta = mld::suffix_array(
        t | std::views::transform([](char c) { return c - 'a'; }), 26);

    std::vector<u32> sr(n), tr(n);
    for (u32 i = 0; i < n; ++i) sr[sa[i]] = i;
    for (u32 i = 0; i < n; ++i) tr[ta[i]] = i;

    auto lcp = mld::lcp_array(s, sa);
    auto lcs = mld::lcp_array(t, ta);

    mld::four_russian<mld::sparse_table<mld::algebra::min<u32>>> rp(lcp), rs(lcs);

    u32 out = 0, cur = 1;
    for (u32 i = 1; i < n; ++i, ++cur)
        if (s[i] != s[i - 1]) out = std::max(out, cur), cur = 0;

    out = std::max(out, cur);
    for (u32 l = 2; l <= n; ++l) {
        for (u32 i = l; i < n; i += l) {
            auto [a, b] = std::minmax(sr[i - l], sr[i]);
            auto [c, d] = std::minmax(tr[n - i - 1], tr[n - i - 1 + l]);

            u32 p = rp.fold(a, b).val(), q = rs.fold(c, d).val();
            out = std::max(out, (p + q + l - 1) / l);
        }
    }

    std::cout << out << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
