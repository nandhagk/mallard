#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/z_algorithm.h"

void solve() {
    std::string s, t;
    std::cin >> s >> t;

    auto z = mld::z_algorithm(t + '#' + s);
    std::cout << std::count(z.begin() + t.size() + 1, z.end(), t.size()) << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
