#include <bits/stdc++.h>

#include "lib/manacher.h"
#include "lib/prelude.h"
#include "lib/static_modint.h"

using Z = mld::static_modint_32<51123987>;

void solve() {
    u32 n;
    std::cin >> n;

    std::string s;
    std::cin >> s;

    auto [o, e] = mld::manacher(s);

    std::vector<u64> pre(n + 1), suf(n + 1);
    for (u32 i = 0; i < n; ++i) ++pre[i - o[i] + 1], --pre[i + 1];
    for (u32 i = 0; i < n; ++i) ++pre[i - e[i]], --pre[i];

    for (u32 i = 0; i < n; ++i) --suf[i], ++suf[i + o[i]];
    for (u32 i = 0; i < n; ++i) --suf[i], ++suf[i + e[i]];

    std::inclusive_scan(pre.cbegin(), pre.cend(), pre.begin());
    std::inclusive_scan(suf.crbegin(), suf.crend(), suf.rbegin());

    Z tot = std::reduce(pre.begin(), pre.end(), Z(0));
    Z ans = tot * (tot - 1) / 2, cur = 0;
    for (u32 i = 0; i < n; cur += suf[++i]) ans -= cur * pre[i];

    assert(tot == cur);
    std::cout << ans.val() << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
