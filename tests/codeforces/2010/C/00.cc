#include <bits/stdc++.h>

#include "lib/prelude.h"
#include "lib/kmp.h"

void solve() {
    std::string s;
    std::cin >> s;

    auto k = mld::kmp(s);
    if (k.back() > s.size() / 2) {
        std::cout << "YES\n";
        std::cout << s.substr(0, k.back());
    } else {
        std::cout << "NO\n";
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
