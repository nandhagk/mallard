#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    std::vector<std::string> g(n);
    for (auto &&r : g) std::cin >> r;

    std::vector<std::bitset<3072>> s(n);

    for (char c = 'A', d = static_cast<char>('A' + k); c < d; ++c) {
        for (auto &&b : s) b.reset();
        for (u32 i = 0; i < n; ++i) {
            std::bitset<3072> t;
            for (u32 j = 0; j < n; ++j) {
                if (g[i][j] != c) continue;

                if ((s[j] & t).any()) {
                    std::cout << "YES\n";
                    goto done;
                }

                s[j] |= t;
                t.set(j);
            }
        }

        std::cout << "NO\n";
        done:;
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
