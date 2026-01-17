#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/enumerate_runs.h"
#include "lib/prelude.h"

void solve() {
    std::string s;
    std::cin >> s;

    auto runs = mld::enumerate_runs(
        s | std::views::transform([](char c) { return c - 'a'; }), 0, 26);

    std::cout << runs.size() << '\n';
    for (auto &&[t, l, r] : runs) std::cout << t << ' ' << l << ' ' << r << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
