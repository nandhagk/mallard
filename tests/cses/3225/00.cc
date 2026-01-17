#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/inverse_suffix_array.h"
#include "lib/prelude.h"

void solve() {
    u32 n;
    std::cin >> n;

    std::vector<u32> a(n);
    for (auto &&x : a) {
        std::cin >> x;
        --x;
    }

    auto s = mld::inverse_suffix_array(a);
    if (*std::ranges::max_element(s) >= 26) {
        std::cout << -1 << '\n';
    } else {
        for (auto &&x : s) std::cout << static_cast<char>(x + 'a');
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
