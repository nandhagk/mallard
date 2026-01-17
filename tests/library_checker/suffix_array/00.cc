#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/suffix_array.h"

void solve() {
    std::string s;
    std::cin >> s;

    for (auto &&x : mld::suffix_array(
             s | std::views::transform([](char c) { return c - 'a'; }), 26)) {
        std::cout << x << ' ';
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
