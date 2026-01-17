#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"

void solve() {
    u32 n, k;
    std::cin >> n >> k;

    std::vector<std::string> g(n);
    for (auto &&r : g) std::cin >> r;

    std::vector<bool> s(n * n);

    std::vector<u32> t;
    t.reserve(n);

    for (char c = 'A', d = static_cast<char>('A' + k); c < d; ++c) {
        s.assign(n * n, false);

        for (u32 i = 0; i < n; ++i) {
            t.clear();
            for (u32 j = 0; j < n; ++j)
                if (g[i][j] == c) t.push_back(j);

            for (u32 l = 0; l < t.size(); ++l) {
                for (u32 r = l + 1; r < t.size(); ++r) {
                    u32 z = t[r] * n + t[l];
                    if (s[z]) {
                        std::cout << "YES\n";
                        goto done;
                    }

                    s[z] = true;
                }
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
