#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/lyndon_factorization.h"

void solve() {
    std::string s;
    std::cin >> s;

    for (auto &&[x, _] : mld::lyndon_factorization(s)) std::cout << x << ' ';
    std::cout << s.size() << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
