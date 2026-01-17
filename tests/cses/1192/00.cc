#include <bits/stdc++.h>

#include "blazingio.min.h"
#include "lib/prelude.h"
#include "lib/union_find.h"

void solve() {
    u32 n, m;
    std::cin >> n >> m;

    std::vector<std::string> g(n);
    for (auto &&r : g) {
        r.reserve(m + 1);
        std::cin >> r;
    }

    mld::union_find uf(n * m);

    for (u32 i = 0; i < n; ++i) {
        for (u32 j = 0; j < m; ++j) {
            if (g[i][j] == '#') continue;

            if (i != 0 && g[i - 1][j] == '.') uf.merge(i * m + j, (i - 1) * m + j);
            if (j != 0 && g[i][j - 1] == '.') uf.merge(i * m + j, i * m + (j - 1));
        }
    }

    std::cout << uf.ccs() - std::transform_reduce(
                                g.begin(), g.end(), 0, std::plus{},
                                [](auto &&r) { return std::ranges::count(r, '#'); })
              << '\n';
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
