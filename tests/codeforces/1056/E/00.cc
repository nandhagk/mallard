#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/suffix_array_query.h"

void solve() {
    std::string s, t;
    std::cin >> s >> t;

    u32 a = static_cast<u32>(std::ranges::count(s, '0'));
    u32 b = static_cast<u32>(std::ranges::count(s, '1'));
    assert(a && b && a + b == s.size());

    u32 u = static_cast<u32>(std::ranges::find(s, '0') - s.begin());
    u32 v = static_cast<u32>(std::ranges::find(s, '1') - s.begin());

    mld::suffix_array_query saq(
        t | std::views::transform([](char c) { return c - 'a'; }), 26);

    u32 ans = 0;

    u32 n = static_cast<u32>(t.size());
    for (u32 x = 1; x <= n; ++x) {
        if (x * a >= n) break;

        u32 y = (n - x * a) / b;
        if (a * x + b * y != n) continue;

        u32 p = u * y, q = v * x;
        if (p + x > n || q + y > n || (x == y && saq.equate_substr(p, q, x))) continue;

        u32 i = 0;
        for (char c : s) {
            if (c == '0') {
                if (i + x > n || !saq.equate_substr(p, i, x)) goto invalid;
                i += x;
            } else {
                if (i + y > n || !saq.equate_substr(q, i, y)) goto invalid;
                i += y;
            }
        }

        ++ans;
    invalid:;
    }

    std::cout << ans << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
