#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "ska_sort.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) std::cin >> x;

    ska::ska_sort(a.begin(), a.end());
    std::cout << std::unique(a.begin(), a.end()) - a.begin() << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
