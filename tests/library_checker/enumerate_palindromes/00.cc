#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/manacher.h"
#include "lib/prelude.h"

void solve() {
    std::string s;
    std::cin >> s;

    auto &&[odd, even] = mld::manacher(s);

    std::cout << odd[0] * 2 - 1 << ' ';
    for (u32 i = 1; i < s.size(); ++i)
        std::cout << even[i] * 2 << ' ' << odd[i] * 2 - 1 << ' ';

    std::cout << '\n';
}

i32 main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
