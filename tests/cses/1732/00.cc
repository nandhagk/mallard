#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/kmp.h"
#include "lib/prelude.h"

void solve() {
    std::string s;
    std::cin >> s;

    auto k = mld::kmp(s);

    std::vector<u32> ans;
    ans.reserve(s.size());

    for (u32 j = k.back(); j > 0; j = k[j - 1]) ans.push_back(j);
    for (auto &&x : ans | std::views::reverse) std::cout << x << ' ';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
