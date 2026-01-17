#include <bits/stdc++.h>

#include "lib/am_tree.h"
#include "lib/offline/dynamic_connectivity.h"
#include "lib/prelude.h"
#include "lib/static_csr.h"

static constexpr u32 Q = 500'000;

void solve() {
    u32 n, m, q;
    std::cin >> n >> m >> q;

    mld::static_csr<u32> g(std::views::iota(u32{0}, n) |
                           std::views::transform([m](auto) { return m; }));

    auto f = [](auto &&am, u32) { std::cout << am.ccs() << '\n'; };

    mld::offline::dynamic_connectivity<mld::am_tree<i32>> dct;
    dct.reserve(2 * n * m + 4 * Q);
    for (u32 k = 0; k < q;) {
        dct.build(n * m);

        for (u32 i = 0; i < n; ++i) {
            for (u32 j = 0; j < m; ++j) {
                if (i > 0 && g[i - 1][j] == g[i][j])
                    dct.link(i * m + j, (i - 1) * m + j);
                if (j > 0 && g[i][j - 1] == g[i][j])
                    dct.link(i * m + j, i * m + (j - 1));
            }
        }

        for (u32 t = 0; t < Q && k < q; ++t, ++k) {
            u32 x, y, c;
            std::cin >> x >> y >> c;

            --x, --y;
            if (g[x][y] == c) {
                dct.query(f);
                continue;
            }

            if (x > 0) {
                if (g[x - 1][y] == g[x][y])
                    dct.cut(x * m + y, (x - 1) * m + y);
                else if (g[x - 1][y] == c)
                    dct.link(x * m + y, (x - 1) * m + y);
            }

            if (y > 0) {
                if (g[x][y - 1] == g[x][y])
                    dct.cut(x * m + y, x * m + (y - 1));
                else if (g[x][y - 1] == c)
                    dct.link(x * m + y, x * m + (y - 1));
            }

            if (x + 1 < n) {
                if (g[x + 1][y] == g[x][y])
                    dct.cut(x * m + y, (x + 1) * m + y);
                else if (g[x + 1][y] == c)
                    dct.link(x * m + y, (x + 1) * m + y);
            }

            if (y + 1 < m) {
                if (g[x][y + 1] == g[x][y])
                    dct.cut(x * m + y, x * m + (y + 1));
                else if (g[x][y + 1] == c)
                    dct.link(x * m + y, x * m + (y + 1));
            }

            g[x][y] = c;
            dct.query(f);
        }

        mld::am_tree<i32> amt(n * m);
        dct.solve(amt);
    }
}

i32 main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    solve();
}
