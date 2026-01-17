#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/z_algorithm.h"
#include "lib/kmp.h"

void solve() {
    std::string s;
    std::cin >> s;

    std::cout << 0 << ' ';
    for (auto &&x : mld::z_algorithm(s) | std::views::drop(1)) std::cout << x << ' ';
    std::cout << '\n';

    for (auto &&x : mld::kmp(s)) std::cout << x << ' ';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
