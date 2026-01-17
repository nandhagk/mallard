#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/manacher.h"
#include "lib/prelude.h"

void solve() {
    std::string s;
    std::cin >> s;

    auto [odd, even] = mld::manacher(s);

    auto a = std::ranges::max_element(odd);
    auto b = std::ranges::max_element(even);

    if (u32 x = 2 * (*a) - 1, y = 2 * (*b); x > y)
        std::cout << s.substr(a - odd.begin() - *a + 1, x);
    else
        std::cout << s.substr(b - even.begin() - *b, y);
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
