#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/z_algorithm.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::string s;
    s.reserve(n);

    std::cin >> s;

    for (auto &&x : mld::z_algorithm(s)) std::cout << x << ' ';
    std::cout << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
