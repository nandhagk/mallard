#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/z_algorithm.h"

void solve() {
    std::string s;
    std::cin >> s;

    auto z = mld::z_algorithm(s);
    for (u32 i = 1; i < s.size(); ++i)
        if (i + z[i] == s.size()) std::cout << i << ' ';

    std::cout << s.size() << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
